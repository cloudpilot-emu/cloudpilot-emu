import { Injectable, NgZone } from '@angular/core';
import { Snapshot, SnapshotContainer } from '@common/engine/Snapshot';
import { DynamicTimeout } from '@common/helper/dynamicTimeout';
import { LoadingController } from '@ionic/angular';
import { Event } from 'microevent.ts';

import { MemoryMetadata } from '@pwa//model/MemoryMetadata';
import { Session } from '@pwa/model/Session';
import { StorageCard } from '@pwa/model/StorageCard';

import { AlertService } from './alert.service';
import { EmulationContextService } from './emulation-context.service';
import { ErrorService } from './error.service';
import { E_LOCK_LOST, StorageService } from './storage.service';
import {
    OBJECT_STORE_LOCK,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_MEMORY_META,
    OBJECT_STORE_NAND,
    OBJECT_STORE_STATE,
    OBJECT_STORE_STORAGE,
    OBJECT_STORE_STORAGE_CARD,
} from './storage/constants';
import { complete, compressPage, compressPage8 } from './storage/util';

declare global {
    interface IDBDatabase {
        transaction(
            storeNames: string | string[],
            mode?: IDBTransactionMode,
            options?: { durability?: 'default' | 'strict' | 'relaxed' },
        ): IDBTransaction;
    }
}

class ClonePool {
    clear(): void {
        this.pages = [];
        this.nextPage = -1;
    }

    reset(): void {
        this.nextPage = this.pages.length - 1;
    }

    duplicate(page: number | Uint32Array): typeof page {
        if (typeof page === 'number') return page;

        if (this.nextPage >= 0) {
            const clone = this.pages[this.nextPage--];
            clone.set(page);

            return clone;
        }

        const clone = page.slice();
        this.pages.push(clone);

        return clone;
    }

    private pages: Array<Uint32Array> = [];
    private nextPage = -1;
}

const STATIC_TIMEOUT_MSEC = 500;
const TIMEOUT_DELTA_MSEC_PER_KB_DEFAULT = 0.5;
const TIMEOUT_DELTA_MSEC_PER_KB_MAX = 8 * TIMEOUT_DELTA_MSEC_PER_KB_DEFAULT;
const MAX_CONSECUTIVE_ERRORS = 3;

const E_SESSION_MISMATCH = new Error('session does not match emulation');

function snapshotSizeBytes(snapshot: Snapshot | undefined): number {
    return snapshot ? snapshot.pageSize * snapshot.pageCount : 0;
}

@Injectable({
    providedIn: 'root',
})
export class SnapshotService {
    constructor(
        private storageService: StorageService,
        private emulationContext: EmulationContextService,
        private errorService: ErrorService,
        private alertService: AlertService,
        private loadingController: LoadingController,
        private ngZone: NgZone,
    ) {}

    async initialize(session: Session): Promise<void> {
        if (this.errorService.hasFatalError()) return;

        await this.waitForPendingSnapshot();

        this.sessionId = session.id;
        this.clonePoolMemory.clear();
        this.clonePoolNand.clear();
        this.clonePoolStorage.clear();
        this.savestateClone = undefined;

        this.db = await this.storageService.getDb();
    }

    storeSnapshot = (snapshotContainer: SnapshotContainer): Promise<void> =>
        this.ngZone.runOutsideAngular(async () => {
            if (this.sessionId < 0) return;

            if (this.snapshotInProgress) throw new Error('cannot happen: snapshot while snapshot is in progress');

            this.snapshotInProgress = true;

            this.pendingSnapshotPromise = this.storeSnapshotUnguarded(snapshotContainer).finally(
                () => (this.snapshotInProgress = false),
            );

            return this.pendingSnapshotPromise;
        });

    triggerSnapshot(): Promise<void> {
        return new Promise((resolve, reject) =>
            this.snapshotRequestEvent.dispatch((error) => {
                if (error !== undefined) reject(error);
                else resolve();
            }),
        );
    }

    waitForPendingSnapshot(): Promise<void> {
        if (this.snapshotInProgress) return this.pendingSnapshotPromise;
        if (this.errorService.hasFatalError()) return Promise.reject();

        return Promise.resolve();
    }

