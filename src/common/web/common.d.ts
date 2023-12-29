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

interface GunzipContext<VoidPtr> {
    GetState(): GunzipState;
    Continue(): GunzipState;

    GetUncompressedData(): VoidPtr;
    ReleaseUncompressedData(): VoidPtr;
    GetUncompressedSize(): number;

    GetError(): string;
}

interface GzipContext<VoidPtr> {
    SetFilename(filename: string): GzipContext<VoidPtr>;
    SetMtime(mtime: number): GzipContext<VoidPtr>;

    Continue(): number;
    GetState(): number;

    GetGzipData(): VoidPtr;
    GetGzipSize(): number;

    GetError(): string;
}

interface CreateZipContext<VoidPtr> {
    Initialize(compressionLevel: number): boolean;

    AddEntry(name: string): boolean;

    WriteData(data: VoidPtr, size: number): boolean;

    GetZipData(): VoidPtr;
    GetZipDataSize(): number;

    GetLastError(): number;
}

interface ModuleWithGunzipContext<VoidPtr> {
    GunzipContext: {
        new (data: VoidPtr, size: number, slizeSize: number): GunzipContext<VoidPtr>;
    };
}

interface ModuleWithGzipContext<VoidPtr> {
    GzipContext: {
        new (data: VoidPtr, size: number, slizeSize: number): GzipContext<VoidPtr>;
    };
}

interface ModuleWithCreateZipContext<VoidPtr> {
    CreateZipContext: {
        new (): CreateZipContext<VoidPtr>;
    };
}
