import { Injectable, NgZone } from '@angular/core';
import { isIOS } from '@common/helper/browser';
import { crc32 } from '@common/helper/crc';
import { nandSize } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { EngineType } from '@common/model/EngineType';
import { Mutex } from 'async-mutex';
import md5 from 'md5';
import { Event } from 'microevent.ts';
import { v4 as uuid } from 'uuid';

import { Kvs } from '@pwa/model/Kvs';
import { MemoryMetadata } from '@pwa/model/MemoryMetadata';
import { Session } from '@pwa/model/Session';
import { StorageCard } from '@pwa/model/StorageCard';

import { environment } from '../../environments/environment';
import { ErrorService } from './error.service';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { StorageError } from './storage/StorageError';
import {
    DB_VERSION,
    OBJECT_STORE_KVS,
    OBJECT_STORE_LOCK,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_MEMORY_META,
    OBJECT_STORE_NAND,
    OBJECT_STORE_ROM,
    OBJECT_STORE_SESSION,
    OBJECT_STORE_STATE,
    OBJECT_STORE_STORAGE,
    OBJECT_STORE_STORAGE_CARD,
} from './storage/constants';
import {
    migrate0to1,
    migrate1to2,
    migrate2to4,
    migrate4to5,
    migrate5to6,
    migrate6to7,
    migrate7to8,
    migrate8to9,
    migrate9to10,
} from './storage/migrations';
import { complete, compressPage, compressPage8 } from './storage/util';

export const E_LOCK_LOST = new Error('page lock lost');
const E_VERSION_MISMATCH = new Error('version mismatch');
const LOCK_TOKEN = uuid();

function guard(): MethodDecorator {
    return (target: unknown, propertyKey: string | symbol, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = async function (this: Partial<StorageService>, ...args: Array<unknown>) {
            const errorService = this.errorService;

            try {
                return await oldMethod.apply(this, args);
            } catch (e: unknown) {
                console.error(e);

                if (e instanceof StorageError) {
                    errorService?.fatalIDBDead();
                } else if (e === E_VERSION_MISMATCH) {
                    errorService?.fatalVersionMismatch();
                } else if (e === E_LOCK_LOST) {
                    errorService?.fatalPageLockLost();
                } else if (e instanceof Error) {
                    errorService?.fatalBug(e?.message);
                } else {
                    errorService?.fatalBug('unknown reason');
                }

                throw e;
            }
        };

        return desc;
    };
}

@Injectable({
    providedIn: 'root',
})
export class StorageService {
    constructor(
        private ngZone: NgZone,
        public errorService: ErrorService,
        private storageCardContext: StorageCardContext,
    ) {
        this.db = this.requestPersistentStorage().then(() => ngZone.runOutsideAngular(() => this.setupDb()));
    }

    getDb(): Promise<IDBDatabase> {
        return this.db;
    }

    @guard()
    addSession(
        session: Session,
        { rom, memory, nand, state }: { rom: Uint8Array; memory?: Uint8Array; nand?: Uint8Array; state?: Uint8Array },
    ): Promise<Session> {
        return this.ngZone.runOutsideAngular(async () => {
            const hash = md5(rom);

            const tx = await this.newTransaction(
                OBJECT_STORE_SESSION,
                OBJECT_STORE_ROM,
                OBJECT_STORE_STATE,
                OBJECT_STORE_MEMORY,
                OBJECT_STORE_MEMORY_META,
                OBJECT_STORE_NAND,
            );
            const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
            const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

            const recordRom = await complete(objectStoreRom.get(hash));
            if (!recordRom) {
                objectStoreRom.put(rom, hash);
            }

            const { id, ...sessionSansId } = session;
            const key = await complete(objectStoreSession.add({ ...sessionSansId, rom: hash }));

            const storedSession = await complete<Session>(objectStoreSession.get(key));

            this.saveMemory(tx, session.engine, storedSession.id, memory);
            this.saveNand(tx, storedSession.id, nand);
            this.saveState(tx, storedSession.id, state);

            await complete(tx);

            return storedSession;
        });
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
            OBJECT_STORE_MEMORY,
            OBJECT_STORE_MEMORY_META,
            OBJECT_STORE_NAND,
        );
        const objectStoreSession = tx.objectStore(OBJECT_STORE_SESSION);
        const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

