import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { Event } from 'microevent.ts';

import { Snapshot, SnapshotContainer } from '../Snapshot';
import { HostMessage, HostMessageType } from './worker/HostMessage';
import { Snapshot as RemoteSnapshot, UarmSnapshot } from './worker/Snapshot';

function getTransferables(snapshot: RemoteSnapshot | undefined): Array<Transferable> {
    return snapshot ? [snapshot.pagePool, snapshot.scheduledPages] : [];
}

type Writeable<T> = { -readonly [P in keyof T]: T[P] };

export class SnapshotContainerImpl implements SnapshotContainer {
    constructor(
        memorySize: number,
        nandSize: number,
        private dispatch: (message: HostMessage, transferrables?: Array<Transferable>) => void,
    ) {
        if (memorySize % 1024 !== 0) throw new Error('invalid memory size');
        if (nandSize % 4224 !== 0) throw new Error('invalid NAND size');

        const memoryPages = memorySize >>> 10;
        const offsetsMemory = new Uint32Array(memoryPages);
        for (let i = 0; i < memoryPages; i++) offsetsMemory[i] = i * (1024 >>> 2);

        this.snapshotMemory = {
            pageCount: 0,
            pageCountTotal: memoryPages,
            offsets: offsetsMemory,
            pages: new Uint32Array(),
            pageData: new Uint32Array(),
            pageSize: 1024 >>> 2,
        };

        const nandPages = (nandSize / 4224) | 0;
        const offsetsNand = new Uint32Array(nandPages);
        for (let i = 0; i < nandPages; i++) offsetsNand[i] = i * (4224 >>> 2);

        this.snapshotNand = {
            pageCount: 0,
            pageCountTotal: nandPages,
            offsets: offsetsNand,
            pages: new Uint32Array(),
            pageData: new Uint32Array(),
            pageSize: 4224 >>> 2,
        };
    }

    schedule(uarmSnapshot: UarmSnapshot): void {
        if (this.uarmSnapshot) throw new Error('snapshot already pending');

        this.updateSnapshot(this.snapshotMemory, uarmSnapshot.memory);
        if (uarmSnapshot.nand) this.updateSnapshot(this.snapshotNand, uarmSnapshot.nand);
        this.updateSnapshotSd(uarmSnapshot);

        this.savestate = new Uint8Array(uarmSnapshot.savestate);

        this.uarmSnapshot = uarmSnapshot;
    }

    materialize(): boolean {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return true;
    }

    release(persistenceSuccess: boolean, timeStorageBlocking: number, timeStorageBackground: number): void {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        let size =
            this.uarmSnapshot.memory.scheduledPageCount * 1024 +
            (this.uarmSnapshot.nand?.scheduledPageCount ?? 0) * 4224 +
            this.uarmSnapshot.savestate.byteLength;

        if (this.uarmSnapshot.sd) size += this.uarmSnapshot.sd.snapshot.scheduledPageCount * 8192;

        this.dispatch(
            { type: HostMessageType.returnSnapshot, snapshot: this.uarmSnapshot, success: persistenceSuccess },
            [
                ...getTransferables(this.uarmSnapshot.memory),
                ...getTransferables(this.uarmSnapshot.nand),
                ...getTransferables(this.uarmSnapshot.sd?.snapshot),
                this.uarmSnapshot.savestate,
            ],
        );

        const timeBlocking = this.uarmSnapshot.time;
        const timeTotal = timeBlocking + timeStorageBackground + timeStorageBlocking;
        const timestamp = this.uarmSnapshot.timestamp;

        this.uarmSnapshot = undefined;
        this.snapshotSuccessEvent.dispatch({ size, timeBlocking, timeTotal, timestamp });
    }

    getStorageKey(): string | undefined {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.uarmSnapshot.sd?.key;
    }

    getSnapshotMemory(): Snapshot {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.snapshotMemory;
    }

    getSnapshotNand(): Snapshot | undefined {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.uarmSnapshot.nand ? this.snapshotNand : undefined;
    }

    getSnapshotStorage(): Snapshot | undefined {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.uarmSnapshot.sd ? this.snapshotSd : undefined;
    }

    getMemoryCrc(): number | undefined {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.uarmSnapshot.memory.crc;
    }

    getSavestate(): Uint8Array | undefined {
        if (!this.uarmSnapshot) throw new Error('no pending snapshot');

        return this.savestate;
    }

    private updateSnapshot(snapshot: Writeable<Snapshot>, remoteSnapshot: RemoteSnapshot): void {
        snapshot.pageCount = remoteSnapshot.scheduledPageCount;
        snapshot.pages = new Uint32Array(remoteSnapshot.scheduledPages);
        snapshot.pageData = new Uint32Array(remoteSnapshot.pagePool);
    }

    private updateSnapshotSd(uarmSnapshot: UarmSnapshot): void {
        if (!uarmSnapshot.sd) return;

        if (!this.snapshotSd) {
            this.snapshotSd = {
                pageCount: 0,
                pageCountTotal: 0,
                offsets: new Uint32Array(),
                pages: new Uint32Array(),
                pageData: new Uint32Array(),
                pageSize: 8192,
            };
        }

        if (this.sdSize !== uarmSnapshot.sd.size) {
            this.snapshotSd.pageCountTotal = uarmSnapshot.sd.size >>> 13;
            if (uarmSnapshot.sd.size > this.snapshotSd.pageCountTotal << 13) this.snapshotSd.pageCountTotal++;

            this.snapshotSd.offsets = new Uint32Array(this.snapshotSd.pageCountTotal);
            for (let i = 0; i < this.snapshotSd.pageCountTotal; i++) this.snapshotSd.offsets[i] = i * 8192;

            this.sdSize = uarmSnapshot.sd.size;
        }

        this.updateSnapshot(this.snapshotSd, uarmSnapshot.sd.snapshot);
    }

    snapshotSuccessEvent = new Event<SnapshotStatistics>();

    private snapshotMemory: Writeable<Snapshot>;
    private snapshotNand: Writeable<Snapshot>;
    private snapshotSd: Writeable<Snapshot> | undefined;
    private savestate: Uint8Array<ArrayBufferLike> = new Uint8Array();
    private sdSize = 0;

    private uarmSnapshot: UarmSnapshot | undefined;
}
