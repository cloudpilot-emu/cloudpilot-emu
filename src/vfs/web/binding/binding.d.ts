declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export const enum VfsResult {
    FR_OK = 0,
    FR_DISK_ERR,
    FR_INT_ERR,
    FR_NOT_READY,
    FR_NO_FILE,
    FR_NO_PATH,
    FR_INVALID_NAME,
    FR_DENIED,
    FR_EXIST,
    FR_INVALID_OBJECT,
    FR_WRITE_PROTECTED,
    FR_INVALID_DRIVE,
    FR_NOT_ENABLED,
    FR_NO_FILESYSTEM,
    FR_MKFS_ABORTED,
    FR_TIMEOUT,
    FR_LOCKED,
    FR_NOT_ENOUGH_CORE,
    FR_TOO_MANY_OPEN_FILES,
    FR_INVALID_PARAMETER,
}

export const enum VfsAttr {
    AM_RDO = 0x01,
    AM_HID = 0x02,
    AM_SYS = 0x04,
    AM_DIR = 0x10,
    AM_ARC = 0x20,
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

    GetEntry(): FileEntry;
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

    GetEntry(): FileEntry;

    GetStatus(): ReaddirStatus;
    GetError(): ReaddirError;
    GetErrorDescription(): string;
}