    private async storeSnapshotUnguarded(snapshotContainer: SnapshotContainer): Promise<void> {
        this.timeSync = this.timeBackground = 0;
        this.timeoutDeltaMsecPerKb = this.timeoutDeltaMsecPerKbStart;

        while (true) {
            try {
                await snapshotContainer.release(
                    await this.attemptToStoreSnapshot(snapshotContainer),
                    this.timeSync,
                    this.timeBackground,
                );
                this.consecutiveErrorCount = 0;

                this.hideLoader();

                return;
            } catch (e) {
                this.hideLoader();

                if (e instanceof DynamicTimeout) {
                    if (e.isTainted()) continue;

                    console.log(`snapshot timeout expired after ${e.getAccumulatedTimeout()} msec`);

                    if (this.timeoutDeltaMsecPerKb > TIMEOUT_DELTA_MSEC_PER_KB_DEFAULT) {
                        await this.alertService.snapshotTimeout();
                        await this.showLoader();
                    }

                    this.timeoutDeltaMsecPerKb *= 2;
                    this.timeoutDeltaMsecPerKbStart = Math.min(
                        2 * this.timeoutDeltaMsecPerKbStart,
                        TIMEOUT_DELTA_MSEC_PER_KB_MAX,
                    );

                    continue;
                }

                console.error('snapshot error', e);

                if (e === E_SESSION_MISMATCH) {
                    this.errorService.fatalBug('attempting to snapshot a session that is not currently running');

                    throw e;
                }

                if (e === E_LOCK_LOST) {
                    this.errorService.fatalPageLockLost();

                    throw e;
                }

                if (++this.consecutiveErrorCount > MAX_CONSECUTIVE_ERRORS) {
                    this.errorService.fatalIDBDead();

                    throw e;
                }
            }
        }
    }

    private async attemptToStoreSnapshot(snapshotContainer: SnapshotContainer): Promise<boolean> {
        const tx = this.db.transaction(
            [
                OBJECT_STORE_MEMORY,
                OBJECT_STORE_MEMORY_META,
                OBJECT_STORE_STATE,
                OBJECT_STORE_STORAGE,
                OBJECT_STORE_STORAGE_CARD,
                OBJECT_STORE_NAND,
                OBJECT_STORE_LOCK,
            ],
            'readwrite',
            {
                durability: 'relaxed',
            },
        );

        await this.storageService.acquireLock(tx);

        const storageCards: Array<StorageCard> = await complete(tx.objectStore(OBJECT_STORE_STORAGE_CARD).getAll());

        this.assertSessionMatches();

        if (snapshotContainer.materialize()) return false;

        let timestampBlockingStart = 0;
        let timestampBlockingEnd = 0;

        return new Promise<boolean>((resolve, reject) => {
            let isTimeout = false;

            this.clonePoolMemory.reset();
            this.clonePoolNand.reset();
            this.clonePoolStorage.reset();

            const snapshotMemory = snapshotContainer.getSnapshotMemory();
            const snapshotNand = snapshotContainer.getSnapshotNand();
            const snapshotStorage = snapshotContainer.getSnapshotStorage();
            const savestate = snapshotContainer.getSavestate();
            const storageKey = snapshotContainer.getStorageKey();
            const memoryCrc = snapshotContainer.getMemoryCrc();

            const totalSizeBytes =
                snapshotSizeBytes(snapshotMemory) +
                snapshotSizeBytes(snapshotNand) +
                snapshotSizeBytes(snapshotStorage) +
                (savestate?.length ?? 0);

            const timeoutMsec = STATIC_TIMEOUT_MSEC + ((this.timeoutDeltaMsecPerKb * totalSizeBytes) >>> 10);
            const timeout = new DynamicTimeout(timeoutMsec, () => {
                isTimeout = true;
                reject(timeout);

                try {
                    tx.abort();
                    // eslint-disable-next-line @typescript-eslint/no-unused-vars
                } catch (e) {
                    // this most likely means the transaction is already aborted, so never mind
                }
            });

            tx.oncomplete = () => {
                timeout.cancel();
                this.timeBackground += timeout.getAccumulatedTimeout();

                resolve(true);
            };

            tx.onerror = () => {
                timeout.cancel();
                this.timeBackground += timeout.getAccumulatedTimeout();

                reject(new Error(tx.error?.message));
            };

            try {
                timestampBlockingStart = performance.now();

                this.saveSnapshotMemory(tx, snapshotMemory, memoryCrc);

                if (snapshotNand) {
                    this.saveSnapshotOther(tx, snapshotNand, this.sessionId, OBJECT_STORE_NAND, this.clonePoolNand);
                }

                if (snapshotStorage && storageKey !== undefined) {
                    const storageCard = storageCards.find((card) => card.storageId === storageKey);
                    if (!storageCard) throw new Error(`no storage card with key ${storageKey}`);

                    this.saveSnapshotOther(
                        tx,
                        snapshotStorage,
                        storageCard.id,
                        OBJECT_STORE_STORAGE,
                        this.clonePoolMemory,
                    );
                }

                this.saveSavestate(tx, savestate);

                timeout.start();
                timestampBlockingEnd = performance.now();

                this.timeSync += timestampBlockingEnd - timestampBlockingStart;
            } catch (e) {
                if (!isTimeout) {
                    timeout.cancel();
                    tx.abort();
                }

                throw e;
            }
        });
    }

