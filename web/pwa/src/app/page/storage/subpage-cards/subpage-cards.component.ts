import { ActionSheetController, AlertController, ModalController } from '@ionic/angular';
import { CardSettings, EditCardDialogComponent } from '@pwa/page/storage/edit-card-dialog/edit-card-dialog.component';
import { Component, Input } from '@angular/core';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { FsckContext, FsckResult } from '@common/bridge/FSTools';
import { NewCardSize, StorageCardService } from '@pwa/service/storage-card.service';

import { AlertService } from '@pwa/service/alert.service';
import { CardSupportLevel } from '@common/bridge/Cloudpilot';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { ErrorService } from '@pwa/service/error.service';
import { NewCardDialogComponent } from '../new-card-dialog/new-card-dialog.component';
import { StorageCard } from '@pwa/model/StorageCard';
import { VfsService } from '@pwa/service/vfs.service';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameFragment } from '@pwa/helper/filename';

@Component({
    selector: 'app-subpage-cards',
    templateUrl: './subpage-cards.component.html',
    styleUrls: ['./subpage-cards.component.scss'],
})
export class SubpageCardsComponent {
    constructor(
        private actionSheetController: ActionSheetController,
        public storageCardService: StorageCardService,
        private modalController: ModalController,
        private alertController: AlertController,
        private cloudpilotService: CloudpilotService,
        private alertService: AlertService,
        private errorService: ErrorService,
        private vfsService: VfsService,
        private fileService: FileService
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

    showHelp(): void {}

    async selectCard(card: StorageCard) {
        const session = this.storageCardService.mountedInSession(card.id);
        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be browsed.`,
                { 'Eject now': () => (forceEject = true) }
            );

            if (forceEject) {
                await this.storageCardService.forceEjectCard(card.id);
            } else {
                return;
            }
        }

        if (await this.storageCardService.attachCardToVfs(card.id)) {
            this.onMountForBrowse(card);
        }
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

    saveCard(card: StorageCard) {
        this.storageCardService.saveCard(card.id, `${filenameFragment(card.name.replace(/\.img$/, ''))}.img`);
    }

    async checkCard(card: StorageCard): Promise<void> {
        const session = this.storageCardService.mountedInSession(card.id);
        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be checked.`,
                { 'Eject now': () => (forceEject = true) }
            );

            if (forceEject) {
                await this.storageCardService.forceEjectCard(card.id);
            } else {
                return;
            }
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
        const alert = await this.alertController.create({
            header: 'Warning',
            backdropDismiss: false,
            message: `Deleting the card '${card.name}' cannot be undone. Are you sure you want to continue?`,
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                {
                    text: 'Delete',
                    handler: async () => {
                        try {
                            await this.storageCardService.deleteCard(card.id);
                        } catch (e) {
                            this.errorService.fatalBug(e instanceof Error ? e.message : 'delete failed');
                        }
                    },
                },
            ],
        });

        await alert.present();
    }

    trackCardBy(index: number, card: StorageCard) {
        return card.id;
    }

    private importCardImage(): void {
        this.fileService.openFile(this.handleCardImport);
    }

    private async createNewCardImage(): Promise<void> {
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

    private disambiguateName(name: string): string {
        const cards = this.storageCardService.getAllCards();

        return disambiguateName(name, (x) => cards.some((card) => card.name === x));
    }

    private async applyFsckResult(cardId: number, context: FsckContext) {
        await this.storageCardService.applyFsckResult(cardId, context);

        this.alertService.message('Card fixed', 'All filesystem errors have been fixed.');
    }

    lastCardTouched: number | undefined = undefined;

    @Input()
    onMountForBrowse: (card: StorageCard) => void = () => undefined;
}
