export interface Snapshot {
    readonly pageCount: number;
    readonly pageSize: number;

    readonly pages: Uint32Array;
    readonly offsets: Uint32Array;
    readonly pageData: Uint32Array;
}

export interface SnapshotContainer {
    isValid(): boolean;
    getStorageId(): string;

    materialize(): void;
    release(persistenceSuccess: boolean): Promise<void>;

    snapshotMemory(): Snapshot;
    snapshotNand(): Snapshot | undefined;
    snapshotStorage(): Snapshot | undefined;

    savestate: Uint8Array | undefined;
}
