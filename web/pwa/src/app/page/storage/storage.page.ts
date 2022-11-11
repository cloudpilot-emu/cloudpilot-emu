import { Component } from '@angular/core';
import { ModalController } from '@ionic/angular';
import { NewCardDialogComponent } from './new-card-dialog/new-card-dialog.component';
import { NewCardSize } from '@pwa/service/storage-card.service';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageCardService } from './../../service/storage-card.service';

@Component({
    selector: 'app-storage',
    templateUrl: './storage.page.html',
    styleUrls: ['./storage.page.scss'],
})
export class StoragePage {
    constructor(public storageCardService: StorageCardService, private modalController: ModalController) {}

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards().sort((x, y) => x.name.localeCompare(y.name));
    }

    importCardImage(): void {}

    async createNewCardImage(): Promise<void> {
        const modal = await this.modalController.create({
            component: NewCardDialogComponent,
            backdropDismiss: false,
            componentProps: {
                onCancel: () => modal.dismiss(),
                onCreate: (name: string, size: NewCardSize) => {
                    this.storageCardService.createNewCard(name, size);
                    modal.dismiss();
                },
            },
        });

        await modal.present();
    }

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
