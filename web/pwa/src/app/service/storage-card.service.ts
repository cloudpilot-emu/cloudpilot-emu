import { FsckContext, FsckResult, mkfs } from '@common/bridge/FSTools';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { FileDescriptor } from '@pwa/service/file.service';
import { FileService } from './file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';
import { SnapshotService } from '@pwa/service/snapshot.service';
import { StorageService } from '@pwa/service/storage.service';
import { v4 as uuid } from 'uuid';

export enum NewCardSize {
    mb4,
    mb8,
    mb16,
    mb32,
    mb64,
    mb128,
}

export function cardSizeNumeric(newCardSize: NewCardSize): number {
    switch (newCardSize) {
        case NewCardSize.mb4:
            return 4;

        case NewCardSize.mb8:
            return 8;

        case NewCardSize.mb16:
            return 16;

        case NewCardSize.mb32:
            return 32;

        case NewCardSize.mb64:
            return 64;

        case NewCardSize.mb128:
            return 128;

        default:
            throw new Error(`bad card size ${newCardSize}`);
    }
}

export function calculateNewCardSizeBytes(newCardSize: NewCardSize): number {
    switch (newCardSize) {
        case NewCardSize.mb4:
            return 494 * 16 * 512;

        case NewCardSize.mb8:
            return (2 * 496 - 2) * 16 * 512;

        case NewCardSize.mb16:
            return (2 * 496 - 2) * 32 * 512;

        case NewCardSize.mb32:
            return (4 * 496 - 2) * 32 * 512;

        case NewCardSize.mb64:
            return (8 * 496 - 2) * 32 * 512;

        case NewCardSize.mb128:
            return (16 * 496 - 2) * 32 * 512;

        default:
            throw new Error(`bad card size ${newCardSize}`);
    }
}

function newStorageId(): string {
    return uuid().replace(/-/g, '');
}

@Injectable({ providedIn: 'root' })
export class StorageCardService {
    constructor(
        private sessionService: SessionService,
        private storageService: StorageService,
        private emulationStateService: EmulationStateService,
        private cloudpilotService: CloudpilotService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService,
        private alertService: AlertService,
        private errorService: ErrorService,
        private fileService: FileService
    ) {
        this.updateCardsFromDB().then(() => (this.loading = false));

        storageService.storageCardChangeEvent.addHandler(() => this.updateCardsFromDB());
    }

    getAllCards(): Array<StorageCard> {
        return this.cards;
    }

    async createEmptyCard(name: string, size: NewCardSize, dontFsckAutomatically: boolean): Promise<StorageCard> {
        const loader = await this.loadingController.create({ message: 'Formatting...' });

        try {
            await loader.present();

            const cardImage = await mkfs(cardSizeNumeric(size));
            if (!cardImage) throw new Error('failed to create image');

            const cardWithoutId: Omit<StorageCard, 'id'> = {
                storageId: newStorageId(),
                name,
                size: cardImage.length * 4,
                status: StorageCardStatus.clean,
                dontFsckAutomatically,
            };

            const card = await this.storageService.importStorageCard(cardWithoutId, cardImage);
            this.updateCardsFromDB();

            return card;
        } finally {
            loader.dismiss();
        }
    }

    async createCardFromFile(name: string, file: FileDescriptor, dontFsckAutomatically: boolean): Promise<void> {
        const cardWithoutId: Omit<StorageCard, 'id'> = {
            storageId: newStorageId(),
            name,
            size: file.content.length,
            status: StorageCardStatus.clean,
            dontFsckAutomatically,
        };
        const data32 = new Uint32Array(file.content.buffer, file.content.byteOffset, file.content.byteLength >>> 2);
        let card: StorageCard;

        const loader = await this.loadingController.create({ message: 'Importing...' });
        try {
            if (dontFsckAutomatically) {
                await loader.present();
                card = await this.storageService.importStorageCard(cardWithoutId, data32);
            } else {
                const [fixedData32, checkedCardWithoutId] = await this.fsckNewCard(data32, cardWithoutId);

                await loader.present();
                card = await this.storageService.importStorageCard(checkedCardWithoutId, fixedData32);
            }
        } finally {
            loader.dismiss();
        }

        this.updateCardsFromDB();
    }

