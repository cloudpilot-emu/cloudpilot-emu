import { Attributes, FileEntry, ReaddirError, Vfs, VfsResult } from '@common/bridge/Vfs';
import { CardOwner, StorageCardContext } from './storage-card-context';

import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageService } from '@pwa/service/storage.service';
import deepEqual from 'deep-equal';

export enum VfsError {
    none,
    read,
    write,
}

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(
        private storageService: StorageService,
        private storageCardContext: StorageCardContext,
        private alertService: AlertService
    ) {}

    normalizePath(path: string): string {
        return ('/' + path).replace(/\/{2,}/g, '/').replace(/\/*$/, '');
    }

    splitPath(path: string): Array<string> {
        return this.normalizePath(path).replace(/^\/+/, '').split('/');
    }

    dirname(path: string): string {
        return this.normalizePath(path).replace(/\/[^\/]*$/, '');
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

    async readdir(path: string): Promise<Array<FileEntry> | undefined> {
        const normalizedPath = this.normalizePath(path);

        if (!this.directoryCache.has(normalizedPath)) {
            const readdirResult = (await this.vfs).readdir(path);
            if (readdirResult.error === ReaddirError.none) {
                this.directoryCache.set(normalizedPath, readdirResult.entries);
            }
        }

        return this.directoryCache.get(normalizedPath);
    }

    async updateFileEntry(
        path: string,
        changes: { name?: string; attributes: Partial<Attributes> }
    ): Promise<VfsError> {
        const vfs = await this.vfs;
        path = this.normalizePath(path);

        const { result, entry } = vfs.stat(path);
        if (result !== VfsResult.FR_OK) {
            await this.alertService.errorMessage(`Unable to stat ${path}.`);
            return VfsError.read;
        }

        this.directoryCache.delete(this.dirname(path));

        if (changes.attributes !== undefined && !deepEqual(changes.attributes, entry.attributes)) {
            switch (vfs.chmod(path, changes.attributes)) {
                case VfsResult.FR_OK:
                    break;

                default:
                    await this.alertService.errorMessage(`Failed to change file attributes.`);
                    return VfsError.write;
            }
        }

        if (changes.name !== undefined && changes.name !== entry.name) {
            if (changes.name.indexOf('/') >= 0) {
                await this.alertService.errorMessage('Invalid file name.');
                throw new Error('bad file name');
            }

            switch (vfs.rename(path, `${this.dirname(path)}/${changes.name}`)) {
                case VfsResult.FR_OK:
                    break;

                case VfsResult.FR_INVALID_NAME:
                    await this.alertService.errorMessage('Invalid file name.');
                    return VfsError.read;

                default:
                    await this.alertService.errorMessage('Failed to rename file.');
                    return VfsError.write;
            }
        }

        await this.sync();
        return VfsError.none;
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

    private directoryCache = new Map<string, Array<FileEntry>>();
    private bytesFree = 0;
    private bytesTotal = 0;
}
