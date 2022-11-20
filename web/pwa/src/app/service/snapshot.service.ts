import { E_LOCK_LOST, StorageService } from './storage.service';
import { Injectable, NgZone } from '@angular/core';
import { OBJECT_STORE_MEMORY, OBJECT_STORE_STATE } from './storage/constants';

import { Cloudpilot } from '@common/Cloudpilot';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { Event } from 'microevent.ts';
import { Session } from '@pwa/model/Session';
import { SnapshotStatistics } from '@pwa/model/SnapshotStatistics';
import { compressPage } from './storage/util';

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

        const pageCount = session.totalMemory / 1024;

        this.sessionId = session.id;
        this.dirtyPages = new Uint8Array(pageCount >>> 3);
        this.pages = new Array<Uint32Array>(pageCount);
        this.state = undefined;

        for (let i = 0; i < pageCount; i++) {
            this.pages[i] = new Uint32Array(256);
        }

        this.db = await this.storageService.getDb();
        this.cloudpilot = cloudpilot;
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

    private async triggerSnapshotOnce(): Promise<void> {
        const tx = this.db.transaction([OBJECT_STORE_MEMORY, OBJECT_STORE_STATE], 'readwrite', {
            durability: 'relaxed',
        });

        await this.storageService.acquireLock(tx.objectStore(OBJECT_STORE_STATE), -1);

        if (this.cloudpilot.isSuspended()) return;

        if (this.emulationState.getCurrentSession()?.id !== this.sessionId) throw E_SESSION_MISMATCH;

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
                this.dirtyPages.fill(0);

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

        const dirtyPages = this.cloudpilot.getDirtyPages();
        const memory = this.cloudpilot.getMemory32();
        let pagesSaved = 0;

        for (let i = 0; i < dirtyPages.length; i++) {
            this.dirtyPages[i] |= dirtyPages[i];
            dirtyPages[i] = 0;

            if (this.dirtyPages[i] === 0) {
                iPage += 8;
                continue;
            }

            for (let j = 0; j < 8; j++) {
                if (this.dirtyPages[i] & (1 << j)) {
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

    private dirtyPages!: Uint8Array;
    private pages!: Array<Uint32Array>;
    private cloudpilot!: Cloudpilot;
    private state: Uint8Array | undefined;

    private db!: IDBDatabase;
}
