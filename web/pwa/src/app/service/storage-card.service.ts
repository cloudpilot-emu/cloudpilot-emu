import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { Injectable } from '@angular/core';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';

let NEXT_ID = 0;

@Injectable({ providedIn: 'root' })
export class StorageCardService {
    constructor(private sessionService: SessionService) {
        setTimeout(() => (this.loading = false), 500);
    }

    getAllCards(): Array<StorageCard> {
        return this.cards;
    }

    async addNewCard(card: Omit<StorageCard, 'id'>): Promise<StorageCard> {
        const newCard = { ...card, id: NEXT_ID++ };
        this.cards.push(newCard);

        return newCard;
    }

    async updateCard(updatedCard: StorageCard): Promise<void> {
        this.cards = this.cards.map((card) => (card.id === updatedCard.id ? { ...updatedCard } : card));
    }

    async deleteCard(cardForDeletion: StorageCard): Promise<void> {
        this.cards = this.cards.filter((card) => card.id !== cardForDeletion.id);
    }

    mountedInSession(cardId: number): Session | undefined {
        if (cardId === 0) return this.sessionService.getSessions()[0];

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
            storageId: '55047f6788b64d9aad5a344e77ecf060',
            size: 32 * 1024 * 1024,
            name: 'Lustige Karte 3',
            status: StorageCardStatus.unformatted,
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
