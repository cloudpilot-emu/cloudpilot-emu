export interface Snapshot {
    readonly pageCountTotal: number;
    readonly pageCount: number;
    readonly pageSize: number;

    readonly pages: Uint32Array;
    readonly offsets: Uint32Array;
    readonly pageData: Uint32Array;
}

export interface SnapshotContainer {
    getStorageId(): string | undefined;

    materialize(): void;
    release(persistenceSuccess: boolean): Promise<void>;

    getSnapshotMemory(): Snapshot;
    getSnapshotNand(): Snapshot | undefined;
    getSnapshotStorage(): Snapshot | undefined;

    getSavestate(): Uint8Array | undefined;
}
