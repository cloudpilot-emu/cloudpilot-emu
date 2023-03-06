import { AlertController, LoadingController } from '@ionic/angular';
import { Attributes, FileEntry, ReaddirError, UnzipResult, Vfs, VfsResult, WriteFileResult } from '@common/bridge/Vfs';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { StorageService } from '@pwa/service/storage.service';
import deepEqual from 'deep-equal';
import { filenameForArchive } from '@pwa/helper/filename';
import { ucFirst } from '@pwa/helper/text';

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(
        private storageService: StorageService,
        private storageCardContext: StorageCardContext,
        private alertService: AlertService,
        private fileService: FileService,
        private loadingController: LoadingController,
        private alertController: AlertController
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

        const loader = await this.loadingController.create({ message: 'Creating archive...' });
        await loader.present();

        try {
            const vfs = await this.vfs;
            const directories = entries
                .filter((entry) => entry.isDirectory)
                .map((entry) => this.normalizePath(entry.path));
            const files = entries.filter((entry) => !entry.isDirectory).map((entry) => this.normalizePath(entry.path));
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

    async unlinkEntry(entry: FileEntry): Promise<void> {
        if (entry.isDirectory) return;

        const vfs = await this.vfs;
        const result = vfs.unlinkFile(entry.path);

        if (result !== VfsResult.FR_OK) {
            console.error(`failed to unlink ${entry.path}: ${result}`);

            await this.error(`Failed to delete: ${entry.path}`);
            return;
        }

        this.directoryCache.delete(this.dirname(entry.path));
        await this.sync();
    }

    async deleteRecursive(entries: Array<FileEntry>): Promise<void> {
        const files = entries.filter((entry) => !entry.isDirectory).map((entry) => this.normalizePath(entry.path));
        const directories = entries.filter((entry) => entry.isDirectory).map((entry) => this.normalizePath(entry.path));

        const loader = await this.loadingController.create({ message: 'Deleting...' });
        await loader.present();

        try {
            const vfs = await this.vfs;
            const result = await vfs.deleteRecursive({ files, directories });

            if (!result.success) {
                await loader.dismiss();
                await this.fatalError(`Failed to delete ${result.failingItem}`);

                return;
            }

            entries.map((entry) => entry.path).forEach((path) => this.directoryCache.delete(this.dirname(path)));
            await this.sync();
        } finally {
            await loader.dismiss();
        }
    }

    async mkdir(path: string): Promise<void> {
        const vfs = await this.vfs;

        switch (vfs.mkdir(path)) {
            case VfsResult.FR_INVALID_NAME:
                await this.alertService.errorMessage('Invalid name.');
                return;

            case VfsResult.FR_DENIED:
                await this.alertService.errorMessage('No space left on card.');
                return;

            case VfsResult.FR_EXIST:
                await this.alertService.errorMessage('Name already taken.');
                return;

            case VfsResult.FR_OK:
                break;

            default:
                await this.error(`Failed to create ${path}.`);
                return;
        }

        await this.sync();
        this.directoryCache.delete(this.dirname(path));
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

    async unpackArchive(zip: Uint8Array, destination: string): Promise<void> {
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const { entriesTotal, entriesSuccess, result } = await this.unpackArchiveUnchecked(zip, destination);

            await loader.dismiss();

            let status: string;
            let title: string;
            switch (result) {
                case UnzipResult.success:
                    status = 'The archive was extracted successfully, but some entries were skipped.';
                    title = 'Skipped entries';
                    break;

                case UnzipResult.ioError:
                    await this.fatalError('There was an error writing to the card.');
                    return;

                case UnzipResult.cardFull:
                    status = 'No space left on card.';
                    title = 'No space left';
                    break;

                case UnzipResult.zipfileError:
                    status = 'There was an error reading from the archive.';
                    title = 'Error reading archive';
                    break;

                default:
                    throw new Error('unreachable');
            }

            const entriesFailed = entriesTotal - entriesSuccess;
            const pluralize = (entries: number) => (entries > 1 ? `${entries} entries` : 'one entry');

            if (entriesTotal > 0 && entriesFailed === entriesTotal) {
                await this.alertService.message(
                    title,
                    `
                    ${status}
                    <br><br>
                    The archive contains ${pluralize(entriesTotal)}, but none could be unpacked.
                `
                );
            }

            if (entriesFailed > 0 && entriesSuccess > 0) {
                await this.alertService.message(
                    title,
                    `
                    ${status}
                    <br><br>
                    ${ucFirst(pluralize(entriesSuccess))} unpacked succesfully, but ${pluralize(
                        entriesSuccess
                    )} could not be unpacked.
                `
                );
            }
        } finally {
            void loader.dismiss();
        }
    }

    async addFiles(files: Array<FileDescriptor>, destination: string): Promise<void> {
        if (files.length === 0) return;

        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const failed = await this.addFilesUnchecked(files, destination);

            await loader.dismiss();

            const pluralize = (files: number) => (files > 1 ? `${files} entries` : 'one file');

            if (failed.length === files.length) {
                await this.alertService.message('Skipped files', 'No files were added to the card.');
            } else if (failed.length !== 0) {
                await this.alertService.message(
                    `Skipped files`,
                    `Successfully added ${pluralize(files.length - failed.length)} to card. ${ucFirst(
                        pluralize(failed.length)
                    )} could not be added.
                `
                );
            }
        } finally {
            void loader.dismiss();
        }
    }

    private async addFilesUnchecked(files: Array<FileDescriptor>, destination: string): Promise<Array<string>> {
        const entriesCurrent = this.readdir(destination);
        const failed: Array<string> = [];
        const vfs = await this.vfs;

        let rememberChoice = false;
        let overwrite = false;

        iterate_files: for (let i = 0; i < files.length; i++) {
            const file = files[i];
            const existingEntry = entriesCurrent.find((entry) => entry.name === file.name);

            if (existingEntry && existingEntry.isDirectory) {
                failed.push(file.name);
                continue;
            }

            if (existingEntry) {
                if (!rememberChoice) {
                    const { overwrite: overwriteSelection, rememberChoice: rememberChoiceSelection } =
                        await this.overwriteFileDialog(file.name, 'file');

                    overwrite = overwriteSelection;
                    rememberChoice = rememberChoiceSelection;
                }

                if (!overwrite) {
                    failed.push(file.name);
                    continue;
                }
            }

            let content: Uint8Array;
            try {
                content = await file.getContent();
            } catch (e) {
                console.warn(e);
                failed.push(file.name);

                await this.alertService.errorMessage(`Unable to open ${file.name}`);
                continue;
            }

            switch (vfs.writeFile(this.normalizePath(`${destination}/${file.name}`), content)) {
                case WriteFileResult.errCardFull:
                    await this.alertService.errorMessage('Card full.');
                    failed.push(...files.slice(i).map((file) => file.name));

                    break iterate_files;

                case WriteFileResult.errIO:
                    await this.fatalError('Unable to write file.');
                    throw new Error();

                case WriteFileResult.success:
                    break;

                default:
                    failed.push(file.name);
                    break;
            }
        }

        await this.sync();
        this.directoryCache.delete(this.normalizePath(destination));

        return failed;
    }

    private async unpackArchiveUnchecked(
        zip: Uint8Array,
        destination: string
    ): Promise<{ result: UnzipResult; entriesTotal: number; entriesSuccess: number }> {
        const vfs = await this.vfs;

        let overwriteFile = false;
        let rememberChoiceFiles = false;
        let overwriteDirectory = false;
        let rememberChoiceDirectories = false;

        const result = await vfs.unzipArchive(this.normalizePath(destination), zip, {
            onFileCollision: async (collisionPath: string) => {
                if (rememberChoiceFiles) return overwriteFile;

                const { overwrite, rememberChoice } = await this.overwriteFileDialog(collisionPath, 'file');
                overwriteFile = overwrite;
                rememberChoiceFiles = rememberChoice;

                return overwriteFile;
            },
            onDirectoryCollision: async (collisionPath: string) => {
                if (rememberChoiceDirectories) return overwriteDirectory;

                const { overwrite, rememberChoice } = await this.overwriteFileDialog(collisionPath, 'directory');
                overwriteDirectory = overwrite;
                rememberChoiceDirectories = rememberChoice;

                return overwriteDirectory;
            },
            onInvalidEntry: async (entry: string) => {
                await this.alertService.message('Invalid entry', `Could not extract ${entry}`, {}, 'Continue');
            },
        });

        if (result.result !== UnzipResult.ioError) {
            await this.sync();
            this.directoryCache.delete(this.normalizePath(destination));
        }

        return result;
    }

    private async overwriteFileDialog(
        name: string,
        type: 'file' | 'directory'
    ): Promise<{ overwrite: boolean; rememberChoice: boolean }> {
        let overwrite = false;
        let rememberChoice = false;

        const alert = await this.alertController.create({
            header: 'File collision',
            backdropDismiss: false,
            cssClass: 'alert-checkbox-no-border installation-error',
            message:
                type === 'file'
                    ? `Do you want to overwrite the existing file ${name}?`
                    : `
                    Do you want to overwrite the existing directory ${name}?
                    <br><br>
                    WARNING: All files in this directory will be lost.
                    `,
            buttons: [
                {
                    text: 'No',
                    role: 'cancel',
                },
                {
                    text: 'Yes',
                    handler: () => (overwrite = true),
                },
            ],
            inputs: [
                {
                    type: 'checkbox',
                    label: `Remember for all ${type}s`,
                    checked: false,
                    handler: (inpt) => (rememberChoice = inpt.checked === true),
                    cssClass: 'alert-checkbox',
                },
            ],
        });

        await alert.present();
        await alert.onDidDismiss();

        return { overwrite, rememberChoice };
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
