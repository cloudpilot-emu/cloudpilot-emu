declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface CloudpilotNative {
    malloc(size: number): VoidPtr;
    free(buffer: VoidPtr): void;
}
