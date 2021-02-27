import { E_LOCK_LOST, StorageService } from './storage.service';
import { Injectable, NgZone } from '@angular/core';
import { OBJECT_STORE_MEMORY, OBJECT_STORE_STATE } from './storage/constants';

import { AlertService } from 'src/app/service/alert.service';
import { Cloudpilot } from './../helper/Cloudpilot';
import { EmulationStateService } from './emulation-state.service';
import { PageLockService } from './page-lock.service';
import { Session } from './../model/Session';
import { compressPage } from './storage/util';

const TIMEOUT_MSEC = 1000;
const MAX_CONSECUTIVE_ERRORS = 3;

const E_TIMEOUT = new Error('transaction timeout');
const E_SESSION_MISMATCH = new Error('session does not match emulation');

export const enum Health {
    healthy,
    errors,
    defunct,
}

@Injectable({
    providedIn: 'root',
})
export class SnapshotService {
    constructor(
        private storageService: StorageService,
        private pageLockService: PageLockService,
        private emulationState: EmulationStateService,
        private alertService: AlertService,
        private ngZone: NgZone
    ) {}

    async initialize(session: Session, cloudpilot: Cloudpilot): Promise<void> {
        if (this.defunct || this.pageLockService.lockLost()) return;

        const pageCount = session.ram * 1024;

        this.sessionId = session.id;
        this.dirtyPages = new Uint8Array(pageCount >> 3);
        this.pages = new Array<Uint8Array>(pageCount);
        this.state = undefined;

        for (let i = 0; i < pageCount; i++) {
            this.pages[i] = new Uint8Array(1024);
        }

        this.db = await this.storageService.getDb();
        this.cloudpilot = cloudpilot;
    }

    getHealth(): Health {
        if (this.defunct) return Health.defunct;
        if (this.consecutiveErrorCount > 0) return Health.errors;

        return Health.healthy;
    }

    triggerSnapshot(): Promise<void> {
        if (this.sessionId < 0) return Promise.resolve();

        if (this.snapshotInProgress) return this.pendingSnapshotPromise;
        if (this.defunct) return Promise.reject();

        this.snapshotInProgress = true;

        this.pendingSnapshotPromise = (async () => {
            try {
                await this.triggerSnapshotUnguarded();
            } finally {
                this.snapshotInProgress = false;
            }
        })();

        return this.pendingSnapshotPromise;
    }

    waitForPendingSnapshot(): Promise<void> {
        if (this.snapshotInProgress) return this.pendingSnapshotPromise;
        if (this.defunct) return Promise.reject();

        return Promise.resolve();
    }

    private async triggerSnapshotUnguarded(): Promise<void> {
        while (true) {
            try {
                await this.triggerSnapshotOnce();

                if (this.consecutiveErrorCount !== 0) this.ngZone.run(() => (this.consecutiveErrorCount = 0));

                return;
            } catch (e) {
                console.error('snapshot error', e);

                if (e === E_SESSION_MISMATCH) throw e;

                if (e === E_LOCK_LOST) {
                    this.ngZone.run(() => (this.defunct = true));

                    throw e;
                }

                if (++this.consecutiveErrorCount > MAX_CONSECUTIVE_ERRORS) {
                    this.ngZone.run(() => (this.defunct = true));

                    await this.alertService.fatalError('IndexedDB access lost. This is most likely a browser bug.');

                    throw e;
                }
            }
        }
    }

    private async triggerSnapshotOnce(): Promise<void> {
        const tx = this.db.transaction([OBJECT_STORE_MEMORY, OBJECT_STORE_STATE], 'readwrite');

        await this.storageService.acquireLock(tx.objectStore(OBJECT_STORE_STATE), -1);

        if (this.emulationState.getCurrentSession()?.id !== this.sessionId) throw E_SESSION_MISMATCH;

        await new Promise<void>((resolve, reject) => {
            let isTimeout = false;

            const timeout = setTimeout(() => {
                isTimeout = true;
                tx.abort();

                reject(E_TIMEOUT);
            }, TIMEOUT_MSEC);

            tx.oncomplete = () => {
                clearTimeout(timeout);
                this.dirtyPages.fill(0);

                resolve();
            };

            tx.onerror = () => {
                clearTimeout(timeout);

                reject(new Error(tx.error?.message));
            };

            try {
                this.saveDirtyMemory(tx);
                this.saveSession(tx);
            } catch (e) {
                if (!isTimeout) {
                    clearTimeout(timeout);
                    tx.abort();
                }

                throw e;
            }
        });
    }

    private saveDirtyMemory(tx: IDBTransaction): void {
        let iPage = 0;
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);

        const dirtyPages = this.cloudpilot.getDirtyPages();
        const memory = this.cloudpilot.getMemory();

        for (let i = 0; i < dirtyPages.length; i++) {
            this.dirtyPages[i] |= dirtyPages[i];
            dirtyPages[i] = 0;

            if (this.dirtyPages[i] === 0) {
                iPage += 8;
                continue;
            }

            for (let j = 0; j < 8; j++) {
                if (this.dirtyPages[i] & (1 << j)) {
                    const compressedPage = compressPage(memory.subarray(iPage * 1024, (iPage + 1) * 1024));

                    if (typeof compressedPage === 'number') {
                        objectStore.put(compressedPage, [this.sessionId, iPage]);
                    } else {
                        this.pages[iPage].set(compressedPage);

                        objectStore.put(this.pages[iPage], [this.sessionId, iPage]);
                    }
                }

                iPage++;
            }
        }
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

    private defunct = false;
    private sessionId = -1;
    private consecutiveErrorCount = 0;
    private snapshotInProgress = false;
    private pendingSnapshotPromise = Promise.resolve<void>(undefined);

    private dirtyPages!: Uint8Array;
    private pages!: Array<Uint8Array>;
    private cloudpilot!: Cloudpilot;
    private state: Uint8Array | undefined;

    private db!: IDBDatabase;
}
