export const OBJECT_STORE_SESSION = 'session';
export const OBJECT_STORE_ROM = 'rom';
export const OBJECT_STORE_MEMORY = 'memory';
export const OBJECT_STORE_STATE = 'state';
export const OBJECT_STORE_KVS = 'kvs';

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

export async function migrate2to3(db: IDBDatabase, tx: IDBTransaction | null): Promise<void> {
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
