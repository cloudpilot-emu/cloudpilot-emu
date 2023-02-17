import { CardOwner, StorageCardContext } from './storage-card-context';
import { FsckContext, FsckResult, mkfs } from '@common/bridge/FSTools';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { FileService } from './file.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { Session } from '@pwa/model/Session';
import { SessionService } from '@pwa/service/session.service';
import { SnapshotService } from '@pwa/service/snapshot.service';
import { StorageService } from '@pwa/service/storage.service';
import { VfsService } from './vfs.service';
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
        private fileService: FileService,
        private vfsService: VfsService,
        private storageCardContext: StorageCardContext
    ) {
        void this.updateCardsFromDB().then(() => (this.loading = false));

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

            return await this.storageService.importStorageCard(cardWithoutId, cardImage);
        } finally {
            void loader.dismiss();
        }
    }

    async createCardFromImage(name: string, image: Uint8Array, dontFsckAutomatically: boolean): Promise<void> {
        const cardWithoutId: Omit<StorageCard, 'id'> = {
            storageId: newStorageId(),
            name,
            size: image.length,
            status: StorageCardStatus.clean,
            dontFsckAutomatically,
        };
        const data32 = new Uint32Array(image.buffer, image.byteOffset, image.byteLength >>> 2);

        const loader = await this.loadingController.create({ message: 'Importing...' });
        try {
            if (dontFsckAutomatically) {
                await loader.present();
                await this.storageService.importStorageCard(cardWithoutId, data32);
            } else {
                const [fixedData32, checkedCardWithoutId] = await this.fsckNewCard(data32, cardWithoutId);

                await loader.present();
                await this.storageService.importStorageCard(checkedCardWithoutId, fixedData32);
            }
        } finally {
            void loader.dismiss();
        }
    }

    onEmulatorStop(): void {
        const mountedCard = this.emulationStateService.getCurrentSession()?.mountedCard;
        if (mountedCard !== undefined) this.storageCardContext.release(mountedCard, CardOwner.cloudpilot);
    }

    async loadCardInEmulator(cardId: number, data?: Uint32Array): Promise<StorageCard> {
        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        await this.vfsService.releaseCard(cardId);

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

        this.storageCardContext.claim(cardId, CardOwner.cloudpilot);

        if (data) cardData.set(data);
        else await this.storageService.loadCardData(card.id, cardData, CardOwner.cloudpilot);

        return card;
    }

    async ejectCurrentCard(): Promise<void> {
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
        this.storageCardContext.release(cardId, CardOwner.cloudpilot);
    }

    async checkCard(card: StorageCard): Promise<void> {
        const session = this.mountedInSession(card.id);
        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be checked.`,
                { 'Eject now': () => (forceEject = true) },
                'Cancel'
            );

            if (forceEject) {
                await this.ejectCard(card.id);
            } else {
                return;
            }
        }

        const fsckContext = await this.executeFsckAndUpdateCard(card.id);

        switch (fsckContext.getResult()) {
            case FsckResult.ok:
                void this.alertService.cardIsClean();
                return;

            case FsckResult.invalid:
                void this.alertService.cardHasNoValidFileSystem();
                return;

            case FsckResult.unfixable:
                void this.alertService.cardHasUncorrectableErrors();
                break;

            case FsckResult.fixed: {
                let fixErrors = false;

                await this.alertService.message(
                    'Filesystem errors',
                    `The filesystem on this card contains errors${
                        card.dontFsckAutomatically ? '' : '  that need to be fixed before it can be used'
                    }. Do you want to fix them now?`,
                    { 'Fix now': () => (fixErrors = true) },
                    'Cancel'
                );

                if (fixErrors) {
                    await this.applyFsckResult(card.id, fsckContext);
                } else {
                    this.storageCardContext.release(card.id, CardOwner.fstools);
                }

                return;
            }
        }
    }

    insertCard(id: number): Promise<boolean> {
        return this.checkAndClaimCard(id, 'insert');
    }

    async browseCard(card: StorageCard): Promise<boolean> {
        const session = this.mountedInSession(card.id);

        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be browsed.`,
                { 'Eject now': () => (forceEject = true) },
                'Cancel'
            );

            if (forceEject) {
                await this.ejectCard(card.id);
            } else {
                return false;
            }
        }

        return this.checkAndClaimCard(card.id, 'vfs');
    }

    async updateCard(id: number, update: Partial<StorageCard>): Promise<void> {
        const card = await this.storageService.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        if (!update.dontFsckAutomatically && card.dontFsckAutomatically) update.status = StorageCardStatus.dirty;

        return this.storageService.updateStorageCardPartial(id, update);
    }

    async deleteCard(id: number): Promise<void> {
        await this.vfsService.releaseCard(id);
        await this.ejectCard(id);
        await this.storageService.deleteStorageCard(id);
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
                await this.storageService.loadCardData(card.id, data, this.storageCardContext.getOwner(id));
            }
        } finally {
            void loader.dismiss();
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

    private async checkAndClaimCard(id: number, action: 'insert' | 'vfs'): Promise<boolean> {
        if (await this.cardIsMounted(id)) throw new Error('card already mounted');

        let card = await this.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        let fsckContext: FsckContext | undefined;
        let mountNow = true;
        let mountReadonly = false;

        if (!card.dontFsckAutomatically) {
            if (card.status === StorageCardStatus.dirty) {
                fsckContext = await this.executeFsckAndUpdateCard(id);

                if (fsckContext.getResult() === FsckResult.fixed) {
                    mountNow = false;

                    await this.alertService.message(
                        'Filesystem errors',
                        `The filesystem on this card contains errors that have to fixed before it can be ${
                            action === 'insert' ? 'inserted' : 'browsed'
                        }. Do you want to fix them now?`,
                        { 'Fix now': () => (mountNow = true) },
                        'Cancel'
                    );

                    if (!mountNow) {
                        this.storageCardContext.release(id, CardOwner.fstools);
                        return false;
                    }

                    await this.applyFsckResult(card.id, fsckContext);
                }

                card = await this.getCard(id);
                if (!card) throw new Error(`no card with id ${id}`);
            }

            switch (card.status) {
                case StorageCardStatus.clean:
                    break;

                case StorageCardStatus.unformatted:
                    if (action === 'vfs') {
                        await this.alertService.message(
                            'Card unformatted',
                            'This card is unformatted and cannot be browsed. Insert it into a device and format it.'
                        );

                        return false;
                    }

                case StorageCardStatus.dirty:
                    await this.alertService.message(
                        'Card requires check',
                        `This card needs to be checked before it can be ${
                            action === 'insert' ? 'inserted' : 'browsed'
                        }.`
                    );

                    return false;

                case StorageCardStatus.unfixable:
                    mountNow = false;
                    mountReadonly = true;

                    await this.alertService.message(
                        'Uncorrectable errors',
                        `The filesystem on this card contains uncorrectable errors. ${
                            action === 'insert'
                                ? 'Writing could cause further damage. Do you want to insert it nevertheless?'
                                : 'The card will be mounted read-only in order to prevent further damage.'
                        }`,
                        { 'Insert card': () => (mountNow = true) },
                        'Cancel'
                    );

                    if (!mountNow) return false;
                    break;

                default:
                    throw new Error('unreachable: bad card status');
            }
        }

        try {
            if (action === 'insert') {
                await this.vfsService.releaseCard(id);
                await this.mountCardInEmulator(id, fsckContext?.getImageData());
            } else {
                if (!(await this.mountCardForBrowsing(id, mountReadonly, fsckContext?.getImageData()))) {
                    await this.alertService.message(
                        'Failed to mount card',
                        'The card could not be mounted for browsing.'
                    );
                }
            }
        } catch (e) {
            this.errorService.fatalBug(e instanceof Error ? e.message : 'mount failed');
            return false;
        }

        return true;
    }

    private async executeFsckAndUpdateCard(cardId: number): Promise<FsckContext> {
        const loader = await this.loadingController.create({ message: 'Checking card...' });

        try {
            await loader.present();

            if (await this.cardIsMounted(cardId)) throw new Error('attempt to fsck a mounted card');

            const card = await this.storageService.getCard(cardId);
            if (!card) {
                throw new Error(`no card with id ${cardId}`);
            }

            await this.vfsService.releaseCard(cardId);
            this.storageCardContext.claim(cardId, CardOwner.fstools);
            const context = await FsckContext.create(card.size);

            await this.storageService.loadCardData(card.id, context.getImageData(), CardOwner.fstools);

            const result = context.fsck();
            switch (result) {
                case FsckResult.ok:
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.clean });
                    this.storageCardContext.release(cardId, CardOwner.fstools);

                    break;

                case FsckResult.invalid:
                    await this.storageService.updateStorageCardPartial(cardId, {
                        status: StorageCardStatus.unformatted,
                    });
                    this.storageCardContext.release(cardId, CardOwner.fstools);

                    break;

                case FsckResult.fixed:
                    // Do NOT release ownership in this case --- the caller may either apply the changes or release themselves.
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
                    break;

                case FsckResult.unfixable:
                    await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.unfixable });
                    this.storageCardContext.release(cardId, CardOwner.fstools);

                    break;

                default:
                    throw new Error('unrachable: invalid fsck result');
            }

            return context;
        } finally {
            void loader.dismiss();
        }
    }

    private async applyFsckResult(cardId: number, context: FsckContext): Promise<void> {
        if (context.getResult() !== FsckResult.fixed) return;

        const loader = await this.loadingController.create({ message: 'Fixing errors...' });

        try {
            await loader.present();
            await this.storageService.updateCardData(
                cardId,
                context.getImageData(),
                context.getDirtyPages(),
                CardOwner.fstools
            );
            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.clean });

            this.storageCardContext.release(cardId, CardOwner.fstools);
        } finally {
            void loader.dismiss();
        }
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
            void loader.dismiss();
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

    private async ejectCard(id: number): Promise<void> {
        const session = this.mountedInSession(id);
        const card = await this.storageService.getCard(id);
        if (!session || !card) return;

        if (session.id === this.emulationStateService.getCurrentSession()?.id) {
            await this.ejectCurrentCard();
        } else {
            await this.storageService.updateSessionPartial(session.id, { mountedCard: undefined });
            await this.storageService.updateStorageCardPartial(id, { status: StorageCardStatus.dirty });
        }
    }

    private async mountCardInEmulator(cardId: number, data?: Uint32Array): Promise<StorageCard> {
        if (await this.cardIsMounted(cardId)) throw new Error('card already mounted');

        const loader = await this.loadingController.create({ message: 'Inserting...' });

        try {
            await loader.present();
            await this.vfsService.releaseCard(cardId);

            const session = this.emulationStateService.getCurrentSession();
            if (!session) throw new Error('no current session');
            if (session.mountedCard !== undefined) throw new Error('session already has a mounted card');

            const card = await this.loadCardInEmulator(cardId, data);
            const cloudpilot = await this.cloudpilotService.cloudpilot;

            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
            await this.storageService.updateSessionPartial(session.id, { mountedCard: cardId });

            if (!cloudpilot.mountCard(card.storageId)) {
                cloudpilot.removeCard(card.storageId);

                throw new Error('failed to mount card');
            }

            return card;
        } finally {
            void loader.dismiss();
        }
    }

    private async mountCardForBrowsing(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        if (this.vfsService.currentCard()?.id === id) return true;

        const loader = await this.loadingController.create({ message: 'Mounting...' });

        try {
            await loader.present();
            return await this.vfsService.mountCardUnchecked(id, readonly, data);
        } finally {
            void loader.dismiss();
        }
    }

    private loading = true;

    private cards: Array<StorageCard> = [];
    private updateMutex = new Mutex();
}
