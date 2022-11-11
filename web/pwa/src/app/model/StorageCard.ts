export enum StorageCardStatus {
    unformatted = 'unformatted',
    dirty = 'dirty',
    clean = 'clean',
}

export interface StorageCard {
    id: number;
    storageId: string;
    size: number;
    name: string;
    status: StorageCardStatus;
}
