import {
    DB_NAME,
    DB_VERSION,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_ROM,
    OBJECT_STORE_SESSION,
    OBJECT_STORE_STATE,
} from './storage/constants';
import { migrate0to1, migrate1to2 } from './storage/migrations';

import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { PageLockService } from './page-lock.service';
import { Session } from 'src/app/model/Session';
import { StateStorageService } from './state-storage.service';
import { complete } from './storage/util';
import md5 from 'md5';

const E_LOCK_LOST = new Error('page lock lost');

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService, private stateStorageService: StateStorageService) {
        this.setupDb();
        this.stateStorageService.setStorageService(this);
    }

    public async addSession(session: Session, rom: Uint8Array, ram?: Uint8Array, state?: Uint8Array): Promise<Session> {
        const hash = md5(rom);

        const tx = await this.newTransaction(
            OBJECT_STORE_SESSION,
            OBJECT_STORE_ROM,
            OBJECT_STORE_STATE,
            OBJECT_STORE_MEMORY
        );
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreSession, -1);

        const recordRom = await complete(objectStoreRom.get(hash));
        if (!recordRom) {
            objectStoreRom.put(rom, hash);
        }

        const { id, ...sessionSansId } = session;
        const key = await complete(objectStoreSession.add({ ...sessionSansId, rom: hash }));

        const storedSession = await complete<Session>(objectStoreSession.get(key));

        if (ram) {
            this.stateStorageService.saveMemory(tx, storedSession.id, ram);
        }

        if (state) {
            this.stateStorageService.saveState(tx, storedSession.id, state);
        }

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

    public async getSession(id: number): Promise<Session | undefined> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);

        await this.acquireLock(objectStoreSession, -1);

        return await complete(objectStoreSession.get(id));
    }

    public async deleteSession(session: Session): Promise<void> {
        const tx = await this.newTransaction(
            OBJECT_STORE_SESSION,
            OBJECT_STORE_ROM,
            OBJECT_STORE_STATE,
            OBJECT_STORE_MEMORY
        );
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreSession, -1);

        session = await complete(objectStoreSession.get(session.id));
        if (!session) return;

        objectStoreSession.delete(session.id);

        const sessionsUsingRom = await complete(objectStoreSession.index('rom').getAll(session.rom));

        if (sessionsUsingRom.length === 0) objectStoreRom.delete(session.rom);

        this.stateStorageService.deleteMemory(tx, session.id);
        this.stateStorageService.deleteState(tx, session.id);

        await complete(tx);

        this.sessionChange.dispatch(session.id);
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

        this.sessionChange.dispatch(session.id);
    }

    public async loadSession(session: Session): Promise<[Uint8Array, Uint8Array | undefined, Uint8Array | undefined]> {
        const tx = await this.newTransaction(OBJECT_STORE_ROM, OBJECT_STORE_STATE, OBJECT_STORE_MEMORY);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreRom, -1);

        return [
            await complete<Uint8Array>(objectStoreRom.get(session.rom)),
            await this.stateStorageService.loadMemory(tx, session.id, session.ram * 1024 * 1024),
            await this.stateStorageService.loadState(tx, session.id),
        ];
    }

    private async acquireLock(store: IDBObjectStore, key: string | number): Promise<void> {
        await complete(store.get(key));

        if (this.pageLockService.lockLost()) throw E_LOCK_LOST;
    }

    private async newTransaction(...stores: Array<string>): Promise<IDBTransaction> {
        return (await this.db).transaction(stores, 'readwrite');
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

                if (e.oldVersion < 2) {
                    migrate1to2(request.result, request.transaction);
                }
            };
        });
    }

    public sessionChange = new Event<number>();

    private db!: Promise<IDBDatabase>;
}
