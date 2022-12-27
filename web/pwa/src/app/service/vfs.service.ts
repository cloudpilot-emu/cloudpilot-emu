import { FileEntry, ReaddirError, Vfs } from '@common/bridge/Vfs';

import { Injectable } from '@angular/core';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageService } from '@pwa/service/storage.service';

@Injectable({ providedIn: 'root' })
export class VfsService {
    constructor(private storageService: StorageService) {}

    normalizePath(path: string): string {
        return path.replace(/\/*$/, '');
    }

    async mountCardUnchecked(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        if (this.mountedCard) this.releaseCard(this.mountedCard.id);

        const vfs = await this.vfs;

        vfs.AllocateImage(card.size);
        const pendingImage = vfs.GetPendingImage();
        if (!pendingImage) throw new Error('failed to allocate image');

        if (data) {
            pendingImage.set(data);
        } else {
            await this.storageService.loadCardData(id, pendingImage);
        }

        if (vfs.MountImage(0)) {
            this.mountedCard = card;
            return true;
        }

        return false;
    }

    async releaseCard(id = this.mountedCard?.id) {
        if (id !== this.mountedCard?.id) return;

        (await this.vfs).UnmountImage(0);
        this.mountedCard = undefined;
        this.cachedDirectoryPath = undefined;
        this.cachedDirectoryContent = undefined;
    }

    async readdir(path: string): Promise<Array<FileEntry> | undefined> {
        const normalizedPath = this.normalizePath(path);
        if (normalizedPath === this.cachedDirectoryPath) return this.cachedDirectoryContent;

        const readdirResult = (await this.vfs).Readdir(path);
        if (readdirResult.error !== ReaddirError.none) return undefined;

        this.cachedDirectoryPath = this.normalizePath(path);
        this.cachedDirectoryContent = readdirResult.entries;

        return this.cachedDirectoryContent;
    }

    currentCard(): StorageCard | undefined {
        return this.mountedCard;
    }

    private mountedCard: StorageCard | undefined;
    private vfs = Vfs.create();

    private cachedDirectoryPath: string | undefined;
    private cachedDirectoryContent: Array<FileEntry> | undefined;
}
