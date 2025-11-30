import { Inject, Injectable, Signal, signal } from '@angular/core';
import { CardSupportLevel } from '@common/bridge/Cloudpilot';
import { FsckContext, FsckResult, GzipContext } from '@common/bridge/FSTools';
import { Engine } from '@common/engine/Engine';
import { isIOS } from '@common/helper/browser';
import { LoadingController } from '@ionic/angular';
import { ZipfileWalkerState } from '@native/cloudpilot_web';
import { Mutex } from 'async-mutex';
import { Event } from 'microevent.ts';
import { v4 as uuid } from 'uuid';

import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameForCards } from '@pwa/helper/filename';
import { Lock } from '@pwa/helper/lock';
import { Session } from '@pwa/model/Session';
import { StorageCard, StorageCardStatus } from '@pwa/model/StorageCard';
import { SessionService } from '@pwa/service/session.service';
import { SnapshotService } from '@pwa/service/snapshot.service';
import { StorageService } from '@pwa/service/storage.service';

import { AlertService } from './alert.service';
import { EmulationContextService } from './emulation-context.service';
import { ErrorService } from './error.service';
import { FileService } from './file.service';
import { FsToolsService } from './fstools.service';
import { NativeSupportService } from './native-support.service';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { TOKEN_EMULATOR_LOCK } from './token';
import { VfsService } from './vfs.service';

