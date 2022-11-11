import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { Injectable } from '@angular/core';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';
import { v4 as uuid } from 'uuid';

export enum NewCardSize {
    mb4,
    mb8,
    mb16,
    mb32,
    mb64,
    mb128,
}

let NEXT_ID = 0;

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

@Injectable({ providedIn: 'root' })
export class StorageCardService {
    constructor(private sessionService: SessionService) {
        setTimeout(() => (this.loading = false), 500);
    }

    getAllCards(): Array<StorageCard> {
        return this.cards;
    }

    async createNewCard(name: string, size: NewCardSize): Promise<StorageCard> {
        const card: StorageCard = {
            id: NEXT_ID++,
            storageId: uuid().replace(/-/g, ''),
            name,
            size: calculateNewCardSizeBytes(size),
            status: StorageCardStatus.dirty,
        };

        this.cards.push(card);

        return card;
    }

    async updateCard(updatedCard: StorageCard): Promise<void> {
        this.cards = this.cards.map((card) => (card.id === updatedCard.id ? { ...updatedCard } : card));
    }

    async deleteCard(cardForDeletion: StorageCard): Promise<void> {
        this.cards = this.cards.filter((card) => card.id !== cardForDeletion.id);
    }

    mountedInSession(cardId: number): Session | undefined {
        return this.sessionService.getSessions().find((session) => session.mountedCard === cardId);
    }

    isLoading(): boolean {
        return this.loading;
    }

    private loading = true;

    private cards: Array<StorageCard> = [
        {
            id: NEXT_ID++,
            storageId: '365350027f8f4d9a924a0c19c74aa094',
            size: 128 * 1024 * 1024,
            name: 'Lustige Karte 1',
            status: StorageCardStatus.clean,
        },
        {
            id: NEXT_ID++,
            storageId: '55047f6788b64d9aad5a344e77ecf060',
            size: 64 * 1024 * 1024,
            name: 'Lustige Karte 2',
            status: StorageCardStatus.dirty,
        },
        {
            id: NEXT_ID++,
            storageId: '2e5fbf116a9247d68b6299221084789e',
            size: 16 * 1024 * 1024,
            name: 'Lustige Karte 4',
            status: StorageCardStatus.clean,
        },
    ];
}
