import {
    DB_VERSION,
    OBJECT_STORE_KVS,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_ROM,
    OBJECT_STORE_SESSION,
    OBJECT_STORE_STATE,
} from './storage/constants';
import { Injectable, NgZone } from '@angular/core';
import { complete, compressPage } from './storage/util';
import { migrate0to1, migrate1to2 } from './storage/migrations';

import { ErrorService } from './error.service';
import { Event } from 'microevent.ts';
import { Kvs } from './../model/Kvs';
import { PageLockService } from './page-lock.service';
import { Session } from 'src/app/model/Session';
import { StorageError } from './storage/StorageError';
import { environment } from '../../environments/environment';
import md5 from 'md5';

export const E_LOCK_LOST = new Error('page lock lost');

function guard(): any {
    return (target: any, propertyKey: string, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = async function (this: any) {
            const errorService: ErrorService = this.errorService;

            try {
                return await oldMethod.apply(this, arguments);
            } catch (e) {
                if (e instanceof StorageError) {
                    errorService.fatalIDBDead();
                } else {
                    errorService.fatalBug(e.message);
                }
            }
        };

        return desc;
    };
}

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(private pageLockService: PageLockService, private ngZone: NgZone, private errorService: ErrorService) {
        this.setupDb();
    }

    getDb(): Promise<IDBDatabase> {
        return this.db;
    }

    @guard()
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

    @guard()
    async getAllSessions(): Promise<Array<Session>> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_SESSION);

        return await complete(objectStore.getAll());
    }

    @guard()
    async getSession(id: number): Promise<Session | undefined> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_SESSION);

        return await complete(objectStore.get(id));
    }

    @guard()
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

    @guard()
    async updateSession(session: Session): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_SESSION);

        const persistentSession = await complete<Session>(objectStore.get(session.id));

        if (!persistentSession) throw new Error(`no session with id ${session.id}`);
        if (persistentSession.rom !== session.rom) throw new Error('attempt to change ROM reference');
        if (persistentSession.ram !== session.ram) throw new Error('attempt to change RAM size');
        if (persistentSession.device !== session.device) throw new Error('attempt to change device type');

        objectStore.put(session);

        await complete(tx);

        this.sessionChangeEvent.dispatch(session.id);
    }

    @guard()
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

    @guard()
    async deleteStateForSession(session: Session): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_STATE);

        objectStore.delete(session.id);

        await complete(tx);
    }

    @guard()
    async kvsGet<T extends keyof Kvs>(key: T): Promise<Kvs[T] | undefined> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        return complete(objectStore.get(key));
    }

    @guard()
    async kvsSet<T extends keyof Kvs>(key: T, value: Kvs[T]): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        objectStore.put(value, key);

        await complete(tx);
    }

    async kvsLoad(): Promise<Partial<Kvs>> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        return new Promise((resolve, reject) => {
            const kvs: Partial<Kvs> = {};
            const request = objectStore.openCursor();

            request.onsuccess = () => {
                const cursor = request.result;
                if (!cursor) return resolve(kvs);

                kvs[cursor.key as keyof Kvs] = cursor.value;

                cursor.continue();
            };

            request.onerror = () => reject(new StorageError(request.error?.message));
        });
    }

    @guard()
    async kvsDelete(key: keyof Kvs): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        objectStore.delete(key);

        await complete(tx);
    }

    @guard()
    async acquireLock(store: IDBObjectStore, key: string | number): Promise<void> {
        await complete(store.get(key));

        if (this.pageLockService.lockLost()) {
            throw E_LOCK_LOST;
        }
    }

    @guard()
    async newTransaction(...stores: Array<string>): Promise<IDBTransaction> {
        return (await this.db).transaction(stores, 'readwrite');
    }

    @guard()
    async prepareObjectStore(name: string): Promise<[IDBObjectStore, IDBTransaction]> {
        const tx = await this.newTransaction(name);
        const objectStore = tx.objectStore(name);

        await this.acquireLock(objectStore, -1);

        return [objectStore, tx];
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

                request.onerror = () => reject(new StorageError(request.error?.message));
            });
        });

    private setupDb() {
        this.db = new Promise((resolve, reject) => {
            const request = indexedDB.open(environment.dbName, DB_VERSION);

            request.onerror = () => reject(new StorageError('failed to open DB'));
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
