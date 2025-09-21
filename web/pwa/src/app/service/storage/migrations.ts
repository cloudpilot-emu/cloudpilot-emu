const OBJECT_STORE_SESSION = 'session';
const OBJECT_STORE_ROM = 'rom';
const OBJECT_STORE_MEMORY = 'memory';
const OBJECT_STORE_STATE = 'state';
const OBJECT_STORE_KVS = 'kvs';
const OBJECT_STORE_MEMORY_META = 'memory-meta';
const OBJECT_STORE_STORAGE = 'storage';
const OBJECT_STORE_STORAGE_META = 'storage-meta';
const OBJECT_STORE_STORAGE_CARD = 'storage-card';
const OBJECT_STORE_LOCK = 'lock';
const OBJECT_STORE_NAND = 'nand';

const INDEX_CARD_STORAGE_ID = 'storageId';

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

export async function migrate2to4(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    interface Session {
        device: string;
        ram: number;
        totalMemory?: number;
    }

    function framebufferSize(deviceId: string) {
        switch (deviceId) {
            case 'PalmIIIc':
            case 'PalmM505':
            case 'PalmM515':
                return 80 * 1024;

            case 'PalmI710':
            case 'PalmM130':
                return 256 * 1024;

            default:
                return 0;
        }
    }

    if (!tx) throw new Error('no version change transaction!');

    const sessionStore = tx.objectStore(OBJECT_STORE_SESSION);
    const cursorRequest = sessionStore.openCursor();

    return new Promise((resolve, reject) => {
        cursorRequest.onsuccess = () => {
            const cursor = cursorRequest.result;
            if (!cursor) {
                resolve();
                return;
            }

            const session: Session = cursor.value;
            const updatedSession: Session = {
                ...session,
                totalMemory: session.ram * 1024 * 1024 + framebufferSize(session.device),
            };

            cursor.update(updatedSession);
            cursor.continue();
        };

        cursorRequest.onerror = (e) => reject(e);
    });
}

export async function migrate4to5(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    if (!tx) throw new Error('no version change transaction!');

    const kvsStore = tx.objectStore(OBJECT_STORE_KVS);

    const credentials: unknown | undefined = await new Promise((resolve, reject) => {
        const request = kvsStore.get('proxyCredentials');

        request.onsuccess = () => resolve(request.result);
        request.onerror = () => reject(new Error(request.error?.message));
    });

    if (credentials) {
        kvsStore.put(credentials, 'credentials');
        kvsStore.delete('proxyCredentials');
    }
}

export async function migrate5to6(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    interface Session {
        id: number;
        totalMemory?: number;
    }

    interface MemoryMetadata {
        sessionId: number;
        totalSize: number;
    }

    if (!tx) throw new Error('no version change transaction!');

    const memoryMetaStore = db.createObjectStore(OBJECT_STORE_MEMORY_META, { keyPath: 'sessionId' });
    db.createObjectStore(OBJECT_STORE_STORAGE);
    db.createObjectStore(OBJECT_STORE_STORAGE_META, { keyPath: 'id', autoIncrement: true });

    const sessionStore = tx.objectStore(OBJECT_STORE_SESSION);
    const cursorRequest = sessionStore.openCursor();

    return new Promise((resolve, reject) => {
        cursorRequest.onsuccess = () => {
            const cursor = cursorRequest.result;
            if (!cursor) {
                resolve();
                return;
            }

            const session: Session = cursor.value;
            if (session.totalMemory !== undefined) {
                const metadata: MemoryMetadata = {
                    sessionId: session.id,
                    totalSize: session.totalMemory,
                };

                memoryMetaStore.put(metadata);

                delete session.totalMemory;
                cursor.update(session);
            }

            cursor.continue();
        };

        cursorRequest.onerror = (e) => reject(e);
    });
}

export async function migrate6to7(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    if (!tx) throw new Error('no version change transaction!');

    tx.objectStore(OBJECT_STORE_STORAGE_META).name = OBJECT_STORE_STORAGE_CARD;
}

export async function migrate7to8(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    if (!tx) throw new Error('no version change transaction!');

    tx.objectStore(OBJECT_STORE_STORAGE_CARD).createIndex(INDEX_CARD_STORAGE_ID, 'storageId');
}

export async function migrate8to9(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    if (!tx) throw new Error('no version change transaction!');

    db.createObjectStore(OBJECT_STORE_LOCK);
}

export async function migrate9to10(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
    db.createObjectStore(OBJECT_STORE_NAND);

    if (!tx) throw new Error('no version change transaction!');

    const sessionStore = tx.objectStore(OBJECT_STORE_SESSION);
    const cursorRequest = sessionStore.openCursor();

    return new Promise((resolve, reject) => {
        cursorRequest.onsuccess = () => {
            const cursor = cursorRequest.result;
            if (!cursor) {
                resolve();
                return;
            }

            const session: object = {
                engine: 'cloudpilot',
                wasResetForcefully: false,
                deviceOrientation: 'portrait',
                hotsyncName: '',
                dontManageHotsyncName: false,
                speed: 1,
                ...cursor.value,
            };

            cursor.update(session);
            cursor.continue();
        };

        cursorRequest.onerror = (e) => reject(e);
    });
}
