import { DB_NAME, DB_VERSION, OBJECT_STORE_ROM, OBJECT_STORE_SESSION } from './storage/constants';

import { Injectable } from '@angular/core';
import { PageLockService } from './page-lock.service';
import { RecordRom } from './storage/RecordRom';
import { Session } from 'src/app/model/Session';
import md5 from 'md5';
import { migrate0to1 } from './storage/migrations';

const E_LOCK_LOST = new Error('page lock lost');

function isTransaction(requestOrTransaction: IDBRequest | IDBTransaction): requestOrTransaction is IDBTransaction {
    return !!(requestOrTransaction as IDBTransaction).abort;
}

function complete(transaction: IDBTransaction): Promise<void>;
function complete<T>(request: IDBRequest<T>): Promise<T>;
function complete(requestOrTransaction: IDBRequest | IDBTransaction): Promise<unknown> {
    return new Promise<unknown>((resolve, reject) => {
        if (isTransaction(requestOrTransaction)) {
            requestOrTransaction.oncomplete = () => resolve(undefined);
        } else {
            requestOrTransaction.onsuccess = () => resolve(requestOrTransaction.result);
        }

        requestOrTransaction.onerror = () => reject();
    });
}

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService) {
        this.setupDb();
    }

    public async addSession(session: Session, rom: Uint8Array): Promise<Session> {
        const hash = md5(rom);

        const tx = await this.newTransaction(OBJECT_STORE_SESSION, OBJECT_STORE_ROM);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreSession, -1);

        const recordRom = await complete(objectStoreRom.get(hash));
        if (!recordRom) {
            objectStoreRom.put({ hash, data: rom } as RecordRom);
        }

        const { id, ...sessionSansId } = session;
        const key = await complete(objectStoreSession.add({ ...sessionSansId, rom: hash }));

        const storedSession = await complete(objectStoreSession.get(key));

        await complete(tx);

        return storedSession;
    }

    public async getAllSessions(): Promise<Array<Session>> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);

        await this.acquireLock(objectStoreSession, -1);

        const sessions = await complete(objectStoreSession.getAll());

        return sessions;
    }

    public async deleteSession(session: Session): Promise<void> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION, OBJECT_STORE_ROM);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreSession, -1);

        session = await complete(objectStoreSession.get(session.id));
        if (!session) return;

        objectStoreSession.delete(session.id);

        const sessionsUsingRom = await complete(objectStoreSession.index('rom').getAll(session.rom));

        if (sessionsUsingRom.length === 0) objectStoreRom.delete(session.rom);

        await complete(tx);
    }

    public async updateSession(session: Session): Promise<void> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);

        await this.acquireLock(objectStoreSession, -1);

        const persistentSession = await complete<Session>(objectStoreSession.get(session.id));

        if (!persistentSession) throw new Error(`no session with id ${session.id}`);
        if (persistentSession.rom !== session.rom) throw new Error('attempt to change ROM reference');
        if (persistentSession.ram !== session.ram) throw new Error('attempt to change RAM size');
        if (persistentSession.device !== session.device) throw new Error('attempt to change device type');

        objectStoreSession.put(session);

        await complete(tx);
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

    private async acquireLock(store: IDBObjectStore, key: string | number): Promise<void> {
        await complete(store.get(key));

        if (this.pageLockService.lockLost()) throw E_LOCK_LOST;
    }

    private async newTransaction(...stores: Array<string>): Promise<IDBTransaction> {
        return (await this.db).transaction(stores, 'readwrite');
    }

    private db: Promise<IDBDatabase>;
}
