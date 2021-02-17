import { DB_NAME, DB_VERSION, OBJECT_STORE_ROM, OBJECT_STORE_SESSION } from './storage/constants';

import { Injectable } from '@angular/core';
import { PageLockService } from './page-lock.service';
import { RecordRom } from './storage/RecordRom';
import { Session } from 'src/app/model/Session';
import md5 from 'md5';
import { migrate0to1 } from './storage/migrations';

const E_LOCK_LOST = new Error('page lock lost');

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService) {
        this.setupDb();
    }

    public async addSession(session: Session, rom: Uint8Array): Promise<Session> {
        const hash = md5(rom);
        const tx = (await this.db).transaction([OBJECT_STORE_SESSION, OBJECT_STORE_ROM], 'readwrite');
        let storedSession: Session;

        return new Promise<Session>(async (resolve, reject) => {
            tx.oncomplete = () => resolve(storedSession);
            tx.onabort = () => reject();

            await this.acquireLock(tx, OBJECT_STORE_SESSION, -1);

            const recordRom = await this.getOne<RecordRom>(tx, OBJECT_STORE_ROM, hash);
            if (!recordRom) {
                tx.objectStore(OBJECT_STORE_ROM).put({ hash, data: rom } as RecordRom);
            }

            const { id, ...sessionSansId } = session;
            // tslint:disable-next-line: no-shadowed-variable
            const key = await new Promise<number>((resolve, reject) => {
                const request = tx.objectStore(OBJECT_STORE_SESSION).add({ ...sessionSansId, rom: hash });

                request.onsuccess = () => resolve(request.result as number);
                request.onerror = () => reject();
            });

            storedSession = await this.getOne<Session>(tx, OBJECT_STORE_SESSION, key);
        });
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

    private getOne<T>(tx: IDBTransaction, store: string, key: string | number): Promise<T | undefined> {
        return new Promise<T>((resolve, reject) => {
            const request = tx.objectStore(store).get(key);

            request.onsuccess = (e) => resolve(request.result);
            request.onerror = () => reject();
        });
    }

    private async acquireLock(tx: IDBTransaction, store: string, key: string | number): Promise<void> {
        await this.getOne(tx, store, key);

        if (this.pageLockService.lockLost()) throw E_LOCK_LOST;
    }

    private db: Promise<IDBDatabase>;
}
