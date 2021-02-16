import { DB_NAME, DB_VERSION } from './storage/constants';

import { Injectable } from '@angular/core';
import { PageLockService } from './page-lock.service';
import { Session } from 'src/app/model/Session';
import { migrate0to1 } from './storage/migrations';

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService) {
        this.setupDb();
    }

    private setupDb() {
        this.db = new Promise((resolve, reject) => {
            const request = indexedDB.open(DB_NAME, DB_VERSION);

            request.onerror = () => reject(new Error('failed to open DB'));
            request.onsuccess = () => resolve(request.result);
            request.onupgradeneeded = (e) => {
                if (e.oldVersion < 1) {
                    migrate0to1(request.result);
                }
            };
        });
    }

    private db: Promise<IDBDatabase>;
}
