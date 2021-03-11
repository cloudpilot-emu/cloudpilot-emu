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

export function compressPage(page: Uint8Array): Uint8Array | number {
    for (let i = 1; i < 1024; i++) {
        if (page[i] !== page[0]) return page;
    }

    return page[0];
}
