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
        const cardWithoutId: Omit<StorageCard, 'id'> = {
            storageId: newStorageId(),
            name,
            size: calculateNewCardSizeBytes(size),
            status: StorageCardStatus.clean,
        };

        const card = await this.storageService.addEmptyStorageCard(cardWithoutId);
        this.updateCardsFromDB();

        return card;
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