    private saveSnapshotMemory(tx: IDBTransaction, snapshot: Snapshot, crc: number | undefined) {
        const { pageCount, pageSize, offsets, pageData, pageCountTotal, pages } = snapshot;

        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);
        const objectStoreMeta = tx.objectStore(OBJECT_STORE_MEMORY_META);

        const metadata: MemoryMetadata = {
            sessionId: this.sessionId,
            totalSize: (pageCountTotal * pageSize) << 2,
            crc32: crc,
        };

        objectStoreMeta.put(metadata);

        for (let iPage = 0; iPage < pageCount; iPage++) {
            const page = pageData.subarray(offsets[iPage], offsets[iPage] + pageSize);
            const compressedPage = this.clonePoolMemory.duplicate(compressPage8(page));

            objectStore.put(compressedPage, [this.sessionId, pages[iPage]]);
        }
    }

    private saveSnapshotOther(tx: IDBTransaction, snapshot: Snapshot, id: number, store: string, clonePool: ClonePool) {
        const { pageCount, pageSize, offsets, pageData, pages } = snapshot;

        const objectStore = tx.objectStore(store);

        for (let iPage = 0; iPage < pageCount; iPage++) {
            const page = pageData.subarray(offsets[iPage], offsets[iPage] + pageSize);
            const compressedPage = clonePool.duplicate(compressPage(page));

            objectStore.put(compressedPage, [id, pages[iPage]]);
        }
    }

    private saveSavestate(tx: IDBTransaction, savestate: Uint8Array | undefined) {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        if (savestate) {
            if (!this.savestateClone) {
                this.savestateClone = savestate.slice();
            } else {
                this.savestateClone.set(savestate);
            }

            objectStore.put(this.savestateClone, this.sessionId);
        } else {
            objectStore.delete(this.sessionId);
        }
    }

    private assertSessionMatches(): void {
        if (this.emulationContext.session()?.id !== this.sessionId) throw E_SESSION_MISMATCH;
    }

    private async showLoader(): Promise<void> {
        if (this.loader) return;

        this.loader = await this.loadingController.create({ message: 'Saving...' });
        await this.loader.present();
    }

    private hideLoader(): void {
        if (!this.loader) return;

        void this.loader.dismiss();

        this.loader = undefined;
    }

    snapshotRequestEvent = new Event<(error?: Error) => void>();

    private sessionId = -1;
    private consecutiveErrorCount = 0;
    private timeoutDeltaMsecPerKbStart = TIMEOUT_DELTA_MSEC_PER_KB_DEFAULT;
    private timeoutDeltaMsecPerKb = TIMEOUT_DELTA_MSEC_PER_KB_DEFAULT;
    private timeSync = 0;
    private timeBackground = 0;
    private snapshotInProgress = false;
    private loader: HTMLIonLoadingElement | undefined;
    private pendingSnapshotPromise = Promise.resolve<void>(undefined);

    private clonePoolMemory = new ClonePool();
    private clonePoolNand = new ClonePool();
    private clonePoolStorage = new ClonePool();
    private savestateClone: Uint8Array | undefined;

    private db!: IDBDatabase;
}
