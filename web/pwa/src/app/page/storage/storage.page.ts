import { ActionSheetController, AlertController, LoadingController, ModalController } from '@ionic/angular';
import { CardSettings, EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
import { FileDescriptor, FileService } from '@pwa/service/file.service';

import { AlertService } from '@pwa/service/alert.service';
import { CardSupportLevel } from '@native/cloudpilot_web';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { Component } from '@angular/core';
import { ErrorService } from './../../service/error.service';
import { FsckContext } from '@common/FSTools';
import { FsckResult } from '@native-fstools/fstools_web';
import { NewCardDialogComponent } from './new-card-dialog/new-card-dialog.component';
import { NewCardSize } from '@pwa/service/storage-card.service';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageCardService } from './../../service/storage-card.service';
import { StorageService } from '@pwa/service/storage.service';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameFragment } from '@pwa/helper/filename';

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
        private alertService: AlertService,
        private storageService: StorageService,
        private loadingController: LoadingController,
        private actionSheetController: ActionSheetController,
        private errorService: ErrorService
    ) {}

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards();
    }

    async addCardImage(): Promise<void> {
        const sheet = await this.actionSheetController.create({
            header: 'What type of image do you want to create?',
            buttons: [
                {
                    text: 'Empty image',
                    handler: () => this.createNewCardImage(),
                },
                {
                    text: 'Import an existing image',
                    handler: () => this.importCardImage(),
                },
            ],
        });

        sheet.present();
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
                onCreate: (name: string, size: NewCardSize, dontFsckAutomatically: boolean) => {
                    this.storageCardService.createEmptyCard(name, size, dontFsckAutomatically);
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
                onSave: (update: Partial<StorageCard>) => {
                    this.storageCardService.updateCard(card.id, update);
                    modal.dismiss();
                },
            },
        });

        await modal.present();
    }

    async saveCard(card: StorageCard) {
        const session = this.storageCardService.mountedInSession(card.id);
        if (session) {
            this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be exported.`
            );

            return;
        }

        const loader = await this.loadingController.create({ message: 'Exporting...' });
        await loader.present();

        const data = new Uint32Array(card.size >>> 2);

        try {
            await this.storageService.loadCardData(card.id, data);
        } finally {
            loader.dismiss();
        }

        this.fileService.saveFile(
            `${filenameFragment(card.name.replace(/\.img$/, ''))}.img`,
            new Uint8Array(data.buffer, data.byteOffset, data.length << 2)
        );
    }

    async checkCard(card: StorageCard): Promise<void> {
        const session = this.storageCardService.mountedInSession(card.id);
        if (session) {
            this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be checked.`
            );

            return;
        }

        const fsckContext = await this.storageCardService.fsckCard(card.id);

        switch (fsckContext.getResult()) {
            case FsckResult.ok:
                this.alertService.cardIsClean();
                return;

            case FsckResult.invalid:
                this.alertService.cardHasNoValidFileSystem();
                return;

            case FsckResult.unfixable:
                this.alertService.cardHasUncorrectableErrors();
                break;

            case FsckResult.fixed:
                this.alertService.message(
                    'Filesystem errors',
                    `The filesystem on this card contains errors${
                        card.dontFsckAutomatically ? '' : '  that need to be fixed before it can be used'
                    }. Do you want to fix them now?`,
                    { 'Fix now': () => this.applyFsckResult(card.id, fsckContext) },
                    'Cancel'
                );

                return;
        }
    }

    async deleteCard(card: StorageCard) {
        const session = this.storageCardService.mountedInSession(card.id);
        if (session) {
            this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be deleted.`
            );

            return;
        }

        const alert = await this.alertController.create({
            header: 'Warning',
            backdropDismiss: false,
            message: `Deleting the card '${card.name}' cannot be undone. Are you sure you want to continue?`,
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                { text: 'Delete', handler: () => this.doDeleteCard(card) },
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
                    card: { name: this.disambiguateName(file.name), size: file.content.length },
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

        if (cardSettings) {
            await this.storageCardService.createCardFromFile(
                cardSettings.name,
                file,
                !!cardSettings.dontFsckAutomatically
            );
        }
    };

    private async doDeleteCard(card: StorageCard): Promise<void> {
        try {
            await this.storageCardService.deleteCard(card);
        } catch (e) {
            this.errorService.fatalBug(e instanceof Error ? e.message : 'delete failed');
        }
    }

    private disambiguateName(name: string): string {
        const cards = this.storageCardService.getAllCards();

        return disambiguateName(name, (x) => cards.some((card) => card.name === x));
    }

    private async applyFsckResult(cardId: number, context: FsckContext) {
        await this.storageCardService.applyFsckResult(cardId, context);

        this.alertService.message('Card fixed', 'All filesystem errors have been fixed.');
    }

    public lastCardTouched: number | undefined = undefined;
}
