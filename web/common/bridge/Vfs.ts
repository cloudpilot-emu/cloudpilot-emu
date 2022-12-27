// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

import { ReaddirError, ReaddirStatus, Vfs as VfsNative } from '@native-vfs/index';
import createModule, { Module } from '@native-vfs/index';

export { ReaddirError } from '@native-vfs/index';

export interface FileEntry {
    name: string;
    size: number;
    modifiedTS: number;
    isDirectory: boolean;
}

export type ReaddirResult =
    | { error: ReaddirError.none; entries: Array<FileEntry> }
    | { error: ReaddirError.no_such_directory | ReaddirError.unknown; reason: string };

export class Vfs {
    private constructor(private module: Module) {
        this.vfsNative = new module.Vfs();
    }

    static async create(): Promise<Vfs> {
        return new Vfs(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
            })
        );
    }

    AllocateImage(size: number): void {
        this.vfsNative.AllocateImage(size >>> 9);
    }

    MountImage(slot: number): boolean {
        return this.vfsNative.MountImage(slot);
    }

    UnmountImage(slot: number): void {
        this.vfsNative.UnmountImage(slot);
    }

    GetPendingImage(): Uint32Array | undefined {
        const ptr = this.module.getPointer(this.vfsNative.GetPendingImage());
        const size = this.vfsNative.GetPendingImageSize();
        if (ptr === 0 || size === 0) return undefined;

        return this.module.HEAPU32.subarray(ptr >>> 2, (ptr + size) >>> 2);
    }

    GetImageInSlot(slot: number): Uint32Array | undefined {
        const ptr = this.module.getPointer(this.vfsNative.GetImage(slot));
        const size = this.vfsNative.GetSize(slot);
        if (ptr === 0 || size === 0) return undefined;

        return this.module.HEAPU32.subarray(ptr >>> 2, (ptr + size) >>> 2);
    }

    Readdir(path: string): ReaddirResult {
        const context = new this.module.ReaddirContext(path);

        try {
            const entries: Array<FileEntry> = [];

            while (context.GetStatus() === ReaddirStatus.more) {
                entries.push({
                    name: context.GetEntryName(),
                    size: context.GetEntrySize(),
                    modifiedTS: context.GetEntryModifiedTS(),
                    isDirectory: context.IsEntryDirectory(),
                });

                context.Next();
            }

            if (context.GetStatus() === ReaddirStatus.error) {
                console.warn(`readdir failed: ${context.GetErrorDescription()}`);
                const error = context.GetError();

                return {
                    error: error === ReaddirError.none ? ReaddirError.unknown : error,
                    reason: context.GetErrorDescription(),
                };
            }

            return { error: ReaddirError.none, entries };
        } finally {
            this.module.destroy(context);
        }
    }

    private vfsNative: VfsNative;
}
