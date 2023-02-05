import { ActionSheetController, AlertController, ModalController } from '@ionic/angular';
import { CardSettings, EditCardDialogComponent } from '@pwa/page/storage/edit-card-dialog/edit-card-dialog.component';
import { ChangeDetectionStrategy, ChangeDetectorRef, Component, DoCheck, Input } from '@angular/core';
import { DragDropClient, DragDropService } from './../../../service/drag-drop.service';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { NewCardSize, StorageCardService } from '@pwa/service/storage-card.service';

import { AlertService } from '@pwa/service/alert.service';
import { CardSupportLevel } from '@common/bridge/Cloudpilot';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { ErrorService } from '@pwa/service/error.service';
import { NewCardDialogComponent } from '../new-card-dialog/new-card-dialog.component';
import { StorageCard } from '@pwa/model/StorageCard';
import { changeDetector } from '@pwa/helper/changeDetect';
import { debounce } from '@pwa/helper/debounce';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameFragment } from '@pwa/helper/filename';

@Component({
    selector: 'app-subpage-cards',
    templateUrl: './subpage-cards.component.html',
    styleUrls: ['./subpage-cards.component.scss'],
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class SubpageCardsComponent implements DragDropClient, DoCheck {
    constructor(
        private actionSheetController: ActionSheetController,
        public storageCardService: StorageCardService,
        private modalController: ModalController,
        private alertController: AlertController,
        private cloudpilotService: CloudpilotService,
        private alertService: AlertService,
        private errorService: ErrorService,
        private fileService: FileService,
        private dragDropService: DragDropService,
        cd: ChangeDetectorRef
    ) {
        this.checkCards = changeDetector(cd, [], () => this.storageCardService.getAllCards());
    }

    ngDoCheck(): void {
        this.checkCards();
    }

    ionViewDidEnter(): void {
        this.dragDropService.registerClient(this);
    }

    ionViewWillLeave(): void {
        this.dragDropService.unregisterClient(this);
    }

    get cards(): Array<StorageCard> {
        return this.storageCardService.getAllCards();
    }

    handleDragDropEvent(e: DragEvent): void | Promise<void> {
        this.fileService.openFromDrop(e, this.handleFilesFromDrop.bind(this));
    }

    @debounce()
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
                { text: 'Cancel' },
            ],
        });

        void sheet.present();
    }

    showHelp(): void {}

    @debounce()
    async selectCard(card: StorageCard) {
        if (await this.storageCardService.browseCard(card)) this.onMountForBrowse(card);
    }

    @debounce()
    async editCard(card: StorageCard) {
        const cloudpilot = await this.cloudpilotService.cloudpilot;

        const modal = await this.modalController.create({
            component: EditCardDialogComponent,
            backdropDismiss: false,
            componentProps: {
                card,
                cardSupportLevel: cloudpilot.getCardSupportLevel(card.size),
                onCancel: () => modal.dismiss(),
                onSave: async (update: Partial<StorageCard>) => {
                    await this.storageCardService.updateCard(card.id, update);
                    await modal.dismiss();
                },
            },
        });

        await modal.present();
    }

    @debounce()
    saveCard(card: StorageCard): Promise<void> {
        return this.storageCardService.saveCard(card.id, `${filenameFragment(card.name.replace(/\.img$/, ''))}.img`);
    }

    @debounce()
    checkCard(card: StorageCard): Promise<void> {
        return this.storageCardService.checkCard(card);
    }

    @debounce()
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

    private handleFilesFromDrop(files: Array<FileDescriptor>): void {
        if (files.length === 0) return;

        if (files.length > 1) {
            void this.alertService.errorMessage(
                'Drop a single file system image in order to import it as a new memory card.'
            );

            return;
        }

        void this.handleCardImport(files[0]);
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
                onCreate: async (name: string, size: NewCardSize, dontFsckAutomatically: boolean) => {
                    await this.storageCardService.createEmptyCard(name, size, dontFsckAutomatically);
                    await modal.dismiss();
                },
            },
        });

        await modal.present();
    }

    private handleCardImport = async (file: FileDescriptor): Promise<void> => {
        const cloudpilot = await this.cloudpilotService.cloudpilot;
        const supportLevel = cloudpilot.getCardSupportLevel(file.content.length);

        if (supportLevel === CardSupportLevel.unsupported) {
            void this.alertService.errorMessage('This is not a supported card image.');
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
                        void modal.dismiss();
                        resolve(undefined);
                    },
                    onSave: (updatedCard: StorageCard) => {
                        resolve(updatedCard);
                        void modal.dismiss();
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

    lastCardTouched: number | undefined = undefined;

    @Input()
    onMountForBrowse: (card: StorageCard) => void = () => undefined;

    private checkCards: () => void;
}
