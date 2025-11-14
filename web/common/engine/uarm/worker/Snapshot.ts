export interface Snapshot {
    scheduledPageCount: number;
    scheduledPages: ArrayBufferLike;
    pagePool: ArrayBufferLike;
    crc?: number;
}

export interface UarmSnapshot {
    memory: Snapshot;
    nand: Snapshot;
    sd?: {
        snapshot: Snapshot;
        key: string;
        size: number;
    };
    savestate: ArrayBufferLike;

    time: number;
    timestamp: number;
}