        session = await complete(objectStoreSession.get(session.id));
        if (!session) return;

        objectStoreSession.delete(session.id);

        const sessionsUsingRom = await complete(objectStoreSession.index('rom').getAll(session.rom));

        if (sessionsUsingRom.length === 0) objectStoreRom.delete(session.rom);

        this.deleteMemory(tx, session.id);
        this.deleteState(tx, session.id);
        this.deleteNand(tx, session.id);

        await complete(tx);

        this.sessionChangeEvent.dispatch([session.id, undefined]);
    }

    @guard()
    updateSessionPartial(id: number, update: Partial<Session>): Promise<void> {
        return this.sessionUpdateMutex.runExclusive(async () => {
            const { id: _id, rom, ram, device, engine, nand, ...rest } = update;
            const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_SESSION);

            const session = await complete<Session>(objectStore.get(id));
            if (!session) throw new Error(`no session with id ${id}`);

            const updatedSession = { ...session, ...rest };
            objectStore.put(updatedSession);

            await complete(tx);

            this.sessionChangeEvent.dispatch([session.id, updatedSession]);
        });
    }

    @guard()
    updateSession(session: Session): Promise<void> {
        return this.sessionUpdateMutex.runExclusive(async () => {
            const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_SESSION);

            const persistentSession = await complete<Session>(objectStore.get(session.id));

            if (!persistentSession) throw new Error(`no session with id ${session.id}`);
            if (persistentSession.engine !== session.engine) throw new Error('attempt to change session engine');
            if (persistentSession.rom !== session.rom) throw new Error('attempt to change ROM reference');
            if (persistentSession.ram !== session.ram) throw new Error('attempt to change RAM size');
            if (persistentSession.device !== session.device) throw new Error('attempt to change device type');
            if (persistentSession.nand !== session.nand) throw new Error('attempt to change NAND size');

            objectStore.put(session);

            await complete(tx);

            this.sessionChangeEvent.dispatch([session.id, session]);
        });
    }

    @guard()
    loadSession(
        session: Session,
        checkCrc: boolean,
    ): Promise<{
        rom: Uint8Array;
        memory?: Uint8Array;
        nand?: Uint8Array;
        savestate?: Uint8Array;
    }> {
        return this.ngZone.runOutsideAngular(async () => {
            const tx = await this.newTransaction(
                OBJECT_STORE_ROM,
                OBJECT_STORE_STATE,
                OBJECT_STORE_MEMORY,
                OBJECT_STORE_MEMORY_META,
                OBJECT_STORE_NAND,
            );
            const objectStoreRom = tx.objectStore(OBJECT_STORE_ROM);

            return {
                rom: await complete<Uint8Array>(objectStoreRom.get(session.rom)),
                memory: await this.loadMemory(tx, session.id, checkCrc),
                savestate: await this.loadState(tx, session.id),
                nand: await this.loadNand(tx, session.id, session.device),
            };
        });
    }

    @guard()
    async deleteStateForSession(session: Session): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_STATE);

        objectStore.delete(session.id);

        await complete(tx);
    }

    @guard()
    async addEmptyStorageCard(card: Omit<StorageCard, 'id'>): Promise<StorageCard> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_STORAGE_CARD);

        const key = await complete(objectStore.add(card));

        const updatedCard = await complete(objectStore.get(key));
        this.storageCardChangeEvent.dispatch(updatedCard.id);

        return updatedCard;
    }

    @guard()
    importStorageCard(card: Omit<StorageCard, 'id'>, data: Uint32Array): Promise<StorageCard> {
        return this.ngZone.runOutsideAngular(async () => {
            const tx = await this.newTransaction(OBJECT_STORE_STORAGE_CARD, OBJECT_STORE_STORAGE);
            const objectStoreStorageCard = tx.objectStore(OBJECT_STORE_STORAGE_CARD);
            const objectStoreStorage = tx.objectStore(OBJECT_STORE_STORAGE);

            const key = await complete(objectStoreStorageCard.add(card));
            if (typeof key !== 'number') throw new Error('invalid key');

            this.savePagedData(
                objectStoreStorage,
                8192,
                compressPage,
                key,
                0,
                new Uint8Array(data.buffer, data.byteOffset, data.byteLength),
            );

            const updatedCard = await complete(objectStoreStorageCard.get(key));
            await complete(tx);

            this.storageCardChangeEvent.dispatch(updatedCard.id);

            return updatedCard;
        });
    }

    @guard()
    async getAllStorageCards(): Promise<Array<StorageCard>> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_STORAGE_CARD);

        return complete(objectStore.getAll());
    }

    @guard()
    async getCard(id: number): Promise<StorageCard | undefined> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_STORAGE_CARD);

        return complete(objectStore.get(id));
    }

    @guard()
    async updateStorageCardPartial(id: number, update: Partial<StorageCard>): Promise<void> {
        const { storageId, size, id: _id, ...rest } = update;
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_STORAGE_CARD);

        const persistentCard: StorageCard = await complete(objectStore.get(id));
        if (!persistentCard) throw new Error(`no card with id ${id}`);

        const updatedCard = { ...persistentCard, ...rest };
        objectStore.put(updatedCard);

        await complete(tx);

        this.storageCardChangeEvent.dispatch(id);
    }

    @guard()
    async deleteStorageCard(id: number): Promise<void> {
        const tx = await this.newTransaction(OBJECT_STORE_STORAGE_CARD, OBJECT_STORE_STORAGE);
        this.storageCardContext.assertOwnership(id, CardOwner.none);

        const objectStoreStorageCard = tx.objectStore(OBJECT_STORE_STORAGE_CARD);
        const objectStoreStorage = tx.objectStore(OBJECT_STORE_STORAGE);

        objectStoreStorageCard.delete(id);
        this.deletePagedData(objectStoreStorage, id);

        await complete(tx);

        this.storageCardChangeEvent.dispatch(id);
    }

    @guard()
    loadCardData(id: number, target: Uint32Array, owner: CardOwner): Promise<void> {
        return this.ngZone.runOutsideAngular(async () => {
            const tx = await this.newTransaction(OBJECT_STORE_STORAGE);
            this.storageCardContext.assertOwnership(id, owner);

            const objectStore = tx.objectStore(OBJECT_STORE_STORAGE);

            await this.loadPagedData(id, objectStore, 8192, 0, target);
        });
    }

    @guard()
    updateCardData(id: number, data: Uint32Array, dirtyPages: Uint8Array, owner: CardOwner): Promise<void> {
        return this.ngZone.runOutsideAngular(async () => {
            const tx = await this.newTransaction(OBJECT_STORE_STORAGE_CARD, OBJECT_STORE_STORAGE);
            this.storageCardContext.assertOwnership(id, owner);

            const objectStoreStorageCard = tx.objectStore(OBJECT_STORE_STORAGE_CARD);
            const objectStoreStorage = tx.objectStore(OBJECT_STORE_STORAGE);

            const card: StorageCard | undefined = await complete(objectStoreStorageCard.get(id));
            if (!card) throw new Error(`no card with ID ${id}`);

            if (data.length !== card.size >>> 2) throw new Error(`data size mismatch`);

            const pagesTotal = (card.size >>> 13) + (card.size % 8192 > 0 ? 1 : 0);
            if ((pagesTotal >>> 3) + (pagesTotal % 8 > 0 ? 1 : 0) !== dirtyPages.length) {
                throw new Error(`dirty pages size mismatch`);
            }

            let iPage = 0;
            for (let i = 0; i < dirtyPages.length; i++) {
                for (let mask = 1; mask < 0x0100; mask <<= 1) {
                    if (iPage >= pagesTotal) break;

                    if (dirtyPages[i] & mask) {
                        objectStoreStorage.put(data.subarray(iPage * 2048, (iPage + 1) * 2048).slice(), [
                            card.id,
                            iPage,
                        ]);
                    }

                    iPage++;
                }
            }

            await complete(tx);
        });
    }

    @guard()
    async kvsGet<T extends keyof Kvs>(key: T): Promise<Kvs[T] | undefined> {
        const [objectStore] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        return complete(objectStore.get(key));
    }

    @guard()
    async kvsSet(data: Partial<Kvs>): Promise<void> {
        const [objectStore, tx] = await this.prepareObjectStore(OBJECT_STORE_KVS);

        for (const key of Object.keys(data)) {
            objectStore.put(data[key as keyof Kvs], key);
        }

        await complete(tx);
    }

    @guard()
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

    async acquireLock(tx: IDBTransaction): Promise<void> {
        if (this.errorService.hasFatalError()) throw new Error('fatal error detected --- refusing DB access');

        const lockToken = await complete(tx.objectStore(OBJECT_STORE_LOCK).get(0));

        if (lockToken !== LOCK_TOKEN) {
            tx.abort();
            tx.db.close();

            throw E_LOCK_LOST;
        }
    }

    @guard()
    async newTransaction(...stores: Array<string>): Promise<IDBTransaction> {
        const tx = (await this.db).transaction([OBJECT_STORE_LOCK, ...stores], 'readwrite', {
            durability: 'relaxed',
        });
        await this.acquireLock(tx);

        return tx;
    }

    @guard()
    async prepareObjectStore(name: string): Promise<[IDBObjectStore, IDBTransaction]> {
        const tx = await this.newTransaction(name);
        const objectStore = tx.objectStore(name);

        return [objectStore, tx];
    }

    private loadState(tx: IDBTransaction, sessionId: number): Promise<Uint8Array | undefined> {
        const objectStore = tx.objectStore(OBJECT_STORE_STATE);

        return complete(objectStore.get(sessionId));
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

    private async loadMemory(
        tx: IDBTransaction,
        sessionId: number,
        checkCrc: boolean,
    ): Promise<Uint8Array | undefined> {
        const objectStoreMemoryMeta = tx.objectStore(OBJECT_STORE_MEMORY_META);
        const metadata: MemoryMetadata = await complete(objectStoreMemoryMeta.get(sessionId));
        if (!metadata) return;

        // eslint-disable-next-line no-async-promise-executor
        const memory = await new Promise<Uint8Array>(async (resolve, reject) => {
            const objectStoreMemory = tx.objectStore(OBJECT_STORE_MEMORY);

            const memory = new Uint32Array(metadata.totalSize >>> 2);

            const request = objectStoreMemory.openCursor(
                IDBKeyRange.bound([sessionId, 0], [sessionId + 1, 0], false, true),
            );

            request.onsuccess = () => {
                const cursor = request.result;

                if (!cursor) return resolve(new Uint8Array(memory.buffer));

                const compressedPage: number | Uint8Array | Uint32Array = cursor.value;
                const [, iPage] = cursor.key as [number, number];

                if (typeof compressedPage === 'number') {
                    memory
                        .subarray(iPage * 256, (iPage + 1) * 256)
                        .fill(compressedPage | (compressedPage << 8) | (compressedPage << 16) | (compressedPage << 24));
                } else {
                    memory
                        .subarray(iPage * 256, (iPage + 1) * 256)
                        .set(compressedPage.length === 1024 ? new Uint32Array(compressedPage.buffer) : compressedPage);
                }

                cursor.continue();
            };

            request.onerror = () => reject(new StorageError(request.error?.message));
        });

        if (metadata.crc32 !== undefined && checkCrc && environment.debug) {
            if (metadata.crc32 !== crc32(memory)) {
                throw new Error('snapshot CRC mismatch');
            }
        }

        return memory;
    }

    private saveMemory(
        tx: IDBTransaction,
        engine: EngineType,
        sessionId: number,
        memory8: Uint8Array | undefined,
    ): void {
        const objectStoreMemory = tx.objectStore(OBJECT_STORE_MEMORY);
        const objectStoreMemoryMeta = tx.objectStore(OBJECT_STORE_MEMORY_META);

        objectStoreMemoryMeta.delete(sessionId);

        if (memory8) {
            const metadata: MemoryMetadata = {
                sessionId,
                totalSize: memory8.length,
            };
            objectStoreMemoryMeta.put(metadata);
        }

        this.savePagedData(
            objectStoreMemory,
            1024,
            engine === 'cloudpilot' ? compressPage8 : compressPage,
            sessionId,
            0,
            memory8,
        );
    }

    private deleteMemory(tx: IDBTransaction, sessionId: number): void {
        const objectStoreMemory = tx.objectStore(OBJECT_STORE_MEMORY);
        const objectStoreMemoryMeta = tx.objectStore(OBJECT_STORE_MEMORY_META);

        this.deletePagedData(objectStoreMemory, sessionId);
        objectStoreMemoryMeta.delete(sessionId);
    }

    private saveNand(tx: IDBTransaction, sessionId: number, nand8: Uint8Array | undefined): void {
        this.savePagedData(tx.objectStore(OBJECT_STORE_NAND), 4224, compressPage, sessionId, 0xff, nand8);
    }

    private async loadNand(tx: IDBTransaction, sessionId: number, deviceId: DeviceId): Promise<Uint8Array | undefined> {
        const size = nandSize(deviceId);
        if (size === undefined) return undefined;

        const data = new Uint8Array(size);
        const data32 = new Uint32Array(data.buffer);
        const objectStore = tx.objectStore(OBJECT_STORE_NAND);

        await this.loadPagedData(sessionId, objectStore, 4224, 0xff, data32);

        return data;
    }

    private deleteNand(tx: IDBTransaction, sessionId: number): void {
        this.deletePagedData(tx.objectStore(OBJECT_STORE_NAND), sessionId);
    }

    private savePagedData(
        store: IDBObjectStore,
        pageSize: number,
        compress: (page: Uint32Array) => number | Uint32Array,
        id: number,
        blankValue: number,
        data: Uint8Array | undefined,
    ): void {
        blankValue = blankValue | (blankValue << 8) | (blankValue << 16) | (blankValue << 24);

        store.delete(IDBKeyRange.bound([id, 0], [id + 1, 0], false, true));
        if (!data) return;

        const data32 = new Uint32Array(data.buffer, data.byteOffset, data.byteLength >>> 2);
        const pageSize4 = pageSize >>> 2;
        const pageCount = ((data.length / pageSize) | 0) + (data.length % pageSize === 0 ? 0 : 1);
        const paddedPage = new Uint32Array(pageSize4);

        for (let i = 0; i < pageCount; i++) {
            let page = data32.subarray(i * pageSize4, (i + 1) * pageSize4);
            if (i === pageCount - 1) {
                paddedPage.set(page);
                page = paddedPage;
            }

            const compressedPage = compress(page);
            if (typeof compressedPage === 'number' && (compressedPage | 0) === blankValue) continue;

            store.put(typeof compressedPage === 'number' ? compressedPage : compressedPage.slice(), [id, i]);
        }
    }

    private loadPagedData(
        id: number,
        store: IDBObjectStore,
        pageSize: number,
        blankValue: number,
        target: Uint32Array,
    ): Promise<void> {
        blankValue = blankValue | (blankValue << 8) | (blankValue << 16) | (blankValue << 24);

        const pageSize4 = pageSize >>> 2;
        target.fill(blankValue);

        return new Promise<void>((resolve, reject) => {
            const request = store.openCursor(IDBKeyRange.bound([id, 0], [id + 1, 0], false, true));

            request.onsuccess = () => {
                const cursor = request.result;

                if (!cursor) return resolve();

                let compressedPage: number | Uint32Array = cursor.value;
                const [, iPage] = cursor.key as [number, number];

                if (typeof compressedPage === 'number') {
                    target.subarray(iPage * pageSize4, (iPage + 1) * pageSize4).fill(compressedPage);
                } else {
                    if ((iPage + 1) * pageSize4 > target.length) {
                        if ((iPage + 1) * pageSize4 > target.length + pageSize4) throw new Error('bad page index');

                        compressedPage = compressedPage.subarray(
                            0,
                            pageSize4 - (iPage + 1) * pageSize4 + target.length,
                        );
                    }

                    target.subarray(iPage * pageSize4, (iPage + 1) * pageSize4).set(compressedPage);
                }

                cursor.continue();
            };

            request.onerror = () => reject(new StorageError(request.error?.message));
        });
    }

    private deletePagedData(store: IDBObjectStore, id: number): void {
        store.delete(IDBKeyRange.bound([id, 0], [id + 1, 0], false, true));
    }

    @guard()
    private async initializeLock(db: IDBDatabase): Promise<void> {
        const tx = db.transaction(OBJECT_STORE_LOCK, 'readwrite');

        await complete(tx.objectStore(OBJECT_STORE_LOCK).put(LOCK_TOKEN, 0));
    }

    @guard()
    private async setupDb(): Promise<IDBDatabase> {
        // This works on spurious hangs during indexedDB setup when starting up from the homescreen
        // on iOS 14.6
        const watchdogHandle = setTimeout(() => isIOS && window.location.reload(), 500);

        try {
            if (indexedDB.databases) {
                const databaseEntry = (await indexedDB.databases()).find((x) => x.name === environment.dbName);

                clearTimeout(watchdogHandle);

                if (databaseEntry && databaseEntry.version !== undefined && databaseEntry.version > DB_VERSION) {
                    throw E_VERSION_MISMATCH;
                }
            }

            const db: IDBDatabase = await new Promise((resolve, reject) => {
                const request = indexedDB.open(environment.dbName, DB_VERSION);

                request.onerror = () => {
                    reject(new StorageError('failed to open DB'));
                };
                request.onsuccess = () => {
                    resolve(request.result);
                };
                request.onblocked = () => {
                    reject(
                        new StorageError(
                            'failed to open DB: required upgrade blocked by an open instance of CloudPilotEmu',
                        ),
                    );
                };
                request.onupgradeneeded = async (e) => {
                    clearTimeout(watchdogHandle);

                    if (e.oldVersion < 1) {
                        migrate0to1(request.result);
                    }

                    if (e.oldVersion < 2) {
                        migrate1to2(request.result, request.transaction);
                    }

                    // v3 introduced u32 view in snapshots. There is no actual migration, the version
                    // increment only ensures that old versions of CP will not try to load the new
                    // data.

                    if (e.oldVersion < 4) {
                        await migrate2to4(request.result, request.transaction);
                    }

                    if (e.oldVersion < 5) {
                        await migrate4to5(request.result, request.transaction);
                    }

                    if (e.oldVersion < 6) {
                        await migrate5to6(request.result, request.transaction);
                    }

                    if (e.oldVersion < 7) {
                        await migrate6to7(request.result, request.transaction);
                    }

                    if (e.oldVersion < 8) {
                        await migrate7to8(request.result, request.transaction);
                    }

                    if (e.oldVersion < 9) {
                        await migrate8to9(request.result, request.transaction);
                    }

                    if (e.oldVersion < 10) {
                        await migrate9to10(request.result, request.transaction);
                    }
                };
            });

            await this.initializeLock(db);

            return db;
        } finally {
            clearTimeout(watchdogHandle);
        }
    }

    private async requestPersistentStorage() {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        if (!(navigator.storage as any)?.persist || !(navigator.storage as any)?.persisted) {
            console.log('storage manager not supported; unable to request persistent storage');
        }

        try {
            if ((await navigator.storage.persisted()) || (await navigator.storage.persist())) {
                console.log('persistent storage enabled');
            } else {
                console.log('request for persistent storage denied by browser');
            }
        } catch (e) {
            console.warn(e);
            console.log('failed to request persistent storage');
        }
    }

    public sessionChangeEvent = new Event<[number, Session | undefined]>();
    public storageCardChangeEvent = new Event<number>();

    private db!: Promise<IDBDatabase>;

    private sessionUpdateMutex = new Mutex();
}
