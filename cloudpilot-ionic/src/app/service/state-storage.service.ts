import { OBJECT_STORE_MEMORY, OBJECT_STORE_STATE } from './storage/constants';

import { Injectable } from '@angular/core';
import { StorageService } from './storage.service';
import { complete } from './storage/util';

interface StorageServiceInterface {}

function compressPage(page: Uint8Array): Uint8Array | number {
    for (let i = 1; i < 1024; i++) {
        if (page[i] !== page[0]) return page;
    }

    return page[0];
}

@Injectable({
    providedIn: 'root',
})
export class StateStorageService {
    constructor() {}

    setStorageService(storageService: StorageServiceInterface): void {
        this.storageService = storageService;
    }

    saveState(tx: IDBTransaction, sessionId: number, state: Uint8Array | undefined): void {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        if (state) {
            objectStore.put(state, sessionId);
        } else {
            objectStore.delete(sessionId);
        }
    }

    deleteState(tx: IDBTransaction, sessionId: number): void {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        objectStore.delete(sessionId);
    }

    loadState(tx: IDBTransaction, sessionId: number): Promise<Uint8Array | undefined> {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        return complete(objectStore.get(sessionId));
    }

    saveMemory(tx: IDBTransaction, sessionId: number, memory: Uint8Array): void {
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);

        objectStore.delete(IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true));

        const pageCount = memory.length >>> 10;

        for (let i = 0; i < pageCount; i++) {
            const compressedPage = compressPage(memory.subarray(i * 1024, (i + 1) * 1024));

            if (typeof compressedPage === 'number') {
                objectStore.put(compressedPage, [sessionId, i]);
            } else {
                const page = new Uint8Array(1024);
                page.set(compressedPage);

                objectStore.put(page, [sessionId, i]);
            }
        }
    }

    deleteMemory(tx: IDBTransaction, sessionId: number): void {
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);

        objectStore.delete(IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true));
    }

    loadMemory(tx: IDBTransaction, sessionId: number, size: number): Promise<Uint8Array> {
        return new Promise((resolve, reject) => {
            const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);
            const memory = new Uint8Array(size);

            const request = objectStore.openCursor(IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true));

            request.onsuccess = () => {
                const cursor = request.result;

                if (!cursor) return resolve(memory);

                const compressedPage = cursor.value;
                const [, iPage] = cursor.key as [number, number];

                if (typeof compressedPage === 'number') {
                    memory.subarray(iPage * 1024, (iPage + 1) * 1024).fill(compressedPage);
                } else {
                    memory.subarray(iPage * 1024, (iPage + 1) * 1024).set(compressedPage);
                }

                cursor.continue();
            };

            request.onerror = () => reject(new Error('failed to load memory image'));
        });
    }

    private saveDirtyMemory(tx: IDBTransaction, sessionId: number, memory: Uint8Array, dirtyPages: Uint8Array): void {
        let iPage = 0;
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);

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
                        objectStore.put(compressPage, [sessionId, iPage]);
                    } else {
                        this.pages[iPage].set(compressedPage);

                        objectStore.put(compressPage(this.pages[iPage]), [sessionId, iPage]);
                    }
                }

                iPage++;
            }
        }
    }

    private dirtyPages!: Uint8Array;
    private pages!: Array<Uint8Array>;
    private storageService!: StorageServiceInterface;
}
