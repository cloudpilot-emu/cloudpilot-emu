import { Cloudpilot } from '@common/bridge/Cloudpilot';

import { Snapshot, SnapshotContainer } from '../Snapshot';

type Writeable<T> = { -readonly [P in keyof T]: T[P] };

const enum SnapshotState {
    idle = 'idle',
    scheduled = 'scheduled',
    materialized = 'materialized',
}

export class SnapshotContainerImpl implements SnapshotContainer {
    constructor(private cloudpilotInstance: Cloudpilot) {}

    initializeMemory(): void {
        const memory = this.cloudpilotInstance.getMemory32();
        const pageCountTotal = memory.length >>> 8;

        this.snapshotMemory = {
            pageSize: 1024 >>> 2,
            pageCount: 0,
            pageCountTotal,
            pages: new Uint32Array(pageCountTotal),
            offsets: new Uint32Array(pageCountTotal),
            pageData: memory,
        };
    }

    addStorage(id: string): void {
        this.storageId = id;

        const data = this.cloudpilotInstance.getCardData(id);
        if (!data) throw new Error(`storage id ${id} not registered`);

        const pageCountTotal = data.length >>> 11;

        this.snapshotStorage = {
            pageSize: 8192 >>> 2,
            pageCount: 0,
            pageCountTotal,
            pages: new Uint32Array(pageCountTotal),
            offsets: new Uint32Array(pageCountTotal),
            pageData: data,
        };
    }

    removeStorage(): void {
        this.storageId = undefined;
        this.snapshotStorage = undefined;
    }

    schedule(): this {
        if (this.state !== SnapshotState.idle) throw new Error('snapshot already in progress');
        this.state = SnapshotState.scheduled;

        return this;
    }

    isIdle(): boolean {
        return this.state === SnapshotState.idle;
    }

    getStorageId(): string | undefined {
        return this.storageId;
    }

    materialize(): void {
        if (this.state !== SnapshotState.scheduled) {
            throw new Error(`bad snapshot state in materialize: ${this.state}`);
        }

        if (!this.snapshotMemory) throw new Error('snapshot not initialized');
        this.materializeSnapshot(this.snapshotMemory, this.cloudpilotInstance.getDirtyPages());

        if (this.snapshotStorage) {
            if (this.storageId === undefined) throw new Error('unreachable');

            const dirtyPages = this.cloudpilotInstance.getCardDirtyPages(this.storageId);
            if (dirtyPages === undefined) throw new Error(`storage id ${this.storageId} not registered`);

            this.materializeSnapshot(this.snapshotStorage, dirtyPages);
        }

        this.savestate = this.cloudpilotInstance.getSavestate();

        this.state = SnapshotState.materialized;
    }

    async release(persistenceSuccess: boolean): Promise<void> {
        if (this.state !== SnapshotState.materialized) {
            throw new Error(`bad snapshot state in release: ${this.state}`);
        }
        this.state = SnapshotState.idle;

        if (persistenceSuccess) return;

        if (!this.snapshotMemory) throw new Error('snapshot not materialized');
        this.mergeBackSnapshot(this.snapshotMemory, this.cloudpilotInstance.getDirtyPages());

        if (this.snapshotStorage) {
            if (this.storageId === undefined) throw new Error('unreachable');

            const dirtyPages = this.cloudpilotInstance.getCardDirtyPages(this.storageId);
            if (dirtyPages === undefined) throw new Error(`storage id ${this.storageId} not registered`);

            this.mergeBackSnapshot(this.snapshotStorage, dirtyPages);
        }
    }

    getSnapshotMemory(): Snapshot {
        if (!this.snapshotMemory) throw new Error('snapshot not initialized');
        if (this.state !== SnapshotState.materialized) throw new Error('snapshot not materialized');

        return this.snapshotMemory;
    }

    getSnapshotNand(): Snapshot | undefined {
        return undefined;
    }

    getSnapshotStorage(): Snapshot | undefined {
        if (this.state !== SnapshotState.materialized) throw new Error('snapshot not materialized');

        return this.snapshotStorage;
    }

    getSavestate(): Uint8Array {
        if (this.state !== SnapshotState.materialized || !this.savestate) throw new Error('snapshot not materialized');

        return this.savestate;
    }

    private mergeBackSnapshot(snapshot: Snapshot, dirtyPages: Uint8Array): void {
        for (let i = 0; i < snapshot.pageCount; i++) {
            const pageIndex = snapshot.pages[i];

            dirtyPages[pageIndex >>> 3] |= 1 << (pageIndex & 0x07);
        }
    }

    private materializeSnapshot(snapshot: Writeable<Snapshot>, dirtyPages: Uint8Array): void {
        const { pageCountTotal, pageSize, pages, offsets } = snapshot;

        let pageCount = 0;
        let iPage = 0;

        for (let i = 0; i < dirtyPages.length; i++) {
            const bitfield = dirtyPages[i];
            dirtyPages[i] = 0;

            if (bitfield === 0) {
                iPage += 8;
                continue;
            }

            for (let mask = 1; mask < 0x0100; mask <<= 1) {
                if (iPage >= pageCountTotal) break;

                if (bitfield & mask) {
                    pages[pageCount] = iPage;
                    offsets[pageCount] = iPage * pageSize;

                    pageCount++;
                }

                iPage++;
            }
        }

        snapshot.pageCount = pageCount;
    }

    private state = SnapshotState.idle;

    private storageId: string | undefined;

    private snapshotMemory: Writeable<Snapshot> | undefined;
    private snapshotStorage: Writeable<Snapshot> | undefined;
    private savestate: Uint8Array | undefined;
}
