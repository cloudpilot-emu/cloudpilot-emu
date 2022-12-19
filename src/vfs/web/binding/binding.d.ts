declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface Vfs {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;
    Nullptr(): VoidPtr;
}
