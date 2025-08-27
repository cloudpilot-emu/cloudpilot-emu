import * as angular from '@ionic/angular';
import { CardSettings, EditCardDialogComponent } from '@pwa/page/storage/edit-card-dialog/edit-card-dialog.component';
import { ChangeDetectionStrategy, ChangeDetectorRef, Component, DoCheck, Input, OnInit } from '@angular/core';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { NewCardSize, StorageCardService } from '@pwa/service/storage-card.service';
import { AlertService } from '@pwa/service/alert.service';
import { CardSupportLevel } from '@common/bridge/Cloudpilot';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { ErrorService } from '@pwa/service/error.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { NewCardDialogComponent } from '../new-card-dialog/new-card-dialog.component';
import { StorageCard } from '@pwa/model/StorageCard';
import { changeDetector } from '@pwa/helper/changeDetect';
import { debounce } from '@pwa/helper/debounce';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameFragment } from '@pwa/helper/filename';
import { FsToolsService } from '@pwa/service/fstools.service';

import helpPage from '@assets/doc/cards.md';
import { SessionService } from '@pwa/service/session.service';
import { ActionMenuCardsComponent } from '../action-menu-cards/action-menu-cards.component';

type Mode = 'manage' | 'select-for-export' | 'select-for-delete';

@Component({
    selector: 'app-subpage-cards',
    templateUrl: './subpage-cards.component.html',
    styleUrls: ['./subpage-cards.component.scss'],
    changeDetection: ChangeDetectionStrategy.OnPush,
    standalone: false,
})
export class SubpageCardsComponent implements DoCheck, OnInit {
    constructor(
        private actionSheetController: angular.ActionSheetController,
        public storageCardService: StorageCardService,
        private modalController: angular.ModalController,
        private cloudpilotService: CloudpilotService,
        private alertService: AlertService,
        private errorService: ErrorService,
        private fileService: FileService,
        private loadingController: angular.LoadingController,
        private fstools: FsToolsService,
        private popoverController: angular.PopoverController,
        sessionService: SessionService,
        private cd: ChangeDetectorRef,
    ) {
        this.checkCards = changeDetector(cd, [], () => this.storageCardService.getAllCards());
        this.checkSessions = changeDetector(cd, undefined, () => sessionService.getSessions());
    }

    ngOnInit(): void {
        if (this.selfReference) this.selfReference.ref = this;
    }

    ngDoCheck(): void {
        this.checkCards();
        this.checkSessions();
    }

