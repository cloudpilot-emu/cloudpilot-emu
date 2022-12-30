import { CardOwner, StorageCardContext } from './storage-card-context';
import { FileEntry, ReaddirError, Vfs } from '@common/bridge/Vfs';

import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageService } from '@pwa/service/storage.service';

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(private storageService: StorageService, private storageCardContext: StorageCardContext) {}

    normalizePath(path: string): string {
        return ('/' + path).replace(/\/{2,}/g, '/').replace(/\/*$/, '');
    }

    splitPath(path: string): Array<string> {
        return this.normalizePath(path).replace(/^\/+/, '').split('/');
    }

    async mountCardUnchecked(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        this.storageCardContext.claim(id, CardOwner.vfs);

        this.directoryCache.clear();

        if (this.mountedCard?.id === id) return true;
        if (this.mountedCard) await this.releaseCard(this.mountedCard.id);

        const vfs = await this.vfs;

        vfs.AllocateImage(card.size);
        const pendingImage = vfs.GetPendingImage();
        if (!pendingImage) throw new Error('failed to allocate image');

        if (data) {
            pendingImage.set(data);
        } else {
            await this.storageService.loadCardData(id, pendingImage, CardOwner.vfs);
        }

        if (vfs.MountImage(0)) {
            this.mountedCard = card;
            return true;
        }

        return false;
    }

    async releaseCard(id = this.mountedCard?.id) {
        if (id === undefined || id !== this.mountedCard?.id) return;

        (await this.vfs).UnmountImage(0);
        this.mountedCard = undefined;

        this.storageCardContext.release(id, CardOwner.vfs);

        this.onReleaseCard.dispatch(id);
    }

    async readdir(path: string): Promise<Array<FileEntry> | undefined> {
        const normalizedPath = this.normalizePath(path);

        if (!this.directoryCache.has(normalizedPath)) {
            const readdirResult = (await this.vfs).Readdir(path);
            if (readdirResult.error === ReaddirError.none) {
                this.directoryCache.set(normalizedPath, readdirResult.entries);
            }
        }

        return this.directoryCache.get(normalizedPath);
    }

    currentCard(): StorageCard | undefined {
        return this.mountedCard;
    }

    readonly onReleaseCard = new Event<number>();

    private mountedCard: StorageCard | undefined;
    private vfs = Vfs.create();

    private directoryCache = new Map<string, Array<FileEntry>>();
}