    async insertCard(id: number): Promise<void> {
        if (await this.cardIsMounted(id)) throw new Error('card already mounted');

        let card = await this.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        let fsckContext: FsckContext | undefined;
        let mountNow = true;

        if (!card.dontFsckAutomatically) {
            if (card.status === StorageCardStatus.dirty) {
                fsckContext = await this.fsckCard(id);

                if (fsckContext.getResult() === FsckResult.fixed) {
                    mountNow = false;

                    await this.alertService.message(
                        'Filesystem errors',
                        'The filesystem on this card contains errors that have to fixed before it can be inserted. Do you want to fix them now?',
                        { 'Fix now': () => (mountNow = true) },
                        'Cancel'
                    );

                    if (!mountNow) return;
                    await this.applyFsckResult(card.id, fsckContext);
                }

                card = await this.getCard(id);
                if (!card) throw new Error(`no card with id ${id}`);
            }

            switch (card.status) {
                case StorageCardStatus.clean:
                case StorageCardStatus.unformatted:
                    break;

                case StorageCardStatus.dirty:
                    await this.alertService.message(
                        'Card requires check',
                        'This card needs to be checked before it can be inserted.'
                    );
                    return;

                case StorageCardStatus.unfixable:
                    mountNow = false;

                    await this.alertService.message(
                        'Uncorrectable errors',
                        'The filesystem on this card contains uncorrectable errors. Writing could cause further damage. Do you want to insert it nevertheless?',
                        { 'Insert card': () => (mountNow = true) },
                        'Cancel'
                    );

                    if (!mountNow) return;
                    break;

                default:
                    throw new Error('unreachable: bad card status');
            }
        }

        try {
            await this.mountCard(id, fsckContext?.getImageData());
        } catch (e) {
            this.errorService.fatalBug(e instanceof Error ? e.message : 'mount failed');
        }
    }

    async loadCard(cardId: number, data?: Uint32Array): Promise<StorageCard> {
        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        const cloudpilot = await this.cloudpilotService.cloudpilot;
        if (cloudpilot.getMountedKey()) {
            throw new Error('attempt to mount a card while another card is mounted');
        }

        if (!cloudpilot.allocateCard(card.storageId, card.size)) {
            throw new Error('failed to allocate card');
        }

        const cardData = cloudpilot.getCardData(card.storageId);
        if (!cardData) {
            throw new Error('failed to access card data');
        }

        if (data) cardData.set(data);
        else await this.storageService.loadCardData(card.id, cardData);

        return card;
    }

    async ejectCard(): Promise<void> {
        const session = this.emulationStateService.getCurrentSession();
        if (!session) throw new Error('no running session');

        const cardId = session?.mountedCard;
        if (cardId === undefined) {
            throw new Error(`no mounted card`);
        }

        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
        await this.snapshotService.waitForPendingSnapshot();
        await this.snapshotService.triggerSnapshot();
        await this.storageService.updateSessionPartial(session.id, { mountedCard: undefined });

        const cloudpilot = await this.cloudpilotService.cloudpilot;
        cloudpilot.removeCard(card.storageId);

        this.snapshotService.resetCard();
    }

