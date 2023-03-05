import createModule, {
    CreateZipContextState,
    DeleteRecursiveContextState,
    FileEntry as FileEntryNative,
    Module,
    ReaddirError,
    ReaddirStatus,
    UnzipContextState,
    VfsAttr,
    Vfs as VfsNative,
    VfsResult,
    VoidPtr,
    WriteFileResult,
} from '@native-vfs/index';

import { dirtyPagesSize } from './util';

// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

export { ReaddirError, VfsResult, WriteFileResult } from '@native-vfs/index';

export interface Attributes {
    readonly: boolean;
    hidden: boolean;
    system: boolean;
}

export interface FileEntry {
    path: string;
    name: string;
    size: number;
    lastModifiedTS: number;
    lastModifiedLocalDate: string;
    isDirectory: boolean;
    attributes: Attributes;
}

export type ReaddirResult =
    | { error: ReaddirError.none; entries: Array<FileEntry> }
    | { error: ReaddirError.no_such_directory | ReaddirError.unknown; reason: string };

export enum UnzipResult {
    success,
    zipfileError,
    ioError,
    cardFull,
}

const TIMESLICE_SIZE_MSEC = 25;

function deserializeAttributes(attr: number): Attributes {
    return {
        readonly: (attr & VfsAttr.AM_RDO) !== 0,
        hidden: (attr & VfsAttr.AM_HID) !== 0,
        system: (attr & VfsAttr.AM_SYS) !== 0,
    };
}

