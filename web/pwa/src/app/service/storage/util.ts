import { StorageError } from './StorageError';

function isTransaction(requestOrTransaction: IDBRequest | IDBTransaction): requestOrTransaction is IDBTransaction {
    return !!(requestOrTransaction as IDBTransaction).abort;
}

export function complete(transaction: IDBTransaction): Promise<void>;
export function complete<T>(request: IDBRequest<T>): Promise<T>;
export function complete(requestOrTransaction: IDBRequest | IDBTransaction): Promise<unknown> {
    return new Promise<unknown>((resolve, reject) => {
        if (isTransaction(requestOrTransaction)) {
            requestOrTransaction.oncomplete = () => resolve(undefined);
        } else {
            requestOrTransaction.onsuccess = () => resolve(requestOrTransaction.result);
        }

        requestOrTransaction.onerror = () => reject(new StorageError(requestOrTransaction.error?.message));
    });
}

export function compressPage(page: Uint32Array): Uint32Array | number {
    let probe = page[0] & 0xff;
    probe |= probe << 8;
    probe |= probe << 16;

    for (let i = 0; i < 256; i++) {
        if (page[i] !== probe) return page;
    }

    return page[0] & 0xff;
}

export function compressStoragePage(page: Uint32Array): Uint32Array | number {
    const fst = page[0];

    for (let i = 1; i < 2048; i++) {
        if (page[i] !== fst) return page;
    }

    return fst;
}
