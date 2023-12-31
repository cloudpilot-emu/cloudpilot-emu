export const enum GunzipState {
    more = 0,
    done = 1,
    error = -1,
}

export const enum GzipState {
    initial = 0,
    more = 1,
    done = 2,
    error = -1,
}

export const enum ZipfileWalkerState {
    error = -1,
    open = 0,
    done = 1,
}

export interface GunzipContext<VoidPtr> {
    GetState(): GunzipState;
    Continue(): GunzipState;

    GetUncompressedData(): VoidPtr;
    ReleaseUncompressedData(): VoidPtr;
    GetUncompressedSize(): number;

    GetError(): string;
}

export interface GzipContext<VoidPtr> {
    SetFilename(filename: string): GzipContext<VoidPtr>;
    SetMtime(mtime: number): GzipContext<VoidPtr>;

    Continue(): number;
    GetState(): number;

    GetGzipData(): VoidPtr;
    GetGzipSize(): number;

    GetError(): string;
}

export interface CreateZipContext<VoidPtr> {
    Initialize(compressionLevel: number): boolean;

    AddEntry(name: string): boolean;

    WriteData(data: VoidPtr, size: number): boolean;

    GetZipData(): VoidPtr;
    GetZipDataSize(): number;

    GetLastError(): number;
}

export interface ZipfileWalker<VoidPtr> {
    GetState(): ZipfileWalkerState;
    Next(): ZipfileWalkerState;

    GetTotalEntries(): number;

    GetCurrentEntrySize(): number;
    GetCurrentEntryName(): string;
    GetCurrentEntryContent(): VoidPtr;
}

export interface ModuleWithGunzipContext<VoidPtr> {
    GunzipContext: {
        new (data: VoidPtr, size: number, slizeSize: number): GunzipContext<VoidPtr>;
    };
}

export interface ModuleWithGzipContext<VoidPtr> {
    GzipContext: {
        new (data: VoidPtr, size: number, slizeSize: number): GzipContext<VoidPtr>;
    };
}

export interface ModuleWithCreateZipContext<VoidPtr> {
    CreateZipContext: {
        new (): CreateZipContext<VoidPtr>;
    };
}

export interface ModuleWithZipfileWalker<VoidPtr> {
    ZipfileWalker: {
        new (bufferSize: number, buffer: VoidPtr): ZipfileWalker<VoidPtr>;
    };
}