function convertFileEntry(entry: FileEntryNative, path: string): FileEntry {
    const name = entry.GetName();
    const lastModifiedTS = entry.GetModifiedTS();

    return {
        path,
        name,
        size: entry.GetSize(),
        lastModifiedTS,
        lastModifiedLocalDate: new Date(lastModifiedTS * 1000).toLocaleDateString(),
        isDirectory: entry.IsDirectory(),
        attributes: deserializeAttributes(entry.GetAttributes()),
    };
}

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

    allocateImage(size: number): void {
        this.vfsNative.AllocateImage(size >>> 9);
    }

    mountImage(slot: number): boolean {
        return this.vfsNative.MountImage(slot);
    }

    unmountImage(slot: number): void {
        this.vfsNative.UnmountImage(slot);
    }

    getPendingImage(): Uint32Array | undefined {
        const ptr = this.module.getPointer(this.vfsNative.GetPendingImage());
        const size = this.vfsNative.GetPendingImageSize();
        if (ptr === 0 || size === 0) return undefined;

        return this.module.HEAPU32.subarray(ptr >>> 2, (ptr + size) >>> 2);
    }

    getImageInSlot(slot: number): Uint32Array | undefined {
        const ptr = this.module.getPointer(this.vfsNative.GetImage(slot));
        const size = this.vfsNative.GetSize(slot);
        if (ptr === 0 || size === 0) return undefined;

        return this.module.HEAPU32.subarray(ptr >>> 2, (ptr + size) >>> 2);
    }

    getDirtyPagesForSlot(slot: number): Uint8Array | undefined {
        const dirtyPagesPtr = this.module.getPointer(this.vfsNative.GetDirtyPages(slot));
        if (dirtyPagesPtr === 0) return undefined;

        const bufferSize = dirtyPagesSize(this.vfsNative.GetSize(slot));
        return this.module.HEAPU8.subarray(dirtyPagesPtr, dirtyPagesPtr + bufferSize);
    }

    readdir(path: string): ReaddirResult {
        const context = new this.module.ReaddirContext(path);

        try {
            const entries: Array<FileEntry> = [];

            while (context.GetStatus() === ReaddirStatus.more) {
                const nativeEntry = context.GetEntry();
                entries.push(convertFileEntry(nativeEntry, `${path.replace(/\/*$/, '')}/${nativeEntry.GetName()}`));
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

    rename(from: string, to: string): VfsResult {
        return this.vfsNative.RenameFile(from, to);
    }

    chmod(path: string, attributes: Partial<Attributes>): VfsResult {
        let attr = 0;
        let mask = 0;

        let processAttribute = (value: boolean | undefined, flag: number) => {
            if (value === undefined) return;
            mask |= flag;
            if (value) attr |= flag;
        };

        processAttribute(attributes.hidden, VfsAttr.AM_HID);
        processAttribute(attributes.readonly, VfsAttr.AM_RDO);
        processAttribute(attributes.system, VfsAttr.AM_SYS);

        return this.vfsNative.ChmodFile(path, attr, mask);
    }

    stat(
        path: string
    ):
        | { result: VfsResult.FR_OK; entry: FileEntry }
        | { result: Exclude<VfsResult, VfsResult.FR_OK>; entry: undefined } {
        const result = this.vfsNative.StatFile(path);
        if (result !== VfsResult.FR_OK) return { result, entry: undefined };

        return { entry: convertFileEntry(this.vfsNative.GetEntry(), path), result };
    }

    bytesFree(slot: number): number {
        return this.vfsNative.BytesFree(slot);
    }

    bytesTotal(slot: number): number {
        return this.vfsNative.BytesTotal(slot);
    }

    readFile(path: string): Uint8Array | undefined {
        if (!this.vfsNative.ReadFile(path)) return undefined;

        const ptr = this.module.getPointer(this.vfsNative.GetCurrentFileContent());
        const size = this.vfsNative.GetCurrentFileSize();

        const fileContent = this.module.HEAPU8.subarray(ptr, ptr + size).slice();
        this.vfsNative.ReleaseCurrentFile();

        return fileContent;
    }

    unlinkFile(path: string): VfsResult {
        return this.vfsNative.UnlinkFile(path);
    }

    mkdir(path: string): VfsResult {
        return this.vfsNative.Mkdir(path);
    }

    async createZipArchive({
        files,
        directories,
        prefix,
    }: {
        files?: Array<string>;
        directories?: Array<string>;
        prefix: string;
    }): Promise<{
        archive: Uint8Array | undefined;
        failedItems: Array<string>;
    }> {
        const context = new this.module.CreateZipContext(prefix, TIMESLICE_SIZE_MSEC);
        const failingItems: Array<string> = [];

        try {
            if (files) files.forEach((file) => context.AddFile(file));
            if (directories) directories.forEach((directory) => context.AddDirectory(directory));

            while (context.GetState() !== CreateZipContextState.done) {
                switch (context.Continue()) {
                    case CreateZipContextState.errorDirectory:
                    case CreateZipContextState.errorFile:
                        failingItems.push(context.GetErrorItem());
                        break;
                }

                if (context.GetState() !== CreateZipContextState.done) {
                    await new Promise((resolve) => setTimeout(resolve, 0));
                }
            }

            let archive: Uint8Array | undefined;
            const ptr = this.module.getPointer(context.GetZipContent());
            const size = context.GetZipSize();

            if (ptr !== 0 && size !== 0) {
                archive = this.module.HEAPU8.subarray(ptr, ptr + size).slice();
            }

            return { archive, failedItems: failingItems };
        } finally {
            this.module.destroy(context);
        }
    }

    async deleteRecursive({
        files,
        directories,
    }: {
        files?: Array<string>;
        directories?: Array<string>;
    }): Promise<{ success: true } | { success: false; failingItem: string }> {
        const context = new this.module.DeleteRecursiveContext(TIMESLICE_SIZE_MSEC);

        try {
            if (files) files.forEach((file) => context.AddFile(file));
            if (directories) directories.forEach((directory) => context.AddDirectory(directory));

            while (context.Continue() === DeleteRecursiveContextState.more) {
                await new Promise((resolve) => setTimeout(resolve, 0));
            }

            if (context.GetState() === DeleteRecursiveContextState.done) {
                return { success: true };
            } else {
                return { success: false, failingItem: context.GetFailingPath() };
            }
        } finally {
            this.module.destroy(context);
        }
    }

    writeFile(path: string, data: Uint8Array): WriteFileResult {
        const dataPtr = this.copyIn(data);

        try {
            return this.vfsNative.WriteFile(path, data.length, dataPtr);
        } finally {
            this.vfsNative.Free(dataPtr);
        }
    }

    async unzipArchive(
        destination: string,
        data: Uint8Array,
        handlers: {
            onFileCollision: (collisionPath: string, entry: string) => Promise<boolean>;
            onDirectoryCollision: (collisionPath: string, entry: string) => Promise<boolean>;
            onInvalidEntry: (entry: string) => Promise<void>;
        }
    ): Promise<{ result: UnzipResult; entriesTotal: number; entriesSuccess: number }> {
        const dataPtr = this.copyIn(data);
        const context = new this.module.UnzipContext(TIMESLICE_SIZE_MSEC, destination, dataPtr, data.length);
        const result = (unzipResult: UnzipResult) => ({
            result: unzipResult,
            entriesTotal: context.GetEntriesTotal(),
            entriesSuccess: context.GetEntriesSuccess(),
        });

        try {
            while (true) {
                await new Promise((resolve) => setTimeout(resolve, 0));

                switch (context.GetState()) {
                    case UnzipContextState.done:
                        return result(UnzipResult.success);

                    case UnzipContextState.cardFull:
                        return result(UnzipResult.cardFull);

                    case UnzipContextState.ioError:
                        return result(UnzipResult.ioError);

                    case UnzipContextState.zipfileError:
                        return result(UnzipResult.zipfileError);

                    case UnzipContextState.collision:
                        if (await handlers.onFileCollision(context.GetCollisionPath(), context.GetCurrentEntry())) {
                            context.ContinueWithOverwrite();
                        } else {
                            context.Continue();
                        }

                        continue;

                    case UnzipContextState.collisionWithDirectory:
                        if (
                            await handlers.onDirectoryCollision(context.GetCollisionPath(), context.GetCurrentEntry())
                        ) {
                            context.ContinueWithOverwrite();
                        } else {
                            context.Continue();
                        }

                        continue;

                    default:
                        context.Continue();
                        break;
                }
            }
        } finally {
            this.module.destroy(context);
            this.vfsNative.Free(dataPtr);
        }
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const buffer = this.vfsNative.Malloc(data.length);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAPU8.subarray(bufferPtr, bufferPtr + data.length).set(data);

        return buffer;
    }

    private vfsNative: VfsNative;
}
