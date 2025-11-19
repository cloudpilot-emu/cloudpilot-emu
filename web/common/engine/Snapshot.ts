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
    refresh(): Promise<void>;
    release(persistenceSuccess: boolean, timeBlocking: number, timeBackground: number): void;

    getStorageKey(): string | undefined;
    getSnapshotMemory(): Snapshot | undefined;
    getSnapshotNand(): Snapshot | undefined;
    getSnapshotStorage(): Snapshot | undefined;
    getMemoryCrc(): number | undefined;

    getSavestate(): Uint8Array | undefined;
}
