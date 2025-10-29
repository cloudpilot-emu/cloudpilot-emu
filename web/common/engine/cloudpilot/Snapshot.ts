import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { crc32 } from '@common/helper/crc';

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

    schedule(withCrc: boolean): this {
        if (this.state !== SnapshotState.idle) throw new Error('snapshot already in progress');
        this.state = SnapshotState.scheduled;
        this.memoryCrcCheck = withCrc;

        return this;
    }

    isIdle(): boolean {
        return this.state === SnapshotState.idle;
    }

    getStorageKey(): string | undefined {
        return this.storageKey;
    }

    materialize(): boolean {
        if (this.cloudpilotInstance.isSuspended()) {
            this.state = SnapshotState.idle;
            return false;
        }

        switch (this.state) {
            case SnapshotState.scheduled:
                break;

            case SnapshotState.materialized:
                this.mergeBack();
                break;

            default:
                throw new Error(`bad snapshot state in materialize: ${this.state}`);
        }

        if (!this.snapshotMemory) throw new Error('snapshot not initialized');
        this.materializeSnapshot(this.snapshotMemory, this.cloudpilotInstance.getDirtyPages());
        this.updateMemoryCrc();

        const storageKey = this.cloudpilotInstance.getMountedKey() || undefined;
        this.updateSnapshotStorage(storageKey);

        if (this.snapshotStorage) {
            if (this.storageKey === undefined) throw new Error('unreachable');

            const dirtyPages = this.cloudpilotInstance.getCardDirtyPages(this.storageKey);
            if (dirtyPages === undefined) throw new Error(`storage key ${this.storageKey} not registered`);

            this.materializeSnapshot(this.snapshotStorage, dirtyPages);
        }

        this.savestate = this.cloudpilotInstance.saveState() ? this.cloudpilotInstance.getSavestate() : undefined;

        this.state = SnapshotState.materialized;
        return true;
    }

    async release(persistenceSuccess: boolean): Promise<void> {
        const oldState = this.state;
        this.state = SnapshotState.idle;

        if (oldState !== SnapshotState.materialized) return;

        if (!persistenceSuccess) this.mergeBack();
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

    getMemoryCrc(): number | undefined {
        return this.memoryCrc;
    }

    private mergeBackSnapshot(snapshot: Snapshot, dirtyPages: Uint8Array): void {
        for (let i = 0; i < snapshot.pageCount; i++) {
            const pageIndex = snapshot.pages[i];

            dirtyPages[pageIndex >>> 3] |= 1 << (pageIndex & 0x07);
        }
    }

    private mergeBack(): void {
        if (!this.snapshotMemory) throw new Error('snapshot not materialized');
        this.mergeBackSnapshot(this.snapshotMemory, this.cloudpilotInstance.getDirtyPages());

        if (this.snapshotStorage) {
            if (this.storageKey === undefined) throw new Error('unreachable');

            const dirtyPages = this.cloudpilotInstance.getCardDirtyPages(this.storageKey);
            if (dirtyPages === undefined) {
                console.warn(
                    `seems like storage ${this.storageKey} was ejected; nothing to merge back, changes are lost`,
                );
            } else {
                this.mergeBackSnapshot(this.snapshotStorage, dirtyPages);
            }
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

    private updateMemoryCrc(): void {
        if (this.memoryCrcCheck) {
            const memory = this.cloudpilotInstance.getMemory32();

            this.memoryCrc = crc32(new Uint8Array(memory.buffer, memory.byteOffset, memory.byteLength));
        } else {
            this.memoryCrc = undefined;
        }
    }

    private updateSnapshotStorage(storageKey: string | undefined): void {
        if (this.storageKey === storageKey) return;

        this.storageKey = storageKey;
        this.snapshotStorage = undefined;

        if (storageKey === undefined) return;

        const data = this.cloudpilotInstance.getCardData(storageKey);
        if (!data) throw new Error(`storage key ${storageKey} not registered`);

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

    private state = SnapshotState.idle;
    private memoryCrcCheck = false;
    private memoryCrc: number | undefined;

    private snapshotMemory: Writeable<Snapshot> | undefined;
    private snapshotStorage: Writeable<Snapshot> | undefined;
    private savestate: Uint8Array | undefined;
    private storageKey: string | undefined;
}