    async fsckCard(cardId: number): Promise<FsckContext> {
        const loader = await this.loadingController.create({ message: 'Checking card...' });

        try {
            await loader.present();

            if (await this.cardIsMounted(cardId)) throw new Error('attempt to fsck a mounted card');

            const card = await this.storageService.getCard(cardId);
            if (!card) {
                throw new Error(`no card with id ${cardId}`);
            }

            const context = await FsckContext.create(card.size);
            await this.storageService.loadCardData(card.id, context.getImageData());

            const result = context.fsck();
            switch (result) {
                case FsckResult.ok:
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.clean });
                    break;

                case FsckResult.invalid:
                    await this.storageService.updateStorageCardPartial(cardId, {
                        status: StorageCardStatus.unformatted,
                    });
                    break;

                case FsckResult.fixed:
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
                    break;

                case FsckResult.unfixable:
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.unfixable });
                    break;

                default:
                    throw new Error('unrachable: invalid fsck result');
            }

            return context;
        } finally {
            loader.dismiss();
        }
    }

    async applyFsckResult(cardId: number, context: FsckContext): Promise<void> {
        if (context.getResult() !== FsckResult.fixed) return;

        const loader = await this.loadingController.create({ message: 'Fixing errors...' });

        try {
            await loader.present();
            await this.storageService.updateCardData(cardId, context.getImageData(), context.getDirtyPages());
            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.clean });
        } finally {
            loader.dismiss();
        }
    }

    async updateCard(id: number, update: Partial<StorageCard>): Promise<void> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        if (!update.dontFsckAutomatically && card.dontFsckAutomatically) update.status = StorageCardStatus.dirty;

        return this.storageService.updateStorageCardPartial(id, update);
    }

    async deleteCard(card: StorageCard): Promise<void> {
        const session = this.mountedInSession(card.id);

        if (session) {
            await this.storageService.updateSessionPartial(session.id, { mountedCard: undefined });

            if (session.id === this.emulationStateService.getCurrentSession()?.id) {
                const cloudpilot = await this.cloudpilotService.cloudpilot;
                cloudpilot.removeCard(card.storageId);

                this.snapshotService.resetCard();
            }
        }

        await this.storageService.deleteStorageCard(card.id);

        this.updateCardsFromDB();
    }

    async saveCard(id: number, name: string): Promise<void> {
        const card = await this.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        const loader = await this.loadingController.create({ message: 'Exporting...' });
        let data: Uint32Array | undefined;

        try {
            await loader.present();

            const session = this.mountedInSession(id);

            if (session && session.id === this.emulationStateService.getCurrentSession()?.id) {
                await this.snapshotService.waitForPendingSnapshot();
                await this.snapshotService.triggerSnapshot();

                const cloudpilot = await this.cloudpilotService.cloudpilot;
                data = cloudpilot.getCardData(card.storageId);
            }

            if (!data) {
                data = new Uint32Array(card.size >>> 2);
                await this.storageService.loadCardData(card.id, data);
            }
        } finally {
            loader.dismiss();
        }

        this.fileService.saveFile(name, new Uint8Array(data.buffer, data.byteOffset, data.length << 2));
    }

    async getCard(id: number): Promise<StorageCard | undefined> {
        return this.storageService.getCard(id);
    }

    mountedInSession(cardId: number): Session | undefined {
        return this.sessionService.getSessions().find((session) => session.mountedCard === cardId);
    }

    isLoading(): boolean {
        return this.loading;
    }

    private async updateCardsFromDB(): Promise<void> {
        this.cards = await this.updateMutex.runExclusive(async () =>
            (await this.storageService.getAllStorageCards()).sort((x, y) => x.name.localeCompare(y.name))
        );
    }

    private async cardIsMounted(id: number): Promise<boolean> {
        const sessions = await this.storageService.getAllSessions();

        return sessions.some((session) => session.mountedCard === id);
    }

    private async fsckNewCard(
        data: Uint32Array,
        cardWithoutId: Omit<StorageCard, 'id'>
    ): Promise<[Uint32Array, Omit<StorageCard, 'id'>]> {
        const loader = await this.loadingController.create({ message: 'Checking card...' });
        let fsckContext: FsckContext;
        let result: FsckResult;

        try {
            await loader.present();

            fsckContext = await FsckContext.create(cardWithoutId.size);
            fsckContext.getImageData().set(data);
            result = fsckContext.fsck();
        } finally {
            loader.dismiss();
        }

        switch (result) {
            case FsckResult.ok:
                return [data, { ...cardWithoutId, status: StorageCardStatus.clean }];

            case FsckResult.fixed: {
                let fix = false;

                await this.alertService.message(
                    'Filesystem errors',
                    'The filesystem on this card contains errors that must be fixed before the card can be used. Do you want to fix them now?',
                    { 'Fix now': () => (fix = true) },
                    'Skip'
                );

                return fix
                    ? [fsckContext.getImageData(), { ...cardWithoutId, status: StorageCardStatus.clean }]
                    : [data, { ...cardWithoutId, status: StorageCardStatus.dirty }];
            }

            case FsckResult.invalid:
                await this.alertService.cardHasNoValidFileSystem();

                return [data, { ...cardWithoutId, status: StorageCardStatus.unformatted }];

            case FsckResult.unfixable:
                await this.alertService.cardHasUncorrectableErrors();

                return [data, { ...cardWithoutId, status: StorageCardStatus.unfixable }];

            default:
                throw new Error('unreachable: bad fsck result');
        }
    }

    private async mountCard(cardId: number, data?: Uint32Array): Promise<StorageCard> {
        if (await this.cardIsMounted(cardId)) throw new Error('card already mounted');

        const loader = await this.loadingController.create({ message: 'Inserting...' });

        try {
            await loader.present();

            const session = this.emulationStateService.getCurrentSession();
            if (!session) throw new Error('no current session');
            if (session.mountedCard !== undefined) throw new Error('session already has a mounted card');

            const card = await this.loadCard(cardId, data);
            const cloudpilot = await this.cloudpilotService.cloudpilot;

            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
            await this.storageService.updateSessionPartial(session.id, { mountedCard: cardId });

            if (!cloudpilot.mountCard(card.storageId)) {
                cloudpilot.removeCard(card.storageId);

                throw new Error('failed to mount card');
            }

            return card;
        } finally {
            loader.dismiss();
        }
    }

    private loading = true;

    private cards: Array<StorageCard> = [];
    private updateMutex = new Mutex();
}
