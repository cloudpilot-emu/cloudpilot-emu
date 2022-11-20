import { E_LOCK_LOST, StorageService } from './storage.service';
import {
    INDEX_CARD_STORAGE_ID,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_MEMORY_META,
    OBJECT_STORE_STATE,
    OBJECT_STORE_STORAGE,
    OBJECT_STORE_STORAGE_CARD,
} from './storage/constants';
import { Injectable, NgZone } from '@angular/core';
import { complete, compressPage } from './storage/util';

import { Cloudpilot } from '@common/Cloudpilot';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { Event } from 'microevent.ts';
import { MemoryMetadata } from './../model/MemoryMetadata';
import { Session } from '@pwa/model/Session';
import { SnapshotStatistics } from '@pwa/model/SnapshotStatistics';
import { StorageCard } from './../model/StorageCard';

declare global {
    interface IDBDatabase {
        transaction(
            storeNames: string | string[],
            mode?: IDBTransactionMode,
            options?: { durability?: 'default' | 'strict' | 'relaxed' }
        ): IDBTransaction;
    }
}

const TIMEOUT_MSEC = 6000;
const MAX_CONSECUTIVE_ERRORS = 3;

const E_TIMEOUT = new Error('transaction timeout');
const E_SESSION_MISMATCH = new Error('session does not match emulation');
@Injectable({
    providedIn: 'root',
})
export class SnapshotService {
    constructor(
        private storageService: StorageService,
        private emulationState: EmulationStateService,
        private errorService: ErrorService,
        private ngZone: NgZone
    ) {}

    async initialize(session: Session, cloudpilot: Cloudpilot): Promise<void> {
        if (this.errorService.hasFatalError()) return;

        this.sessionId = session.id;
        this.dirtyPages = undefined;
        this.pages = undefined;
        this.state = undefined;

        this.db = await this.storageService.getDb();
        this.cloudpilot = cloudpilot;

        this.resetCard();
    }

    resetCard(): void {
        this.cardPagePool = [];
        this.cardDirtyPages = undefined;
    }

    triggerSnapshot = (): Promise<void> =>
        this.ngZone.runOutsideAngular(() => {
            if (this.sessionId < 0 || this.cloudpilot.isSuspended()) return Promise.resolve();

            if (this.snapshotInProgress) return this.pendingSnapshotPromise;
            if (this.errorService.hasFatalError()) return Promise.reject();

            this.snapshotInProgress = true;

            this.pendingSnapshotPromise = (async () => {
                try {
                    await this.triggerSnapshotUnguarded();
                } finally {
                    this.snapshotInProgress = false;
                }
            })();

            return this.pendingSnapshotPromise;
        });

    waitForPendingSnapshot(): Promise<void> {
        if (this.snapshotInProgress) return this.pendingSnapshotPromise;
        if (this.errorService.hasFatalError()) return Promise.reject();

        return Promise.resolve();
    }

    private async triggerSnapshotUnguarded(): Promise<void> {
        while (true) {
            try {
                await this.triggerSnapshotOnce();
                this.consecutiveErrorCount = 0;

                return;
            } catch (e) {
                console.error('snapshot error', e);

                if (e === E_SESSION_MISMATCH) {
                    this.errorService.fatalBug('attempting to snapshot a session that is not currently running');

                    throw e;
                }

                if (e === E_LOCK_LOST) throw e;

                if (++this.consecutiveErrorCount > MAX_CONSECUTIVE_ERRORS) {
                    this.errorService.fatalIDBDead();

                    throw e;
                }
            }
        }
    }

    private async triggerSnapshotOnce(): Promise<void> {
        const tx = this.db.transaction(
            [
                OBJECT_STORE_MEMORY,
                OBJECT_STORE_MEMORY_META,
                OBJECT_STORE_STATE,
                OBJECT_STORE_STORAGE,
                OBJECT_STORE_STORAGE_CARD,
            ],
            'readwrite',
            {
                durability: 'relaxed',
            }
        );

        await this.storageService.acquireLock(tx.objectStore(OBJECT_STORE_STATE), -1);

        if (this.cloudpilot.isSuspended()) return;

        if (this.emulationState.getCurrentSession()?.id !== this.sessionId) throw E_SESSION_MISMATCH;

        let storageCard: StorageCard | undefined;
        const storageId = this.cloudpilot.getMountedKey();

        if (storageId) {
            storageCard = await complete(
                tx.objectStore(OBJECT_STORE_STORAGE_CARD).index(INDEX_CARD_STORAGE_ID).get(storageId)
            );
        }

        const statistics: SnapshotStatistics = await new Promise((resolve, reject) => {
            let isTimeout = false;

            const timestampStart = performance.now();
            let timestampBlockingStart = 0;
            let timestampBlockingEnd = 0;
            let pages = 0;

            const timeout = setTimeout(() => {
                isTimeout = true;

                try {
                    tx.abort();
                } catch (e) {}

                reject(E_TIMEOUT);
            }, TIMEOUT_MSEC);

            tx.oncomplete = () => {
                clearTimeout(timeout);
                this.dirtyPages?.fill(0);
                this.cardDirtyPages?.fill(0);

                resolve({
                    timestamp: Date.now(),
                    pages,
                    timeBlocking: timestampBlockingEnd - timestampBlockingStart,
                    timeTotal: performance.now() - timestampStart,
                });
            };

            tx.onerror = () => {
                clearTimeout(timeout);

                reject(new Error(tx.error?.message));
            };

            try {
                timestampBlockingStart = performance.now();

                pages = this.saveDirtyMemory(tx);
                if (storageCard) {
                    pages += this.saveDirtyStorage(storageCard, tx);
                }
                this.saveSession(tx);

                timestampBlockingEnd = performance.now();
            } catch (e) {
                if (!isTimeout) {
                    clearTimeout(timeout);

                    tx.abort();
                }

                throw e;
            }
        });

        this.snapshotEvent.dispatch(statistics);
    }

