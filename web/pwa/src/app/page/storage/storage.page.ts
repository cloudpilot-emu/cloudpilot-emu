import { AlertController, ModalController } from '@ionic/angular';
import { CardSettings, EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
import { FileDescriptor, FileService } from '@pwa/service/file.service';

import { AlertService } from '@pwa/service/alert.service';
import { CardSupportLevel } from '@native/cloudpilot_web';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { Component } from '@angular/core';
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
        private alertController: AlertController,
        private fileService: FileService,
        private cloudpilotService: CloudpilotService,
        private alertService: AlertService
    ) {}

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards();
    }

    importCardImage(): void {
        this.fileService.openFile(this.handleCardImport);
    }

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
        const cloudpilot = await this.cloudpilotService.cloudpilot;

        const modal = await this.modalController.create({
            component: EditCardDialogComponent,
            backdropDismiss: false,
            componentProps: {
                card,
                cardSupportLevel: cloudpilot.getCardSupportLevel(card.size),
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

    private handleCardImport = async (file: FileDescriptor): Promise<void> => {
        const cloudpilot = await this.cloudpilotService.cloudpilot;
        const supportLevel = cloudpilot.getCardSupportLevel(file.content.length);

        if (supportLevel === CardSupportLevel.unsupported) {
            this.alertService.errorMessage('This is not a supported card image.');
            return;
        }

        const cardSettings: CardSettings | undefined = await new Promise(async (resolve) => {
            const modal = await this.modalController.create({
                component: EditCardDialogComponent,
                backdropDismiss: false,
                componentProps: {
                    card: { name: file.name, size: file.content.length },
                    cardSupportLevel: supportLevel,
                    newCard: true,
                    onCancel: () => {
                        modal.dismiss();
                        resolve(undefined);
                    },
                    onSave: (updatedCard: StorageCard) => {
                        resolve(updatedCard);
                        modal.dismiss();
                    },
                },
            });

            await modal.present();
        });

        if (cardSettings) this.storageCardService.createCardFromFile(cardSettings.name, file);
    };

    public lastCardTouched: number | undefined = undefined;
}
