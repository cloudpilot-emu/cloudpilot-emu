declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export const enum VfsResult {
    FR_OK = 0,
    FR_DISK_ERR = 1,
    FR_INT_ERR = 2,
    FR_NOT_READY = 3,
    FR_NO_FILE = 4,
    FR_NO_PATH = 5,
    FR_INVALID_NAME = 6,
    FR_DENIED = 7,
    FR_EXIST = 8,
    FR_INVALID_OBJECT = 9,
    FR_WRITE_PROTECTED = 10,
    FR_INVALID_DRIVE = 11,
    FR_NOT_ENABLED = 12,
    FR_NO_FILESYSTEM = 13,
    FR_MKFS_ABORTED = 14,
    FR_TIMEOUT = 15,
    FR_LOCKED = 16,
    FR_NOT_ENOUGH_CORE = 17,
    FR_TOO_MANY_OPEN_FILES = 18,
    FR_INVALID_PARAMETER = 19,
}

export const enum VfsAttr {
    AM_RDO = 0x01,
    AM_HID = 0x02,
    AM_SYS = 0x04,
    AM_DIR = 0x10,
    AM_ARC = 0x20,
}

export const enum WriteFileResult {
    success = 0,
    errInvalidName = 1,
    errIsDirectory = 2,
    errCardFull = 3,
    errIO = 4,
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

export const enum CreateZipContextState {
    initial = 0,
    more = 1,
    done = 2,
    errorFile = -1,
    errorDirectory = -2,
}

export const enum DeleteRecursiveContextState {
    initial = 0,
    more = 1,
    done = 2,
    error = -1,
}

export const enum UnzipContextState {
    more = 0,
    done = 1,
    collision = 2,
    invalidEntry = 3,
    collisionWithDirectory = 4,
    ioError = -1,
    zipfileError = -2,
    cardFull = -3,
}

export interface FileEntry {
    GetName(): string;
    IsDirectory(): boolean;
    GetSize(): number;
    GetModifiedTS(): number;
    GetAttributes(): number;
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
    GetDirtyPages(slot: number): VoidPtr;

    RenameFile(from: string, to: string): VfsResult;
    ChmodFile(path: string, attr: number, mask: number): VfsResult;
    StatFile(path: string): VfsResult;
    UnlinkFile(path: string): VfsResult;
    Mkdir(path: string): VfsResult;

    GetEntry(): FileEntry;

    BytesFree(slot: number): number;
    BytesTotal(slot: number): number;

    ReadFile(path: string): boolean;
    GetCurrentFileSize(): number;
    GetCurrentFileContent(): VoidPtr;
    ReleaseCurrentFile(): void;

    WriteFile(path: string, size: number, data: VoidPtr): WriteFileResult;
}

export interface ReaddirContext {
    Next(): number;

    GetEntry(): FileEntry;

    GetStatus(): ReaddirStatus;
    GetError(): ReaddirError;
    GetErrorDescription(): string;
}

export interface CreateZipContext {
    AddFile(path: string): CreateZipContext;
    AddDirectory(path: string): CreateZipContext;

    Continue(): CreateZipContextState;
    GetState(): CreateZipContextState;

    GetZipContent(): VoidPtr;
    GetZipSize(): number;

    GetErrorItem(): string;
}

export interface DeleteRecursiveContext {
    AddFile(path: string): DeleteRecursiveContext;

    Continue(): DeleteRecursiveContextState;
    GetState(): DeleteRecursiveContextState;

    GetFailingPath(): string;
}

interface UnzipContext {
    GetState(): UnzipContextState;
    Continue(): UnzipContextState;
    ContinueWithOverwrite(): UnzipContextState;

    GetCurrentEntry(): string;
    GetCollisionPath(): string;

    GetEntriesTotal(): number;
    GetEntriesSuccess(): number;
}

interface PasteContext {
    AddFile(path: string): PasteContext;

    GetState(): number;
    Continue(): number;
    ContinueWithOverwrite(): number;

    GetCurrentEntry(): string;
    GetCollisionPath(): string;

    GetEntriesSuccess(): number;
}