export const enum NewCardSize {
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

class ImportError extends Error {
    constructor(message: string) {
        super(message);
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
        private emulationContext: EmulationContextService,
        private nativeSupportService: NativeSupportService,
        private loadingController: LoadingController,
        private snapshotService: SnapshotService,
        private alertService: AlertService,
        private errorService: ErrorService,
        private fileService: FileService,
        private vfsService: VfsService,
        private storageCardContext: StorageCardContext,
        private fstools: FsToolsService,
        @Inject(TOKEN_EMULATOR_LOCK) private emulatorLock: Lock,
    ) {
        this.vfsService.setStorageCardService(this);
        void this.updateCardsFromDB().then(() => (this.loading = false));

        storageService.storageCardChangeEvent.addHandler(() => this.updateCardsFromDB());
    }

    get cards(): Signal<Array<StorageCard>> {
        return this._cards;
    }

    async createEmptyCard(name: string, size: NewCardSize, dontFsckAutomatically: boolean): Promise<StorageCard> {
        const loader = await this.loadingController.create({ message: 'Formatting...' });

        try {
            await loader.present();

            const cardImage = await this.fstools.mkfs(cardSizeNumeric(size));
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
        const loader = await this.loadingController.create({ message: 'Importing...' });

        try {
            await loader.present();
            await this.doCreateCardFromImage(name, image, dontFsckAutomatically);
        } finally {
            void loader.dismiss();
        }
    }

    onEmulatorStop(): void {
        const mountedCard = this.emulationContext.session()?.mountedCard;
        if (mountedCard !== undefined) this.storageCardContext.release(mountedCard, CardOwner.cloudpilot);
    }

    async loadCardInEmulator(cardId: number, engine: Engine, data?: Uint32Array): Promise<StorageCard> {
        const card = await this.storageService.getCard(cardId);
        if (!card) {
            throw new Error(`no card with id ${cardId}`);
        }

        await this.vfsService.releaseCard(cardId);

        if ((await engine.getMountedKey()) !== undefined) {
            throw new Error('attempt to mount a card while another card is mounted');
        }

        const cardData = engine.allocateCard(card.storageId, card.size);
        if (!cardData) throw new Error('unable to allocate card data');

        this.storageCardContext.claim(cardId, CardOwner.cloudpilot);

        if (data) cardData.set(data);
        else await this.storageService.loadCardData(card.id, cardData, CardOwner.cloudpilot);

        return card;
    }

    async ejectCurrentCard(): Promise<void> {
        const emulationContext = this.emulationContext.context();
        if (!emulationContext) throw new Error('no running session');

        const cardId = emulationContext.session.mountedCard;
        if (cardId === undefined) {
            throw new Error(`no mounted card`);
        }

        const card = await this.storageService.getCard(cardId);
        if (!card) {
            console.warn(`no card with id ${cardId}`);
            await this.storageService.updateSessionPartial(emulationContext.session.id, { mountedCard: undefined });

            return;
        }

        await this.emulatorLock.runGuarded(async () => {
            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
            await this.snapshotService.triggerSnapshot();
            await this.storageService.updateSessionPartial(emulationContext.session.id, { mountedCard: undefined });

            await emulationContext.engine.releaseCard(card.storageId);

            this.storageCardContext.release(cardId, CardOwner.cloudpilot);
        });
    }

    async checkCard(card: StorageCard): Promise<void> {
        const session = this.mountedInSession(card.id);
        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be checked.`,
                { 'Eject now': () => (forceEject = true) },
                'Cancel',
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
                    'Cancel',
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
        if (this.storageCardContext.getOwner(card.id) === CardOwner.vfs) {
            await this.vfsService.activateCard(card.id);
            return true;
        }

        const session = this.mountedInSession(card.id);

        if (session) {
            let forceEject = false;

            await this.alertService.message(
                'Card is attached',
                `This card needs to be ejected from session '${session.name}' before it can be browsed.`,
                { 'Eject now': () => (forceEject = true) },
                'Cancel',
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
        const loader = await this.loadingController.create({ message: 'Deleting...' });
        await loader.present();

        try {
            const card = this._cards().find((c) => c.id === id);

            await this.vfsService.releaseCard(id);
            await this.ejectCard(id);
            await this.storageService.deleteStorageCard(id);

            if (card) this.deleteCardEvent.dispatch(card);
        } finally {
            void loader.dismiss();
        }
    }

    async saveCard(id: number, name: string, gzip: boolean): Promise<void> {
        const card = await this.getCard(id);
        if (!card) throw new Error(`no card with id ${id}`);

        const loader = await this.loadingController.create({ message: 'Exporting...' });
        let cardData: Uint8Array | undefined;

        try {
            await loader.present();

            cardData = await this.getCardData(card, gzip ? name : undefined);
        } finally {
            void loader.dismiss();
        }

        if (cardData) {
            this.fileService.saveFile(gzip ? `${name}.gz` : name, cardData);
        }
    }

    async saveCards(ids: Array<number>): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Exporting...' });
        const createZipContext = await this.fstools.createZipContext(0);

        try {
            await loader.present();
            const names = new Set<string>();
            const gzipContext = await this.fstools.createGzipContext();

            let i = 0;
            for (const id of ids) {
                i++;

                const card = await this.getCard(id);
                if (!card) continue;

                loader.message = `Exporting ${i}/${ids.length}...`;

                const name = disambiguateName(card.name, (name) => names.has(name));
                names.add(name);

                const cardData = await this.getCardData(card, name, gzipContext);
                if (!cardData) continue;

                await createZipContext.addEntry(`${name}.img.gz`, cardData);
            }
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
        } catch (e: any) {
            await loader.dismiss();
            await this.alertService.errorMessage(`Failed to export card images: ${e?.message ?? 'unknown error'}`);
        } finally {
            void loader.dismiss();
        }

        this.fileService.saveFile(filenameForCards(), createZipContext.getContent());
    }

    async getCard(id: number): Promise<StorageCard | undefined> {
        return this.storageService.getCard(id);
    }

    mountedInSession(cardId: number): Session | undefined {
        return this.sessionService.sessions().find((session) => session.mountedCard === cardId);
    }

    isLoading(): boolean {
        return this.loading;
    }

    async addAllCardsFromArchive(zipData: Uint8Array): Promise<void> {
        const SUFFIX_PATTERN = /\.(bin|img)(|\.gz)$/i;
        const SAFARI_DISCLAIMER = isIOS
            ? `
                This can be caused by either a corrupted image or zip file,
                or by a Safari browser bug triggered by a low memory condition.
                <br><br>
                Please force close and reopen the app after a few seconds and try again. If the import
                keeps failing, please unpack the archive and import the cards manually.
            `
            : '';

        const loader = await this.loadingController.create({ message: 'Importing...' });

        try {
            await loader.present();

            const walker = await this.fstools.createZipfileWalker(zipData);
            if (walker.GetState() === ZipfileWalkerState.error) {
                await this.alertService.errorMessage('Import failed: unable to read zip file.');
                return;
            }

            const entriesTotal = walker.GetTotalEntries();
            const names = new Set<string>(this.cards().map((card) => card.name));
            const gunzipContext = await this.fstools.createGunzipContext();

            let iEntry = 1;
            let skipErrors = false;
            const failures: Array<string> = [];

            while (walker.GetState() === ZipfileWalkerState.open) {
                loader.message = `Importing ${iEntry++}/${entriesTotal}...`;
                let filename = walker.GetCurrentEntryName();

                try {
                    if (!SUFFIX_PATTERN.test(filename)) {
                        throw new ImportError(`${filename} has the wrong suffix to be considered as a card image.`);
                    }

                    let content = walker.GetCurrentEntryContent();

                    if (filename.endsWith('.gz') && content) {
                        gunzipContext.initialize(content);

                        content = (await gunzipContext.gunzip()) ? gunzipContext.getDecompressedData() : undefined;
                        filename = filename.substring(0, filename.length - 3);
                    }

                    if (content === undefined) {
                        throw new ImportError(`Failed to extract ${filename}. ${SAFARI_DISCLAIMER}`);
                    }

                    if (
                        (await this.nativeSupportService.getCardSupportLevel(content.length)) ===
                        CardSupportLevel.unsupported
                    ) {
                        throw new ImportError(`${filename} is not a valid card image.`);
                    }

                    const name = disambiguateName(filename.replace(SUFFIX_PATTERN, ''), (n) => names.has(n));

                    await this.doCreateCardFromImage(name, content, false, true);
                } catch (e: unknown) {
                    if (!(e instanceof ImportError)) throw e;

                    failures.push(filename);

                    if (!skipErrors) {
                        skipErrors = await this.alertService.messageWithChoice(
                            'Error',
                            e.message,
                            'Skip further errors',
                            skipErrors,
                            {},
                            'Continue',
                        );
                    }
                } finally {
                    walker.Next();
                }
            }

            if (failures.length <= 3 && failures.length > 0) {
                await this.alertService.errorMessage(
                    `The following files could not be imported: <br><br>${failures.join('<br>')}`,
                );
            } else if (failures.length > 3) {
                await this.alertService.errorMessage(
                    `
                        The following files could not be imported: <br><br>${failures.slice(0, 3).join('<br>')}
                        <br><br>
                        and ${failures.length - 3} more files.
                    `,
                );
            }
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
        } catch (e: any) {
            await loader.dismiss();
            await this.alertService.errorMessage(
                `Failed to import card images: ${e?.message ?? 'unknown error'}<br><br>${SAFARI_DISCLAIMER}`,
            );
        } finally {
            void loader.dismiss();
        }
    }

    private async doCreateCardFromImage(
        name: string,
        image: Uint8Array,
        dontFsckAutomatically: boolean,
        skipFsck = false,
    ): Promise<void> {
        const cardWithoutId: Omit<StorageCard, 'id'> = {
            storageId: newStorageId(),
            name,
            size: image.length,
            status: skipFsck ? StorageCardStatus.dirty : StorageCardStatus.clean,
            dontFsckAutomatically,
        };
        const data32 = new Uint32Array(image.buffer, image.byteOffset, image.byteLength >>> 2);

        if (skipFsck || dontFsckAutomatically) {
            await this.storageService.importStorageCard(cardWithoutId, data32);
        } else {
            const [fixedData32, checkedCardWithoutId] = await this.fsckNewCard(data32, cardWithoutId);

            await this.storageService.importStorageCard(checkedCardWithoutId, fixedData32);
        }
    }

    private async getCardData(
        card: StorageCard,
        gzipFilename?: string,
        gzipContext?: GzipContext,
    ): Promise<Uint8Array | undefined> {
        let cardData: Uint32Array | undefined;

        if (gzipFilename !== undefined) {
            gzipContext = gzipContext ?? (await this.fstools.createGzipContext());
            gzipContext.initialize(card.size);
        }

        if (gzipContext) {
            const buffer = gzipContext.getBuffer();
            cardData = new Uint32Array(buffer.buffer, buffer.byteOffset, buffer.byteLength >> 2);
        }

        const session = this.mountedInSession(card.id);
        let cardDataFromEmulator: Uint32Array | undefined;

        if (session && session.id === this.emulationContext.session()?.id) {
            const engine = this.emulationContext.engine();
            if (!engine) throw new Error('unreachable');

            cardDataFromEmulator = await engine.getCardData(card.storageId);

            if (cardDataFromEmulator) {
                if (cardData) cardData.set(cardDataFromEmulator);
                else cardData = cardDataFromEmulator;
            }
        }

        if (!cardDataFromEmulator) {
            if (!cardData) cardData = new Uint32Array(card.size >>> 2);
            await this.storageService.loadCardData(card.id, cardData, this.storageCardContext.getOwner(card.id));
        }

        if (gzipContext && gzipFilename) {
            gzipContext.setFilename(gzipFilename).setMtime(Math.floor(Date.now() / 1000));

            if (!(await gzipContext.gzip())) {
                await this.alertService.errorMessage('Failed to compress card image.');
                return;
            }

            return gzipContext.getGzipData();
        }

        return new Uint8Array(cardData!.buffer, cardData!.byteOffset, cardData!.byteLength);
    }

    private async updateCardsFromDB(): Promise<void> {
        this._cards.set(
            await this.updateMutex.runExclusive(async () =>
                (await this.storageService.getAllStorageCards()).sort((x, y) => x.name.localeCompare(y.name)),
            ),
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
                        'Cancel',
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
                            'This card is unformatted and cannot be browsed. Insert it into a device and format it.',
                        );

                        return false;
                    }

                    break;

                case StorageCardStatus.dirty:
                    await this.alertService.message(
                        'Card requires check',
                        `This card needs to be checked before it can be ${
                            action === 'insert' ? 'inserted' : 'browsed'
                        }.`,
                    );

                    return false;

                case StorageCardStatus.unfixable:
                    mountNow = false;
                    mountReadonly = true;

                    await this.alertService.message(
                        'Uncorrectable errors',
                        `The filesystem on this card contains uncorrectable errors. Writing could cause further damage. Do you want to insert it nevertheless?`,
                        { 'Insert card': () => (mountNow = true) },
                        'Cancel',
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
                        'The card could not be mounted for browsing.',
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
            const context = await this.fstools.createFsckContext(card.size);

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
                CardOwner.fstools,
            );
            await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.clean });

            this.storageCardContext.release(cardId, CardOwner.fstools);
        } finally {
            void loader.dismiss();
        }
    }

    private async fsckNewCard(
        data: Uint32Array,
        cardWithoutId: Omit<StorageCard, 'id'>,
    ): Promise<[Uint32Array, Omit<StorageCard, 'id'>]> {
        const loader = await this.loadingController.create({ message: 'Checking card...' });
        let fsckContext: FsckContext;
        let result: FsckResult;

        try {
            await loader.present();

            fsckContext = await this.fstools.createFsckContext(cardWithoutId.size);
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
                    'Skip',
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

        if (session.id === this.emulationContext.session()?.id) {
            await this.ejectCurrentCard();
        } else {
            await this.storageService.updateSessionPartial(session.id, { mountedCard: undefined });
            await this.storageService.updateStorageCardPartial(id, { status: StorageCardStatus.dirty });
        }
    }

    private async mountCardInEmulator(cardId: number, data?: Uint32Array): Promise<StorageCard> {
        if (await this.cardIsMounted(cardId)) throw new Error('card already mounted');

        const engine = this.emulationContext.engine();
        if (!engine) {
            throw new Error('emulation engine not initialized');
        }

        const loader = await this.loadingController.create({ message: 'Inserting...' });

        try {
            await loader.present();

            return await this.emulatorLock.runGuarded(async () => {
                await this.vfsService.releaseCard(cardId);

                const session = this.emulationContext.session();
                if (!session) throw new Error('no current session');
                if (session.mountedCard !== undefined) throw new Error('session already has a mounted card');

                const card = await this.loadCardInEmulator(cardId, engine, data);

                await this.storageService.updateStorageCardPartial(cardId, { status: StorageCardStatus.dirty });
                await this.storageService.updateSessionPartial(session.id, { mountedCard: cardId });

                if (!(await engine.mountCard(card.storageId))) {
                    await engine.releaseCard(card.storageId);

                    throw new Error('failed to mount card');
                }

                this.mountCardEvent.dispatch(card);

                return card;
            });
        } finally {
            void loader.dismiss();
        }
    }

    private async mountCardForBrowsing(id: number, readonly: boolean, data?: Uint32Array): Promise<boolean> {
        const loader = await this.loadingController.create({ message: 'Mounting...' });

        try {
            await loader.present();
            return await this.vfsService.mountCardUnchecked(id, readonly, data);
        } finally {
            void loader.dismiss();
        }
    }

    mountCardEvent = new Event<StorageCard>();
    deleteCardEvent = new Event<StorageCard>();

    private loading = true;

    private _cards = signal<Array<StorageCard>>([]);
    private updateMutex = new Mutex();
}
