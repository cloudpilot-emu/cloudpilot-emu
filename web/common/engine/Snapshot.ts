export interface Snapshot {
    readonly pageCountTotal: number;
    readonly pageCount: number;
    readonly pageSize: number;

    readonly pages: Uint32Array;
    readonly offsets: Uint32Array;
    readonly pageData: Uint32Array;
}

export interface SnapshotContainer {
    materialize(): boolean;
    release(persistenceSuccess: boolean, timeSync: number, timeBackground: number): Promise<void>;

    getStorageKey(): string | undefined;
    getSnapshotMemory(): Snapshot;
    getSnapshotNand(): Snapshot | undefined;
    getSnapshotStorage(): Snapshot | undefined;
    getMemoryCrc(): number | undefined;

    getSavestate(): Uint8Array | undefined;
}
