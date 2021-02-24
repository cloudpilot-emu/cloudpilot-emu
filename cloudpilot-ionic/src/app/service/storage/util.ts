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

        requestOrTransaction.onerror = () => reject();
    });
}
