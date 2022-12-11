declare const __void_ptr_tag__: unique symbol;

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