    private saveDirtyMemory(tx: IDBTransaction): number {
        let iPage = 0;
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);
        const objectStoreMeta = tx.objectStore(OBJECT_STORE_MEMORY_META);

        const dirtyPages = this.cloudpilot.getDirtyPages();
        const memory = this.cloudpilot.getMemory32();
        let pagesSaved = 0;

        if (!this.dirtyPages || !this.pages) {
            const pageCount = memory.length >>> 8;

            this.dirtyPages = new Uint8Array(dirtyPages.length);
            this.pages = new Array<Uint32Array>(pageCount);

            for (let i = 0; i < pageCount; i++) {
                this.pages[i] = new Uint32Array(256);
            }
        }

        const metadata: MemoryMetadata = {
            sessionId: this.sessionId,
            totalSize: memory.length << 2,
        };
        objectStoreMeta.put(metadata);

        for (let i = 0; i < dirtyPages.length; i++) {
            this.dirtyPages[i] |= dirtyPages[i];
            dirtyPages[i] = 0;

            if (this.dirtyPages[i] === 0) {
                iPage += 8;
                continue;
            }

            for (let mask = 1; mask < 0x0100; mask <<= 1) {
                if (iPage >= this.pages.length) break;

                if (this.dirtyPages[i] & mask) {
                    const compressedPage = compressPage(memory.subarray(iPage * 256, (iPage + 1) * 256));

                    if (typeof compressedPage === 'number') {
                        objectStore.put(compressedPage, [this.sessionId, iPage]);
                    } else {
                        this.pages[iPage].set(compressedPage);

                        objectStore.put(this.pages[iPage], [this.sessionId, iPage]);
                    }

                    pagesSaved++;
                }

                iPage++;
            }
        }

        return pagesSaved;
    }

    private saveDirtyStorage(card: StorageCard, tx: IDBTransaction): number {
        const dirtyPages = this.cloudpilot.getCardDirtyPages(card.storageId);
        const data = this.cloudpilot.getCardData(card.storageId);

        if (!data) {
            console.log(`seems ${card.name} was unmounted`);
            return 0;
        }

        if (!dirtyPages) throw new Error(`failed to retrieve dirty pages for card ${card.name}`);
        if (data.length !== card.size >>> 2) throw new Error('card size mismatch');

        if (!this.cardDirtyPages) this.cardDirtyPages = new Uint8Array(dirtyPages.length);

        if (this.cardDirtyPages.length !== dirtyPages.length) {
            throw new Error(`card size has changed without notification`);
        }

        let iPage = 0;
        let iPool = 0;
        let pagesSaved = 0;
        const objectStore = tx.objectStore(OBJECT_STORE_STORAGE);
        const pagesTotal = (data.length >> 11) + (data.length % 2048 > 0 ? 1 : 0);

        for (let i = 0; i < dirtyPages.length; i++) {
            this.cardDirtyPages[i] |= dirtyPages[i];
            dirtyPages[i] = 0;

            if (this.cardDirtyPages[i] === 0) {
                iPage += 8;
                continue;
            }

            for (let mask = 1; mask < 0x0100; mask <<= 1) {
                if (iPage >= pagesTotal) break;

                if (this.cardDirtyPages[i] & mask) {
                    if (iPool >= this.cardPagePool.length) {
                        this.cardPagePool.push(new Uint32Array(2048));
                    }

                    const page = this.cardPagePool[iPool++];
                    page.set(data.subarray(iPage * 2048, (iPage + 1) * 2048));

                    objectStore.put(page, [card.id, iPage]);
                    pagesSaved++;
                }

                iPage++;
            }
        }

        return pagesSaved * 8;
    }

    private saveSession(tx: IDBTransaction) {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        if (this.cloudpilot.saveState()) {
            const state = this.cloudpilot.getSavestate();

            if (!this.state) this.state = new Uint8Array(state.length);

            this.state.set(state);

            objectStore.put(this.state, this.sessionId);
        } else {
            console.log('save state failed');
            objectStore.delete(this.sessionId);
        }
    }

    snapshotEvent = new Event<SnapshotStatistics>();

    private sessionId = -1;
    private consecutiveErrorCount = 0;
    private snapshotInProgress = false;
    private pendingSnapshotPromise = Promise.resolve<void>(undefined);

    private dirtyPages: Uint8Array | undefined;
    private pages: Array<Uint32Array> | undefined;
    private cloudpilot!: Cloudpilot;
    private state: Uint8Array | undefined;

    private cardPagePool: Array<Uint32Array> = [];
    private cardDirtyPages: Uint8Array | undefined;

    private db!: IDBDatabase;
}
