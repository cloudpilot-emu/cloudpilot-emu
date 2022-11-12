import { AlertController, ModalController } from '@ionic/angular';

import { Component } from '@angular/core';
import { EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
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
    constructor(
        public storageCardService: StorageCardService,
        private modalController: ModalController,
        private alertController: AlertController
    ) {}

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards();
    }

    importCardImage(): void {}

    async createNewCardImage(): Promise<void> {
        const modal = await this.modalController.create({
            component: NewCardDialogComponent,
            backdropDismiss: false,
            componentProps: {
                onCancel: () => modal.dismiss(),
                onCreate: (name: string, size: NewCardSize) => {
                    this.storageCardService.createEmptyCard(name, size);
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

    async editCard(card: StorageCard) {
        const modal = await this.modalController.create({
            component: EditCardDialogComponent,
            backdropDismiss: false,
            componentProps: {
                card,
                onCancel: () => modal.dismiss(),
                onSave: (updatedCard: StorageCard) => {
                    this.storageCardService.updateCard(updatedCard);
                    modal.dismiss();
                },
            },
        });

        await modal.present();
    }

    checkCard(card: StorageCard) {
        console.log(`check ${card.name}`);
    }

    ejectCard(card: StorageCard) {
        console.log(`eject ${card.name}`);
    }

    async deleteCard(card: StorageCard) {
        const alert = await this.alertController.create({
            header: 'Warning',
            message: `Deleting the card '${card.name}' cannot be undone. Are you sure you want to continue?`,
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                { text: 'Delete', handler: () => this.storageCardService.deleteCard(card) },
            ],
        });

        await alert.present();
    }

    trackCardBy(index: number, card: StorageCard) {
        return card.id;
    }

    public lastCardTouched: number | undefined = undefined;
}
