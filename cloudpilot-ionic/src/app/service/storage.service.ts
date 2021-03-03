import {
    DB_NAME,
    DB_VERSION,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_ROM,
    OBJECT_STORE_SESSION,
    OBJECT_STORE_STATE,
} from './storage/constants';
import { Injectable, NgZone } from '@angular/core';
import { complete, compressPage } from './storage/util';
import { migrate0to1, migrate1to2 } from './storage/migrations';

import { Event } from 'microevent.ts';
import { PageLockService } from './page-lock.service';
import { Session } from 'src/app/model/Session';
import md5 from 'md5';

export const E_LOCK_LOST = new Error('page lock lost');

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService, private ngZone: NgZone) {
        this.setupDb();
    }

    async addSession(session: Session, rom: Uint8Array, ram?: Uint8Array, state?: Uint8Array): Promise<Session> {
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
            this.saveMemory(tx, storedSession.id, ram);
        }

        if (state) {
            this.saveState(tx, storedSession.id, state);
        }

        await complete(tx);

        return storedSession;
    }

    async getAllSessions(): Promise<Array<Session>> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);

        await this.acquireLock(objectStoreSession, -1);

        const sessions = await complete(objectStoreSession.getAll());

        return sessions;
    }

    async getSession(id: number): Promise<Session | undefined> {
        const tx = await this.newTransaction(OBJECT_STORE_SESSION);
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);

        await this.acquireLock(objectStoreSession, -1);

        return await complete(objectStoreSession.get(id));
    }

    async deleteSession(session: Session): Promise<void> {
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

        this.deleteMemory(tx, session.id);
        this.deleteState(tx, session.id);

        await complete(tx);

        this.sessionChangeEvent.dispatch(session.id);
    }

    async updateSession(session: Session): Promise<void> {
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

        this.sessionChangeEvent.dispatch(session.id);
    }

    async loadSession(session: Session): Promise<[Uint8Array | undefined, Uint8Array, Uint8Array | undefined]> {
        const tx = await this.newTransaction(OBJECT_STORE_ROM, OBJECT_STORE_STATE, OBJECT_STORE_MEMORY);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        await this.acquireLock(objectStoreRom, -1);

        return [
            await complete<Uint8Array>(objectStoreRom.get(session.rom)),
            await this.loadMemory(tx, session.id, session.ram * 1024 * 1024),
            await this.loadState(tx, session.id),
        ];
    }

    async deleteStateForSession(session: Session): Promise<void> {
        const tx = await this.newTransaction(OBJECT_STORE_STATE);
        const objectStoreState = tx.objectStore(OBJECT_STORE_STATE);

        await this.acquireLock(objectStoreState, -1);

        await complete(objectStoreState.delete(session.id));
    }

    getDb(): Promise<IDBDatabase> {
        return this.db;
    }

    async acquireLock(store: IDBObjectStore, key: string | number): Promise<void> {
        await complete(store.get(key));

        if (this.pageLockService.lockLost()) {
            throw E_LOCK_LOST;
        }
    }

    async newTransaction(...stores: Array<string>): Promise<IDBTransaction> {
        return (await this.db).transaction(stores, 'readwrite');
    }

    private saveState(tx: IDBTransaction, sessionId: number, state: Uint8Array | undefined): void {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        if (state) {
            objectStore.put(state, sessionId);
        } else {
            objectStore.delete(sessionId);
        }
    }

    private deleteState(tx: IDBTransaction, sessionId: number): void {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        objectStore.delete(sessionId);
    }

    private loadState(tx: IDBTransaction, sessionId: number): Promise<Uint8Array | undefined> {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        return complete(objectStore.get(sessionId));
    }

    private saveMemory = (tx: IDBTransaction, sessionId: number, memory: Uint8Array): void =>
        this.ngZone.runOutsideAngular(() => {
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
        });

    private deleteMemory(tx: IDBTransaction, sessionId: number): void {
        const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);

        objectStore.delete(IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true));
    }

    private loadMemory = (tx: IDBTransaction, sessionId: number, size: number): Promise<Uint8Array> =>
        this.ngZone.runOutsideAngular(() => {
            return new Promise((resolve, reject) => {
                const objectStore = tx.objectStore(OBJECT_STORE_MEMORY);
                const memory = new Uint8Array(size);

                const request = objectStore.openCursor(
                    IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true)
                );

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
        });

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

    public sessionChangeEvent = new Event<number>();

    private db!: Promise<IDBDatabase>;
}
