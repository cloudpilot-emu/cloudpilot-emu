import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { Injectable } from '@angular/core';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';
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
        private errorService: ErrorService
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
        this.cards.push(card);

        return card;
    }

    updateCard(card: StorageCard): Promise<void> {
        return this.storageService.updateStorageCard(card);
    }

    async deleteCard(card: StorageCard): Promise<void> {
        if (this.emulationStateService.getMountedCard() === card.id) await this.ejectCard();

        await this.storageService.deleteStorageCard(card.id);

        this.updateCardsFromDB();
    }

    mountedInSession(cardId: number): Session | undefined {
        return this.sessionService.getSessions().find((session) => session.mountedCard === cardId);
    }

    isLoading(): boolean {
        return this.loading;
    }

    async mountCard(cardId: number): Promise<void> {
        const card = await this.storageService.getCard(cardId);
        if (!card) {
            return this.errorService.fatalBug(`no card with id ${cardId}`);
        }

        const cloudpilot = await this.cloudpilotService.cloudpilot;
        if (!cloudpilot.allocateCard(card.storageId, card.size)) {
            return this.errorService.fatalBug('failed to allocate card');
        }

        const cardData = cloudpilot.getCardData(card.storageId);
        if (!cardData) {
            return this.errorService.fatalBug('failed to access card data');
        }

        await this.storageService.loadCardData(card.id, cardData);

        if (!cloudpilot.mountCard(card.storageId)) {
            return this.errorService.fatalBug('failed to mount card');
        }

        this.emulationStateService.setMountedCard(cardId);
    }

    async ejectCard(): Promise<void> {
        const cardId = this.emulationStateService.getMountedCard();
        if (cardId === undefined) {
            return this.errorService.fatalBug(`no mounted card`);
        }

        const card = await this.storageService.getCard(cardId);
        if (!card) {
            return this.errorService.fatalBug(`no card with id ${cardId}`);
        }

        const cloudpilot = await this.cloudpilotService.cloudpilot;

        cloudpilot.removeCard(card.storageId);
        this.emulationStateService.clearMountedCard();
    }

    private async updateCardsFromDB(): Promise<void> {
        this.cards = (await this.storageService.getAllStorageCards()).sort((x, y) => x.name.localeCompare(y.name));
    }

    private loading = true;

    private cards: Array<StorageCard> = [];
}
