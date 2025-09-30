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

// Cloudpilot originally used Uint8Array to represent page data and only switched
// to 32bit operations later. In order to stay backward compatible we preserve the
// "8 bit" compression algorithm and still use it for Cloudpilot memory.
export function compressPage8(page: Uint32Array): Uint32Array | number {
    let probe = page[0] & 0xff;
    probe |= probe << 8;
    probe |= probe << 16;

    const len = page.length;

    for (let i = 0; i < len; i++) {
        if (page[i] !== probe) return page;
    }

    return page[0] & 0xff;
}

// SD card and uarm dat.
export function compressPage(page: Uint32Array): Uint32Array | number {
    const fst = page[0];
    const len = page.length;

    for (let i = 1; i < len; i++) {
        if (page[i] !== fst) return page;
    }

    return fst;
}
