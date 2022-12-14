import { FsckContext, FsckResult, mkfs } from '@common/FSTools';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationStateService } from './emulation-state.service';
import { FileDescriptor } from '@pwa/service/file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';
import { SnapshotService } from '@pwa/service/snapshot.service';
import { StorageService } from '@pwa/service/storage.service';
import { v4 as uuid } from 'uuid';

export enum NewCardSize {
    mb4,
    mb8,
    mb16,
    mb32,
    mb64,
    mb128,
}

export type FsckStatusClean = {
    result: FsckResult.ok;
};

export type FsckStatusFixed = {
    result: FsckResult.fixed;

    fixedImage: Uint32Array;
    dirtyPages: Uint8Array;
};

export type FsckStatusBad = {
    result: FsckResult.invalid;
};

export type FsckStatus = FsckStatusClean | FsckStatusFixed | FsckStatusBad;

export function cardSizeNumeric(newCardSize: NewCardSize): number {
    switch (newCardSize) {
        case NewCardSize.mb4:
            return 4;

        case NewCardSize.mb8:
            return 8;

        case NewCardSize.mb16:
            return 16;

        case NewCardSize.mb32:
            return 32;

        case NewCardSize.mb64:
            return 64;

        case NewCardSize.mb128:
            return 128;

        default:
            throw new Error(`bad card size ${newCardSize}`);
    }
}

export function calculateNewCardSizeBytes(newCardSize: NewCardSize): number {
    switch (newCardSize) {
        case NewCardSize.mb4:
            return 494 * 16 * 512;

        case NewCardSize.mb8:
            return (2 * 496 - 2) * 16 * 512;

        case NewCardSize.mb16:
            return (2 * 496 - 2) * 32 * 512;

        case NewCardSize.mb32:
            return (4 * 496 - 2) * 32 * 512;

        case NewCardSize.mb64:
            return (8 * 496 - 2) * 32 * 512;

        case NewCardSize.mb128:
            return (16 * 496 - 2) * 32 * 512;

        default:
            throw new Error(`bad card size ${newCardSize}`);
    }
}

function newStorageId(): string {
    return uuid().replace(/-/g, '');
}

@Injectable({ providedIn: 'root' })
export class StorageCardService {
    constructor(
        private sessionService: SessionService,
        private storageService: StorageService,
        private emulationStateService: EmulationStateService,
        private cloudpilotService: CloudpilotService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService
    ) {
        this.updateCardsFromDB().then(() => (this.loading = false));

        storageService.storageCardChangeEvent.addHandler(() => this.updateCardsFromDB());
    }

    getAllCards(): Array<StorageCard> {
        return this.cards;
    }

    async createEmptyCard(name: string, size: NewCardSize): Promise<StorageCard> {
        const loader = await this.loadingController.create({ message: 'Formatting...' });

        try {
            await loader.present();

            const cardImage = await mkfs(cardSizeNumeric(size));
            if (!cardImage) throw new Error('failed to create image');

            const cardWithoutId: Omit<StorageCard, 'id'> = {
                storageId: newStorageId(),
                name,
                size: cardImage.length * 4,
                status: StorageCardStatus.clean,
            };

            const card = await this.storageService.importStorageCard(cardWithoutId, cardImage);
            this.updateCardsFromDB();

            return card;
        } finally {
            loader.dismiss();
        }
    }

    async createCardFromFile(name: string, file: FileDescriptor): Promise<StorageCard> {
        const cardWithoutId: Omit<StorageCard, 'id'> = {
            storageId: newStorageId(),
            name,
            size: file.content.length,
            status: StorageCardStatus.clean,
        };

        const loader = await this.loadingController.create({ message: 'Importing...' });

        try {
            await loader.present();

            const card = await this.storageService.importStorageCard(
                cardWithoutId,
                new Uint32Array(file.content.buffer, file.content.byteOffset, file.content.length >>> 2)
            );

            this.updateCardsFromDB();

            return card;
        } finally {
            loader.dismiss();
        }
    }

    updateCard(card: StorageCard): Promise<void> {
        return this.storageService.updateStorageCard(card);
    }

