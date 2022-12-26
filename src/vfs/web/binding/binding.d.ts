declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface Vfs {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;
    Nullptr(): VoidPtr;
}

export namespace ReaddirContext {
    const enum Status {
        more = 0,
        done = 1,
        error = 2,
    }

    const enum Error {
        none = 0,
        no_such_directory = 1,
        unknown = 2,
    }
}

export interface ReaddirContext {
    Next(): number;

    GetEntryName(): string;

    IsEntryDirectory(): boolean;
    GetSize(): number;
    GetTSModified(): number;

    GetStatus(): ReaddirContext.Status;
    GetError(): ReaddirContext.Error;
    GetErrorDescription(): string;
}
