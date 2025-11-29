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

export const enum DbInstallResult {
    needsReset = 1,
    ok = 0,
    errCorrupt = -2,
    errOom = -3,
    errIsOpen = -4,
    errCouldNotOverwrite = -5,
    errNotSupported = -6,
    errNotCurrentlyPossible = -7,
    errUnknown = -8,
}

export const enum KeyId {
    keyInvalid = 0,
    keyIdHard1 = 1,
    keyIdHard2 = 2,
    keyIdHard3 = 3,
    keyIdHard4 = 4,
    keyIdUp = 5,
    keyIdDown = 6,
    keyIdLeft = 7,
    keyIdRight = 8,
    keyIdSelect = 9,
    keyIdPower = 10,
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
    CurrentIpsMax(): bigint;
    SetMaxLoad(maxLoad: number): void;
    SetCyclesPerSecondLimit(cyclesPerSecondLimit: number): void;

    GetTimestampUsec(): bigint;

    KeyDown(key: KeyId): void;
    KeyUp(key: KeyId): void;

    PendingSamples(): number;
    PopQueuedSamples(): VoidPtr;
    GetSampleQueueSize(): number;
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

    GetMemoryDataSize(): number;
    GetMemoryData(): VoidPtr;
    GetMemoryDirtyPages(): VoidPtr;

    GetDeviceType(): number;

    SdCardInsert(data: VoidPtr, length: number, id: string): boolean;
    SdCardEject(): void;
    SdCardInitialized(): boolean;
    GetSdCardId(): string;
    Reset(): void;

    Save(): void;
    GetSavestateSize(): number;
    GetSavestateData(): VoidPtr;
    IsSdInserted(): boolean;

    GetRamSize(): number;

    JamKey(key: KeyId, durationMsec: number): void;

    IsUiInitialized(): boolean;
    IsOsVersionSet(): boolean;
    GetOsVersion(): number;

    IsLcdEnabled(): boolean;

    InstallDatabase(len: number, data: VoidPtr): DbInstallResult;

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