    async deleteCard(card: StorageCard): Promise<void> {
        if (await this.cardIsMounted(card.id)) await this.ejectCard();

        await this.storageService.deleteStorageCard(card.id);

        this.updateCardsFromDB();
    }

    mountedInSession(cardId: number): Session | undefined {
        return this.sessionService.getSessions().find((session) => session.mountedCard === cardId);
    }

    isLoading(): boolean {
        return this.loading;
    }

    async mountCard(cardId: number): Promise<StorageCard> {
        const loader = await this.loadingController.create({ message: 'Inserting...' });

        try {
            await loader.present();

            if (await this.cardIsMounted(cardId)) throw new Error('card already mounted');

            const session = this.emulationStateService.getCurrentSession();
            if (!session) throw new Error('no current session');
            if (session.mountedCard !== undefined) throw new Error('session already has a mounted card');

            const card = await this.loadCard(cardId);
            const cloudpilot = await this.cloudpilotService.cloudpilot;

            this.storageService.updateSessionPartial(session.id, { mountedCard: cardId });

            if (!cloudpilot.mountCard(card.storageId)) {
                cloudpilot.removeCard(card.storageId);

                throw new Error('failed to mount card');
            }

            return card;
        } finally {
            loader.dismiss();
        }
    }

    async loadCard(cardId: number): Promise<StorageCard> {
        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        const cloudpilot = await this.cloudpilotService.cloudpilot;
        if (cloudpilot.getMountedKey()) {
            throw new Error('attempt to mount a card while another card is mounted');
        }

        if (!cloudpilot.allocateCard(card.storageId, card.size)) {
            throw new Error('failed to allocate card');
        }

        const cardData = cloudpilot.getCardData(card.storageId);
        if (!cardData) {
            throw new Error('failed to access card data');
        }

        await this.storageService.loadCardData(card.id, cardData);

        return card;
    }

    async ejectCard(): Promise<void> {
        const session = this.emulationStateService.getCurrentSession();
        if (!session) throw new Error('no running session');

        const cardId = session?.mountedCard;
        if (cardId === undefined) {
            throw new Error(`no mounted card`);
        }

        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        await this.snapshotService.waitForPendingSnapshot();
        await this.snapshotService.triggerSnapshot();

        await this.storageService.updateSessionPartial(session.id, { mountedCard: undefined });

        const cloudpilot = await this.cloudpilotService.cloudpilot;

        cloudpilot.removeCard(card.storageId);

        this.snapshotService.resetCard();
    }

    async fsckCard(cardId: number): Promise<FsckStatus> {
        const loader = await this.loadingController.create({ message: 'Checking card...' });

        try {
            await loader.present();

            if (await this.cardIsMounted(cardId)) throw new Error('attempt to fsck a mounted card');

            const card = await this.storageService.getCard(cardId);
            if (!card) {
                throw new Error(`no card with id ${cardId}`);
            }

            const context = await FsckContext.create(card.size);
            await this.storageService.loadCardData(card.id, context.getImageData());

            const result = context.fsck();
            switch (result) {
                case FsckResult.ok:
                case FsckResult.invalid:
                    return { result };

                case FsckResult.fixed:
                    return { result, fixedImage: context.getImageData(), dirtyPages: context.getDirtyPages() };

                default:
                    throw new Error('unrachable: invalid fsck result');
            }
        } finally {
            loader.dismiss();
        }
    }

    async applyFsckResult(cardId: number, result: FsckStatusFixed): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Fixing errors...' });

        try {
            await loader.present();
            await this.storageService.updateCardData(cardId, result.fixedImage, result.dirtyPages);
        } finally {
            loader.dismiss();
        }
    }

    private async updateCardsFromDB(): Promise<void> {
        this.cards = await this.updateMutex.runExclusive(async () =>
            (await this.storageService.getAllStorageCards()).sort((x, y) => x.name.localeCompare(y.name))
        );
    }

    private async cardIsMounted(id: number): Promise<boolean> {
        const sessions = await this.storageService.getAllSessions();

        return sessions.some((session) => session.mountedCard === id);
    }

    private loading = true;

    private cards: Array<StorageCard> = [];
    private updateMutex = new Mutex();
}
