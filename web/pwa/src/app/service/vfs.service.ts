import { Attributes, FileEntry, ReaddirError, Vfs, VfsResult } from '@common/bridge/Vfs';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { FileService } from '@pwa/service/file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { StorageService } from '@pwa/service/storage.service';
import deepEqual from 'deep-equal';
import { filenameForArchive } from '@pwa/helper/filename';

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(
        private storageService: StorageService,
        private storageCardContext: StorageCardContext,
        private alertService: AlertService,
        private fileService: FileService,
        private loadingController: LoadingController
    ) {
        void this.vfs.then((instance) => (this.vfsInstance = instance));
    }

    normalizePath(path: string): string {
        return ('/' + path).replace(/\/{2,}/g, '/').replace(/\/*$/, '');
    }

    splitPath(path: string): Array<string> {
        return this.normalizePath(path).replace(/^\/+/, '').split('/');
    }

    dirname(path: string): string {
        return this.normalizePath(path).replace(/\/[^\/]*$/, '');
    }

    isFilenameValid(name: string | undefined): boolean {
        return name === undefined ? true : !/[\/\\]/.test(name);
    }

    async mountCardUnchecked(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        this.storageCardContext.claim(id, CardOwner.vfs);

        this.directoryCache.clear();

        if (this.mountedCard?.id === id) return true;
        if (this.mountedCard) await this.releaseCard(this.mountedCard.id);

        const vfs = await this.vfs;

        vfs.allocateImage(card.size);
        const pendingImage = vfs.getPendingImage();
        if (!pendingImage) throw new Error('failed to allocate image');

        if (data) {
            pendingImage.set(data);
        } else {
            await this.storageService.loadCardData(id, pendingImage, CardOwner.vfs);
        }

        if (!vfs.mountImage(0)) return false;

        this.mountedCard = card;
        this.bytesFree = vfs.bytesFree(0);
        this.bytesTotal = vfs.bytesTotal(0);

        return true;
    }

    async releaseCard(id = this.mountedCard?.id) {
        if (id === undefined || id !== this.mountedCard?.id) return;

        (await this.vfs).unmountImage(0);
        this.mountedCard = undefined;

        this.storageCardContext.release(id, CardOwner.vfs);

        this.onReleaseCard.dispatch(id);
    }

    readdir(path: string): Array<FileEntry> {
        // This cannot happen, as the FS needs to be mounted before it can be used.
        if (!this.vfsInstance) return [];

        const normalizedPath = this.normalizePath(path);

        if (!this.directoryCache.has(normalizedPath)) {
            const readdirResult = this.vfsInstance.readdir(path);

            this.directoryCache.set(
                normalizedPath,
                readdirResult.error === ReaddirError.none ? readdirResult.entries : []
            );
        }

        return this.directoryCache.get(normalizedPath) || [];
    }

    async updateFileEntry(path: string, changes: { name?: string; attributes: Partial<Attributes> }): Promise<void> {
        const vfs = await this.vfs;
        path = this.normalizePath(path);

        if (!this.isFilenameValid(changes.name)) {
            await this.error('The provided file name is invalid.');
            return;
        }

        const { result, entry } = vfs.stat(path);
        if (result !== VfsResult.FR_OK) {
            await this.error(`The path ${path} is invalid. This is a bug in Cloudpilot.`);
            return;
        }

        if (changes.attributes !== undefined && !deepEqual(changes.attributes, entry.attributes)) {
            this.directoryCache.delete(this.dirname(path));

            switch (vfs.chmod(path, changes.attributes)) {
                case VfsResult.FR_OK:
                    break;

                default:
                    await this.fatalError(`Unable to write the updated file attributes.`);
                    return;
            }
        }

        if (changes.name !== undefined && changes.name !== entry.name) {
            this.directoryCache.delete(this.dirname(path));

            switch (vfs.rename(path, `${this.dirname(path)}/${changes.name}`)) {
                case VfsResult.FR_OK:
                    break;

                case VfsResult.FR_INVALID_NAME:
                    await this.error('The provided file name is invalid.');
                    return;

                default:
                    await this.fatalError('Unable to write the updated file attributes.');
                    return;
            }
        }

        await this.sync();
    }

    async saveFile(entry: FileEntry): Promise<void> {
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const vfs = await this.vfs;
            const content = vfs.readFile(entry.path);

            if (!content) {
                await this.alertService.errorMessage(`Could not read file '${entry.name}'.`);
                return;
            }

            this.fileService.saveFile(entry.name, content);
        } finally {
            await loader.dismiss();
        }
    }

    async archiveFiles(entries: Array<FileEntry>, prefix: string): Promise<void> {
        if (entries.length === 0 || !this.mountedCard) return;

        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const vfs = await this.vfs;
            const directories = entries.filter((entry) => entry.isDirectory).map((entry) => entry.path);
            const files = entries.filter((entry) => !entry.isDirectory).map((entry) => entry.path);
            const archiveName = entries.length === 1 ? `${entries[0].name}.zip` : filenameForArchive(this.mountedCard);

            const { archive, failedItems } = await vfs.createZipArchive({
                directories,
                files,
                prefix,
            });

            await loader.dismiss();

            if (failedItems.length > 0) {
                if (failedItems.length <= 3) {
                    await this.alertService.message(
                        'Warning',
                        `The following files were not archived correctly due to errors: <br><br>${failedItems.join(
                            '<br>'
                        )}`
                    );
                } else {
                    await this.alertService.message(
                        'Warning',
                        `The following files were not archived correctly due to errors: <br><br>${failedItems
                            .slice(0, 3)
                            .join('<br>')}<br><br> and ${failedItems.length - 3} other file`
                    );
                }
            }

            if (archive) this.fileService.saveFile(archiveName, archive);
        } finally {
            await loader.dismiss();
        }
    }

    currentCard(): StorageCard | undefined {
        return this.mountedCard;
    }

    getBytesFree(): number {
        return this.bytesFree;
    }

    getBytesTotal(): number {
        return this.bytesTotal;
    }

    private async error(message: string): Promise<void> {
        await this.alertService.errorMessage(message);
    }

    private async fatalError(message: string): Promise<void> {
        if (!this.mountedCard) return;

        await this.storageService.updateStorageCardPartial(this.mountedCard.id, { status: StorageCardStatus.dirty });
        await this.releaseCard();

        void this.alertService.errorMessage(`${message}
        <br><br>
        No changes have been written, and the card has been unmounted to prevent damage to the file system.
        `);
    }

    private async sync(): Promise<void> {
        if (!this.mountedCard) return;
        const vfs = await this.vfs;

        const data = vfs.getImageInSlot(0);
        const dirtyPages = vfs.getDirtyPagesForSlot(0);

        if (!data || !dirtyPages) return;

        await this.storageService.updateCardData(this.mountedCard.id, data, dirtyPages, CardOwner.vfs);
    }

    readonly onReleaseCard = new Event<number>();

    private mountedCard: StorageCard | undefined;
    private vfs = Vfs.create();
    private vfsInstance: Vfs | undefined;

    private directoryCache = new Map<string, Array<FileEntry>>();
    private bytesFree = 0;
    private bytesTotal = 0;
}