    ionViewDidLeave(): void {
        this.mode = 'manage';
        this.cd.markForCheck();
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

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: helpPage,
            },
        });
        await modal.present();
    }

    @debounce()
    async browseCard(card: StorageCard) {
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
    async saveCard(card: StorageCard): Promise<void> {
        const filename = `${filenameFragment(card.name.replace(/\.img$/, ''))}.img`;

        const sheet = await this.actionSheetController.create({
            header: 'How do you want to export the image?',
            buttons: [
                {
                    text: 'Export gzip compressed image',
                    handler: () => this.storageCardService.saveCard(card.id, filename, true),
                },
                {
                    text: 'Export uncompressed image',
                    handler: () => this.storageCardService.saveCard(card.id, filename, false),
                },

                { text: 'Cancel' },
            ],
        });

        await sheet.present();
    }

    @debounce()
    checkCard(card: StorageCard): Promise<void> {
        return this.storageCardService.checkCard(card);
    }

    @debounce()
    async deleteCard(card: StorageCard) {
        let abort = true;

        await this.alertService.message(
            'Warning',
            `Deleting the card '${card.name}' cannot be undone. Are you sure you want to continue?`,
            {
                Delete: () => (abort = false),
            },
            'Cancel',
        );

        if (abort) return;

        const loader = await this.loadingController.create({ message: 'Deleting...' });
        await loader.present();

        try {
            await this.storageCardService.deleteCard(card.id);
        } catch (e) {
            this.errorService.fatalBug(e instanceof Error ? e.message : 'delete failed');
        } finally {
            void loader.dismiss();
        }
    }

    trackCardBy(index: number, card: StorageCard) {
        return card.id;
    }

    startMassExport(): void {
        this.selection.clear();
        this.mode = 'select-for-export';
    }

    startMassDelete(): void {
        this.selection.clear();
        this.mode = 'select-for-delete';
    }

    @debounce()
    async onSelectionDone(): Promise<void> {
        const wasMode = this.mode;
        this.mode = 'manage';

        switch (wasMode) {
            case 'select-for-export':
                await this.executeMassExport(Array.from(this.selection));
                break;

            case 'select-for-delete':
                await this.executeMassDelete(Array.from(this.selection));
                break;
        }
    }

    onSelectAll(): void {
        this.cards.forEach((card) => this.selection.add(card.id));
    }

    toggleSelection(card: StorageCard): void {
        if (this.selection.has(card.id)) this.selection.delete(card.id);
        else this.selection.add(card.id);
    }

    @debounce()
    async openActionMenu(e: MouseEvent, reference: 'event' | 'trigger' = 'trigger'): Promise<void> {
        const popover = await this.popoverController.create({
            component: ActionMenuCardsComponent,
            event: e,
            componentProps: {
                cardCount: this.cards.length,
                onExport: () => {
                    void popover.dismiss();
                    this.startMassExport();
                    this.cd.markForCheck();
                },
                onDelete: () => {
                    void popover.dismiss();
                    this.startMassDelete();
                    this.cd.markForCheck();
                },
                onHelp: () => {
                    void popover.dismiss();
                    void this.showHelp();
                },
            },
            arrow: false,
            translucent: true,
            reference,
        });

        void popover.present();
    }

    onContextMenu(e: MouseEvent): void {
        if (this.mode !== 'manage') return;

        e.preventDefault();

        void this.openActionMenu(e, 'event');
    }

    private async executeMassExport(selection: Array<number>): Promise<void> {
        switch (selection.length) {
            case 0:
                break;

            case 1: {
                const card = await this.storageCardService.getCard(selection[0]);
                if (card) await this.saveCard(card);

                break;
            }

            default:
                await this.exportSelection(selection);
                break;
        }
    }

    private async executeMassDelete(selection: Array<number>): Promise<void> {
        switch (selection.length) {
            case 0:
                break;

            case 1: {
                const card = await this.storageCardService.getCard(selection[0]);
                if (card) await this.deleteCard(card);

                break;
            }

            default: {
                let abort = true;

                await this.alertService.message(
                    'Warning',
                    `You are about to delete ${selection.length} cards. This cannot be undone. Are you sure you want to continue?`,
                    {
                        Delete: () => (abort = false),
                    },
                    'Cancel',
                );

                if (abort) return;

                const loader = await this.loadingController.create({ message: 'Deleting...' });
                await loader.present();

                try {
                    for (const id of selection) await this.storageCardService.deleteCard(id);
                } catch (e) {
                    this.errorService.fatalBug(e instanceof Error ? e.message : 'delete failed');
                } finally {
                    void loader.dismiss();
                }
            }
        }
    }

    private async exportSelection(selection: Array<number>): Promise<void> {
        const cards = new Map(await this.storageCardService.getAllCards().map((card) => [card.id, card]));

        const size = selection.map((id) => cards.get(id)?.size).reduce((acc, x): number => acc! + (x ?? 0), 0)!;
        if (size > 256 * 1024 * 1024) {
            let abort = true;

            await this.alertService.message(
                'Warning',
                `
                    You are about to export a large amount of data. This may consume a lot of memory and can crash the browser.
                    Are you sure you want to continue?
                `,
                {
                    Continue: () => (abort = false),
                },
                'Cancel',
            );

            if (abort) return;
        }

        await this.storageCardService.saveCards(selection);
    }

    private handleFilesFromDrop(files: Array<FileDescriptor>): void {
        if (files.length === 0) return;

        if (files.length > 1) {
            void this.alertService.errorMessage(
                'Drop a single file system image in order to import it as a new memory card.',
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
        if (!/\.((bin|img)(|\.gz)|zip)$/i.test(file.name)) {
            void this.alertService.errorMessage(
                'Unsupported file suffix. Supported suffixes are .bin, .bin.gz, .img, .img.gz and .zip',
            );
            return;
        }

        if (file.name.endsWith('.zip')) {
            let install = false;
            await this.alertService.message(
                'Mass import',
                `
                    CloudpilotEmu will attempt to import all card images in the selected zip archive.
                    Importing large archives (several 100MB) requires a lot of memory and may crash the browser
                    or cause spurious errors if memory is exhausted.
                    Are you sure you want to continue?
                `,
                { Continue: () => (install = true) },
                'Cancel',
            );

            if (install) await this.storageCardService.addAllCardsFromArchive(await file.getContent());
            return;
        }

        const cloudpilot = await this.cloudpilotService.cloudpilot;
        let content: Uint8Array | undefined;

        try {
            content = await file.getContent();
        } catch (e) {
            console.warn(e);

            await this.alertService.errorMessage(`Unable to open ${file.name}".`);
            return;
        }

        if (/\.gz$/.test(file.name)) {
            const loader = await this.loadingController.create({ message: 'Unpacking...' });
            await loader.present();

            try {
                content = await this.fstools.gunzip(content);
            } finally {
                await loader.dismiss();
            }
        }

        const supportLevel = content ? cloudpilot.getCardSupportLevel(content.length) : CardSupportLevel.unsupported;

        if (content === undefined || supportLevel === CardSupportLevel.unsupported) {
            void this.alertService.errorMessage('This is not a supported card image.');
            return;
        }

        const cardSettings: CardSettings | undefined = await new Promise(async (resolve) => {
            const modal = await this.modalController.create({
                component: EditCardDialogComponent,
                backdropDismiss: false,
                componentProps: {
                    card: { name: this.disambiguateName(file.name.replace(/\.gz$/, '')), size: content!.length },
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
            await this.storageCardService.createCardFromImage(
                cardSettings.name,
                content,
                !!cardSettings.dontFsckAutomatically,
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

    @Input()
    selfReference: { ref: SubpageCardsComponent } | undefined;

    private checkCards: () => void;
    private checkSessions: () => void;

    mode: Mode = 'manage';
    selection = new Set<number>();
}
