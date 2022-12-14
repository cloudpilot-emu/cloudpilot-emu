export enum StorageCardStatus {
    dirty = 'dirty',
    clean = 'clean',
    unformatted = 'unformatted',
}

export interface StorageCard {
    id: number;
    storageId: string;
    size: number;
    name: string;
    status: StorageCardStatus;
    // the double negation is ugly, but it allows for backwards compatibility
    // without an explicit migration.
    dontFsckAutomatically?: boolean;
}
