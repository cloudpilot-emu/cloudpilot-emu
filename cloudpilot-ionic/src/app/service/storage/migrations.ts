import {
    OBJECT_STORE_KVS,
    OBJECT_STORE_MEMORY,
    OBJECT_STORE_ROM,
    OBJECT_STORE_SESSION,
    OBJECT_STORE_STATE,
} from './constants';

export function migrate0to1(db: IDBDatabase): void {
    const sessionStore = db.createObjectStore(OBJECT_STORE_SESSION, { keyPath: 'id', autoIncrement: true });
    sessionStore.createIndex('rom', 'rom');

    db.createObjectStore(OBJECT_STORE_STATE);
}

export function migrate1to2(db: IDBDatabase, tx: IDBTransaction | null): void {
    if (!tx) throw new Error('no version change transaction!');

    if (db.objectStoreNames.contains(OBJECT_STORE_ROM)) {
        db.deleteObjectStore(OBJECT_STORE_ROM);
    }

    if (db.objectStoreNames.contains(OBJECT_STORE_MEMORY)) {
        db.deleteObjectStore(OBJECT_STORE_MEMORY);
    }

    tx.objectStore(OBJECT_STORE_SESSION).clear();

    db.createObjectStore(OBJECT_STORE_ROM);
    db.createObjectStore(OBJECT_STORE_MEMORY);
    db.createObjectStore(OBJECT_STORE_KVS);
}
