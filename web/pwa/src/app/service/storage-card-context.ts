import { Injectable } from '@angular/core';

import { ErrorService } from '@pwa/service/error.service';

export const enum CardOwner {
    cloudpilot = 'cloudpilot',
    fstools = 'fstools',
    vfs = 'vfs',
    none = 'none',
}

@Injectable({ providedIn: 'root' })
export class StorageCardContext {
    constructor(private errorService: ErrorService) {}

    getOwner(cardId: number): CardOwner {
        return this.ownershipMmap.get(cardId) ?? CardOwner.none;
    }

    claim(cardId: number, owner: CardOwner): void {
        this.assertOwnership(cardId, CardOwner.none);
        this.ownershipMmap.set(cardId, owner);
    }

    release(cardId: number, currentOwner: CardOwner) {
        this.assertOwnership(cardId, currentOwner);
        this.ownershipMmap.delete(cardId);
    }

    assertOwnership(cardId: number, owner: CardOwner): void {
        if (this.getOwner(cardId) === owner) return;

        const message =
            owner === CardOwner.none
                ? `Card already claimed for ${this.getOwner(cardId)}.`
                : `Expected card ownership to be ${owner}, but is ${this.getOwner(cardId)}.`;

        this.errorService.fatalBug(message);
        throw new Error(message);
    }

    transferOwnership(cardId: number, from: CardOwner, to: CardOwner): void {
        this.assertOwnership(cardId, from);
        this.ownershipMmap.set(cardId, to);
    }

    private ownershipMmap = new Map<number, CardOwner>();
}
