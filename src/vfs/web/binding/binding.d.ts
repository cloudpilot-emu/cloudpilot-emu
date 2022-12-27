declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface Vfs {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;
    Nullptr(): VoidPtr;

    AllocateImage(blockCount: number): void;
    MountImage(slot: number): boolean;
    UnmountImage(slot: number): void;

    GetPendingImageSize(): number;
    GetSize(slot: number): number;
    GetPendingImage(): VoidPtr;
    GetImage(slot: number): VoidPtr;
}

export const enum ReaddirStatus {
    more = 0,
    done = 1,
    error = 2,
}

export const enum ReaddirError {
    none = 0,
    no_such_directory = 1,
    unknown = 2,
}

export interface ReaddirContext {
    Next(): number;

    GetEntryName(): string;

    IsEntryDirectory(): boolean;
    GetEntrySize(): number;
    GetEntryModifiedTS(): number;

    GetStatus(): ReaddirStatus;
    GetError(): ReaddirError;
    GetErrorDescription(): string;
}
