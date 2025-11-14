import { crc32 } from '@common/helper/crc';

import { Snapshot } from './Snapshot';

const INITIAL_PAGE_POOL_PAGES = 256;
const PAGE_POOL_GROWTH_FACTOR = 1.5;

export interface PagedStorageAdapter {
    getData(): Uint8Array | undefined;
    getDirtyPages(): Uint32Array | undefined;

    isDirty(): boolean;
    setDirty(dirty: boolean): void;
}

export class DirtyPageTracker {
    constructor(
        private pageSize: number,
        private pageCount: number,
        private name: string,
        private crcCheck: boolean,
        private adapter: PagedStorageAdapter,
    ) {
        this.dirtyPagesSize4 = pageCount >>> 5;
        if (this.dirtyPagesSize4 * 32 < pageCount) this.dirtyPagesSize4++;

        this.scheduledPages = new Uint32Array(pageCount);
        this.pagePool = new Uint8Array(INITIAL_PAGE_POOL_PAGES * pageSize);
    }

    takeSnapshot(): Snapshot | undefined {
        if (this.scheduledPageCount > 0) {
            console.warn(`${this.name} pending, skipping snapshot...`);
        }

        if (!this.adapter.isDirty()) return undefined;

        const data = this.adapter.getData();
        const dirtyPages = this.adapter.getDirtyPages();

        if (!data || !dirtyPages) return undefined;

        let iPage = 0;
        let page = 0;
        for (let i = 0; i < dirtyPages.length; i++) {
            let pageSet32 = dirtyPages[i];

            if (pageSet32 === 0) {
                page += 32;
                continue;
            }

            for (let j = 0; j < 4; j++) {
                const pageSet8 = pageSet32 & 0xff;
                pageSet32 >>>= 8;

                if (pageSet8 === 0) {
                    page += 8;
                    continue;
                }

                for (let mask = 1; mask < 0x100; mask <<= 1) {
                    if (!(pageSet8 & mask)) {
                        page++;
                        continue;
                    }

                    this.scheduledPages[iPage] = page;
                    const pagePoolOffset = iPage * this.pageSize;
                    const dataOffset = page * this.pageSize;

                    if (pagePoolOffset >= this.pagePool.length) this.growPagePool();

                    this.pagePool
                        .subarray(pagePoolOffset, pagePoolOffset + this.pageSize)
                        .set(data.subarray(dataOffset, dataOffset + this.pageSize));

                    iPage++;
                    page++;
                }
            }

            dirtyPages[i] = 0;
        }

        this.scheduledPageCount = iPage;

        if (this.scheduledPageCount > 0) {
            this.adapter.setDirty(false);

            return {
                scheduledPageCount: this.scheduledPageCount,
                scheduledPages: this.scheduledPages.buffer,
                pagePool: this.pagePool.buffer,
                crc: this.crcCheck ? crc32(data) : undefined,
            };
        } else {
            return undefined;
        }
    }

    getTransferables(): Array<Transferable> {
        return this.scheduledPageCount > 0 ? [this.scheduledPages.buffer, this.pagePool.buffer] : [];
    }

    returnSnapshot(success: boolean, snapshot: Snapshot | undefined): void {
        if (!snapshot) return;

        const scheduledPageCount = this.scheduledPageCount;

        this.scheduledPages = new Uint32Array(snapshot.scheduledPages);
        this.pagePool = new Uint8Array(snapshot.pagePool);
        this.scheduledPageCount = 0;

        if (success) return;

        const dirtyPages = this.adapter.getDirtyPages();

        if (dirtyPages) {
            for (let i = 0; i < scheduledPageCount; i++) {
                const page = this.scheduledPages[i];

                dirtyPages[page >>> 5] |= 1 << (page & 0x1f);
            }
        }

        this.adapter.setDirty(true);
    }

    enableCrc(crcCheck: boolean) {
        this.crcCheck = crcCheck;
    }

    private growPagePool(): void {
        const newPagePool = new Uint8Array(
            Math.min(
                Math.ceil((this.pagePool.length * PAGE_POOL_GROWTH_FACTOR) / this.pageSize) * this.pageSize,
                this.pageCount * this.pageSize,
            ),
        );

        console.log(`growing ${this.name} page pool to ${newPagePool.length} bytes`);

        newPagePool.set(this.pagePool);
        this.pagePool = newPagePool;
    }

    private dirtyPagesSize4: number;

    private scheduledPages: Uint32Array;
    private pagePool: Uint8Array;
    private scheduledPageCount = 0;
}
