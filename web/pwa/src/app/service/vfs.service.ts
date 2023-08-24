import { AlertController, LoadingController } from '@ionic/angular';
import { Attributes, FileEntry, ReaddirError, UnzipResult, Vfs, VfsResult, WriteFileResult } from '@common/bridge/Vfs';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { ClipboardOperation, VfsClipboard } from '@pwa/model/VfsClipboard';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { PasteResult } from '@common/bridge/Vfs';
import { StorageCardService } from '@pwa/service/storage-card.service';
import { StorageService } from '@pwa/service/storage.service';
import deepEqual from 'deep-equal';
import { filenameForArchive } from '@pwa/helper/filename';
import { ucFirst } from '@pwa/helper/text';

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(
        private storageService: StorageService,
        private storageCardContext: StorageCardContext,
        private fileService: FileService,
        private loadingController: LoadingController,
        private alertController: AlertController,
        private alertService: AlertService,
    ) {
        void this.vfs.then((instance) => (this.vfsInstance = instance));
    }

    setStorageCardService(storageCardService: StorageCardService): void {
        this.storageCardService = storageCardService;

        storageCardService.mountCardEvent.addHandler(this.invalidateClipboardForCard.bind(this));
        storageCardService.deleteCardEvent.addHandler(this.invalidateClipboardForCard.bind(this));
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

    isEntryInClipboard(entry: FileEntry): ClipboardOperation | undefined {
        if (!this.clipboard || this.clipboard.storageId !== this.primaryCard?.storageId) return undefined;

        return this.clipboardSet.has(entry.path) ? this.clipboard.operation : undefined;
    }

    async activateCard(id: number): Promise<void> {
        if (this.primaryCard?.id === id) return;

        if (this.secondaryCard?.id === id) {
            this.primarySlot = this.secondarySlot;
            await this.activatePrimaryCard();

            return;
        }

        throw new Error(`card ${id} not mounted in VFS`);
    }

    async mountCardUnchecked(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        if (this.primaryCard?.id === id || this.secondaryCard?.id === id) {
            throw new Error(`card ${id} already mounted in VFS`);
        }

        this.storageCardContext.claim(id, CardOwner.vfs);

        if (this.primaryCard && this.primaryCard.storageId === this.clipboard?.storageId) {
            this.primarySlot = this.secondarySlot;
        }

        if (this.primaryCard) await this.releaseCard(this.primaryCard.id);

        const vfs = await this.vfs;

        vfs.allocateImage(card.size);
        const pendingImage = vfs.getPendingImage();
        if (!pendingImage) throw new Error('failed to allocate image');

        if (data) {
            pendingImage.set(data);
        } else {
            await this.storageService.loadCardData(id, pendingImage, CardOwner.vfs);
        }

        if (!vfs.mountImage(this.primarySlot)) {
            this.storageCardContext.release(id, CardOwner.vfs);
            return false;
        }

        this.primaryCard = card;
        await this.activatePrimaryCard();

        return true;
    }

    async releaseCard(id: number): Promise<void> {
        let slot: number;
        if (this.primaryCard?.id === id) slot = this.primarySlot;
        else if (this.secondaryCard?.id === id) slot = this.secondarySlot;
        else return;

        (await this.vfs).unmountImage(slot);
        this.mountedCards[slot] = undefined;

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
                readdirResult.error === ReaddirError.none ? readdirResult.entries : [],
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
        if (entries.length === 0 || !this.primaryCard) return;

        const loader = await this.loadingController.create({ message: 'Creating archive...' });
        await loader.present();

        try {
            const vfs = await this.vfs;
            const directories = entries
                .filter((entry) => entry.isDirectory)
                .map((entry) => this.normalizePath(entry.path));
            const files = entries.filter((entry) => !entry.isDirectory).map((entry) => this.normalizePath(entry.path));
            const archiveName = entries.length === 1 ? `${entries[0].name}.zip` : filenameForArchive(this.primaryCard);

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
                            '<br>',
                        )}`,
                    );
                } else {
                    await this.alertService.message(
                        'Warning',
                        `The following files were not archived correctly due to errors: <br><br>${failedItems
                            .slice(0, 3)
                            .join('<br>')}<br><br> and ${failedItems.length - 3} other file`,
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

        await this.updateBytesFree();
        await this.sync();
        this.directoryCache.delete(this.dirname(entry.path));
    }

    async deleteRecursive(entries: Array<FileEntry>): Promise<void> {
        const files = entries.map((entry) => this.normalizePath(entry.path));

        const loader = await this.loadingController.create({ message: 'Deleting...' });
        await loader.present();

        try {
            const vfs = await this.vfs;
            const result = await vfs.deleteRecursive(files);

            if (!result.success) {
                await loader.dismiss();
                await this.fatalError(`Failed to delete ${result.failingItem}`);

                return;
            }

            await this.updateBytesFree();
            await this.sync();
            entries.map((entry) => entry.path).forEach((path) => this.directoryCache.delete(this.dirname(path)));
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

        await this.updateBytesFree();
        await this.sync();
        this.directoryCache.delete(this.dirname(path));
    }

    getBytesFree(): number {
        return this.bytesFree;
    }

    getBytesTotal(): number {
        return this.bytesTotal;
    }

    isClipboardPopulated(): boolean {
        if (!this.clipboard || !this.storageCardService) return false;

        const card = this.storageCardService.getAllCards().find((c) => c.storageId === this.clipboard?.storageId);
        if (!card) return false;

        return (
            card.status === StorageCardStatus.clean ||
            (card.status === StorageCardStatus.dirty && !!card.dontFsckAutomatically)
        );
    }

    invalidateClipboardForCard(card: StorageCard): void {
        if (this.clipboard?.storageId === card.storageId) {
            this.clipboard = undefined;
            this.clipboardSet.clear();
        }
    }

    copyToClipboard(prefix: string, items: Array<string>, operation: ClipboardOperation): void {
        if (!this.primaryCard) return;

        if (this.clipboard) this.directoryCache.delete(this.normalizePath(this.clipboard.prefix));

        this.clipboard = {
            operation,
            prefix: this.normalizePath(prefix),
            storageId: this.primaryCard.storageId,
            items: items.map((item) => ({
                name: item,
                path: this.normalizePath(`${prefix}/${item}`),
            })),
        };

        this.clipboardSet.clear();
        this.clipboard.items.map((item) => item.path).forEach((item) => this.clipboardSet.add(item));
        this.directoryCache.delete(this.normalizePath(this.clipboard.prefix));
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
                `,
                );
            }

            if (entriesFailed > 0 && entriesSuccess > 0) {
                await this.alertService.message(
                    title,
                    `
                    ${status}
                    <br><br>
                    ${ucFirst(pluralize(entriesSuccess))} unpacked succesfully, but ${pluralize(
                        entriesFailed,
                    )} could not be unpacked.
                `,
                );
            }
        } finally {
            void loader.dismiss();
        }
    }

    async paste(destination: string): Promise<void> {
        if (!this.isClipboardPopulated()) return;

        const loader = await this.loadingController.create();
        await loader.present();
        try {
            const result = await this.pasteUnchecked(destination);

            await loader.dismiss();

            switch (result) {
                case PasteResult.cardFull:
                    await this.alertService.message('No space left', 'No space left on card.');
                    break;

                case PasteResult.ioError:
                    await this.fatalError('There was an error copying files to the card.');
                    return;

                default:
                    break;
            }
        } catch (e: unknown) {
            await this.alertService.errorMessage(e instanceof Error ? e.message : 'Unknown error.');
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
                        pluralize(failed.length),
                    )} could not be added.
                `,
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

        let rememberChoiceFiles = false;
        let overwriteFiles = false;
        let rememberChoiceDirectories = false;
        let overwriteDirectories = false;

        iterate_files: for (let i = 0; i < files.length; i++) {
            const file = files[i];
            const existingEntry = entriesCurrent.find((entry) => entry.name === file.name);

            if (existingEntry && existingEntry.isDirectory) {
                if (!rememberChoiceDirectories) {
                    const { overwrite: overwriteSelection, rememberChoice: rememberChoiceSelection } =
                        await this.overwriteFileDialog(file.name, 'directory');

                    overwriteDirectories = overwriteSelection;
                    rememberChoiceDirectories = rememberChoiceSelection;
                }

                let skip = true;

                if (overwriteDirectories) {
                    const deleteRecursiveResult = await vfs.deleteRecursive([
                        this.normalizePath(`${destination}/${file.name}`),
                    ]);

                    skip = !deleteRecursiveResult.success;
                }

                if (skip) {
                    failed.push(file.name);
                    continue;
                }
            }

            if (existingEntry && !existingEntry.isDirectory) {
                if (!rememberChoiceFiles) {
                    const { overwrite: overwriteSelection, rememberChoice: rememberChoiceSelection } =
                        await this.overwriteFileDialog(file.name, 'file');

                    overwriteFiles = overwriteSelection;
                    rememberChoiceFiles = rememberChoiceSelection;
                }

                if (!overwriteFiles) {
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

        await this.updateBytesFree();
        await this.sync();
        this.directoryCache.delete(this.normalizePath(destination));

        return failed;
    }

    private collisionHandlers(): {
        onFileCollision: (collisionPath: string, entry: string) => Promise<boolean>;
        onDirectoryCollision: (collisionPath: string, entry: string) => Promise<boolean>;
    } {
        let overwriteFiles = false;
        let rememberChoiceFiles = false;
        let overwriteDirectory = false;
        let rememberChoiceDirectories = false;

        return {
            onFileCollision: async (collisionPath: string) => {
                if (rememberChoiceFiles) return overwriteFiles;

                const { overwrite, rememberChoice } = await this.overwriteFileDialog(collisionPath, 'file');
                overwriteFiles = overwrite;
                rememberChoiceFiles = rememberChoice;

                return overwriteFiles;
            },
            onDirectoryCollision: async (collisionPath: string) => {
                if (rememberChoiceDirectories) return overwriteDirectory;

                const { overwrite, rememberChoice } = await this.overwriteFileDialog(collisionPath, 'directory');
                overwriteDirectory = overwrite;
                rememberChoiceDirectories = rememberChoice;

                return overwriteDirectory;
            },
        };
    }

    private async unpackArchiveUnchecked(
        zip: Uint8Array,
        destination: string,
    ): Promise<{ result: UnzipResult; entriesTotal: number; entriesSuccess: number }> {
        const vfs = await this.vfs;

        const result = await vfs.unzipArchive(this.normalizePath(destination), zip, {
            ...this.collisionHandlers(),
            onInvalidEntry: async (entry: string) => {
                await this.alertService.message('Invalid entry', `Could not extract ${entry}`, {}, 'Continue');
            },
        });

        if (result.result !== UnzipResult.ioError) {
            await this.updateBytesFree();
            await this.sync();
            this.directoryCache.clear();
        }

        return result;
    }

    private async pasteUnchecked(destination: string): Promise<PasteResult> {
        if (!this.clipboard || !this.primaryCard) {
            throw new Error('invalid state for paste');
        }

        const sourceSlot = await this.mountClipboard();

        const vfs = await this.vfs;
        const result = await vfs.paste(
            `${this.primarySlot}:${this.normalizePath(destination)}`,
            `${sourceSlot}:${this.normalizePath(this.clipboard.prefix)}`,
            this.clipboard.items.map((item) => item.name),
            this.clipboard.operation,
            this.collisionHandlers(),
        );

        if (result === PasteResult.ioError) return result;

        await this.updateBytesFree();
        await this.sync(this.primarySlot);
        if (sourceSlot !== this.primarySlot) await this.sync(sourceSlot);

        this.directoryCache.clear();

        if (this.clipboard.operation === 'cut') {
            this.clipboard = undefined;
            this.clipboardSet.clear();
        }

        return result;
    }

    private async mountClipboard(): Promise<number> {
        if (!this.clipboard || !this.primaryCard || !this.storageCardService) {
            throw new Error('invalid state for paste');
        }

        if (this.clipboard.storageId === this.primaryCard.storageId) return this.primarySlot;
        if (this.secondaryCard && this.clipboard.storageId === this.secondaryCard.storageId) return this.secondarySlot;
        if (this.secondaryCard) await this.releaseCard(this.secondaryCard.id);

        const card = this.storageCardService.getAllCards().find((card) => card.storageId === this.clipboard?.storageId);

        if (!card) {
            throw new Error('invalid storage card in clipboard');
        }

        const vfs = await this.vfs;

        vfs.allocateImage(card.size);
        const pendingImage = vfs.getPendingImage();
        if (!pendingImage) throw new Error('failed to allocate image');

        this.storageCardContext.claim(card.id, CardOwner.vfs);

        await this.storageService.loadCardData(card.id, pendingImage, CardOwner.vfs);
        if (!vfs.mountImage(this.secondarySlot)) {
            this.storageCardContext.release(card.id, CardOwner.vfs);
            throw new Error('unable to mount card');
        }

        this.secondaryCard = card;

        return this.secondarySlot;
    }

    private async overwriteFileDialog(
        name: string,
        type: 'file' | 'directory',
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
                    label: `Remember choice`,
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
        if (!this.primaryCard) return;

        await this.storageService.updateStorageCardPartial(this.primaryCard.id, { status: StorageCardStatus.dirty });
        await this.releaseCard(this.primaryCard.id);

        void this.alertService.errorMessage(`${message}
        <br><br>
        No changes have been written, and the card has been unmounted to prevent damage to the file system.
        `);
    }

    private async sync(slot = this.primarySlot): Promise<void> {
        const card = this.mountedCards[slot];
        if (!card) return;
        const vfs = await this.vfs;

        const data = vfs.getImageInSlot(slot);
        const dirtyPages = vfs.getDirtyPagesForSlot(slot);

        if (!data || !dirtyPages) return;

        await this.storageService.updateCardData(card.id, data, dirtyPages, CardOwner.vfs);

        dirtyPages.fill(0);
    }

    private async activatePrimaryCard(): Promise<void> {
        const vfs = await this.vfs;

        vfs.switchSlot(this.primarySlot);
        this.bytesFree = vfs.bytesFree(this.primarySlot);
        this.bytesTotal = vfs.bytesTotal(this.primarySlot);
        this.directoryCache.clear();
    }

    private async updateBytesFree(): Promise<void> {
        this.bytesFree = (await this.vfs).bytesFree(this.primarySlot);
    }

    private get secondarySlot(): number {
        return 1 - this.primarySlot;
    }

    private get primaryCard(): StorageCard | undefined {
        return this.mountedCards[this.primarySlot];
    }

    private set primaryCard(card: StorageCard | undefined) {
        this.mountedCards[this.primarySlot] = card;
    }

    private get secondaryCard(): StorageCard | undefined {
        return this.mountedCards[this.secondarySlot];
    }

    private set secondaryCard(card: StorageCard | undefined) {
        this.mountedCards[this.secondarySlot] = card;
    }

    readonly onReleaseCard = new Event<number>();

    private storageCardService: StorageCardService | undefined;

    private mountedCards = new Array<StorageCard | undefined>(2);

    private primarySlot = 0;

    private vfs = Vfs.create();
    private vfsInstance: Vfs | undefined;

    private directoryCache = new Map<string, Array<FileEntry>>();
    private bytesFree = 0;
    private bytesTotal = 0;

    private clipboard: VfsClipboard | undefined = undefined;
    private clipboardSet = new Set<string>();
}
