declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export const enum BackupType {
    ram = 0,
    ramRom = 1,
    everything = 2,
}

export const enum BackupState {
    created = 0,
    inProgress = 1,
    done = 2,
    error = -1,
}

export interface Uarm {
    SetRamSize(size: number): Uarm;
    SetNand(size: number, data: VoidPtr): Uarm;
    SetMemory(size: number, data: VoidPtr): Uarm;
    SetSavestate(size: number, data: VoidPtr): Uarm;
    SetSd(size: number, data: VoidPtr, id: string): Uarm;
    SetDefaultMips(defaultMips: number): Uarm;

    Launch(romSize: number, romData: VoidPtr): boolean;

    Cycle(now: bigint): number;

    GetFrame(): VoidPtr;
    ResetFrame(): void;

    GetTimesliceSizeUsec(): number;

    PenDown(x: number, y: number): void;
    PenUp(): void;

    CurrentIps(): number;
    CurrentIpsMax(): number;
    SetMaxLoad(maxLoad: number): void;
    SetCyclesPerSecondLimit(cyclesPerSecondLimit: number): void;

    GetTimestampUsec(): bigint;

    KeyDown(key: number): void;
    KeyUp(key: number): void;

    PendingSamples(): number;
    PopQueuedSamples(): VoidPtr;
    SetPcmOutputEnabled(enabled: boolean): void;
    SetPcmSuspended(suspended: boolean): void;

    GetRomDataSize(): number;
    GetRomData(): VoidPtr;

    GetNandDataSize(): number;
    GetNandData(): VoidPtr;
    GetNandDirtyPages(): VoidPtr;
    IsNandDirty(): boolean;
    SetNandDirty(isDirty: boolean): void;

    GetSdCardDataSize(): number;
    GetSdCardData(): VoidPtr;
    GetSdCardDirtyPages(): VoidPtr;
    IsSdCardDirty(): boolean;
    SetSdCardDirty(isDirty: boolean): void;

    GetRamDataSize(): number;
    GetRamData(): VoidPtr;
    GetRamDirtyPages(): VoidPtr;

    GetDeviceType(): number;

    SdCardInsert(data: VoidPtr, length: number, id: string): boolean;
    SdCardEject(): void;
    Reset(): void;

    Save(): void;
    GetSavestateSize(): number;
    GetSavestateData(): VoidPtr;
    IsSdInserted(): boolean;

    GetRamSize(): number;

    InstallDatabase(len: number, data: VoidPtr): number;

    NewDbBackup(type: BackupType): DbBackup;
}

export interface DbBackup {
    Init(): boolean;

    GetState(): BackupState;
    Continue(): boolean;
    HasLastProcessedDb(): boolean;
    GetLastProcessedDb(): string;

    GetArchiveData(): VoidPtr;
    GetArchiveSize(): number;
}

export interface Bridge {
    Malloc(size: number): VoidPtr;
    Free(ptr: VoidPtr): void;
}
