import { Injectable, NgZone } from '@angular/core';

import { OBJECT_STORE_MEMORY } from './storage/constants';
import { StorageService } from './storage.service';
import { compressPage } from './storage/util';

@Injectable({
    providedIn: 'root',
})
export class StateStorageService {
    constructor(private storageService: StorageService, private ngZone: NgZone) {}

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
}
