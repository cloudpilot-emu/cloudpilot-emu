import { Component } from '@angular/core';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageCardService } from './../../service/storage-card.service';

@Component({
    selector: 'app-storage',
    templateUrl: './storage.page.html',
    styleUrls: ['./storage.page.scss'],
})
export class StoragePage {
    constructor(public storageCardService: StorageCardService) {}

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards();
    }

    importCardImage(): void {}

    createNewCardImage(): void {}

    showHelp(): void {}

    selectCard(card: StorageCard) {
        console.log(`select ${card.name}`);
    }

    editCard(card: StorageCard) {
        console.log(`edit ${card.name}`);
    }

    checkCard(card: StorageCard) {
        console.log(`check ${card.name}`);
    }

    ejectCard(card: StorageCard) {
        console.log(`eject ${card.name}`);
    }

    deleteCard(card: StorageCard) {
        console.log(`delete ${card.name}`);
    }

    trackCardBy(index: number, card: StorageCard) {
        return card.id;
    }

    public lastCardTouched: number | undefined = undefined;
}
