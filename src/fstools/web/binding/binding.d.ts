declare const __void_ptr_tag__: unique symbol;

export const enum FsckResult {
    pending = 0,
    ok = 1,
    fixed = 2,
    invalid = 3,
    unfixable = 4,
}

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface FSTools {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;
    Nullptr(): VoidPtr;
}

export interface MkfsContext {
    Mkfs(size: number, label: string): boolean;

    GetImage(): VoidPtr;
    GetImageSize(): number;
}

export interface FsckContext {
    GetImage(): VoidPtr;
    GetDirtyPages(): VoidPtr;
    GetImageSize(): number;

    Fsck(): FsckResult;
    GetResult(): FsckResult;
}
