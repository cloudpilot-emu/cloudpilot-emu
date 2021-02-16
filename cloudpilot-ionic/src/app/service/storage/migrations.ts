import { OBJECT_STORE_MEMORY, OBJECT_STORE_ROM, OBJECT_STORE_SESSION, OBJECT_STORE_STATE } from './constants';

export function migrate0to1(db: IDBDatabase): void {
    const sessionStore = db.createObjectStore(OBJECT_STORE_SESSION);
    sessionStore.createIndex('rom', 'rom');

    db.createObjectStore(OBJECT_STORE_ROM, { keyPath: 'hash' });
    db.createObjectStore(OBJECT_STORE_STATE);
    db.createObjectStore(OBJECT_STORE_MEMORY, { keyPath: ['s', 'p'] });
}
