declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export const enum PalmButton {
    cal = 0,
    phone = 1,
    todo = 2,
    notes = 3,
    up = 4,
    down = 5,
    power = 6,
    cradle = 7,
}

export const enum DbInstallResult {
    failedCouldNotOverwrite = -6,
    failureInternal = -5,
    failureDbIsCorrupt = -4,
    failureDbIsOpen = -3,
    failureNotEnoughMemory = -2,
    failureUnknownReasoon = -1,
    success = 1,
    needsReboot = 2,
}

export const enum SuspendKind {
    clipboardCopy = 1,
    clipboardPaste = 2,
    networkRpc = 3,
    networkConnect = 4,
}

export const enum ZipfileWalkerState {
    error = -1,
    open = 0,
    done = 1,
}

export interface RomInfo {
    CardVersion(): number;
    CardName(): string;
    RomVersion(): number;
    RomVersionString(): string;
    IsValid(): boolean;
    Supports(deviceId: string): boolean;
}

export interface Cloudpilot {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;
    Nullptr(): VoidPtr;

    InitializeSession(buffer: VoidPtr, size: number, deviceType: string): boolean;

    GetCyclesPerSecond(): number;
    RunEmulation(cycles: number): number;
    SetClockFactor(clockFactor: number): number;

    CopyFrame(): Frame;
    IsScreenDirty(): boolean;
    MarkScreenClean(): void;
    IsSetupComplete(): boolean;

    MinMemoryForDevice(id: string): number;
    FramebufferSizeForDevice(id: string): number;
    TotalMemoryForDevice(id: string): number;

    QueuePenMove(x: number, y: number): void;
    QueuePenUp(): void;

    QueueButtonDown(button: PalmButton): void;
    QueueButtonUp(button: PalmButton): void;

    QueueKeyboardEvent(c: number, ctrl: boolean): void;

    IsPowerOff(): boolean;
    IsUIInitialized(): boolean;
    GetOSVersion(): number;

    Reset(): void;
    ResetNoExtensions(): void;
    ResetHard(): void;

    InstallDb(buffer: VoidPtr, len: number): DbInstallResult;

    GetPalette2bitMapping(): number;

    GetMemoryPtr(): VoidPtr;
    GetDirtyPagesPtr(): VoidPtr;
    GetMemorySize(): number;

    GetRomPtr(): VoidPtr;
    GetRomSize(): number;

    GetSavestatePtr(): VoidPtr;
    GetSavestateSize(): number;

    SaveState(): boolean;
    LoadState(buffer: VoidPtr, len: number): boolean;

    GetHotsyncName(): string;
    SetHotsyncName(name: string): void;

    RegisterPwmHandler(handlerPtr: number): void;

    StartBackup(): DbBackup;

    SetClipboardIntegration(toggle: boolean): void;
    GetClipboardIntegration(): boolean;

    IsSuspended(): boolean;
    GetSuspendContext(): SuspendContext;

    SetNetworkRedirection(toggle: boolean): void;
    GetNetworkRedirection(): boolean;
    RegisterProxyDisconnectHandler(handlerPtr: number): void;

    SetHotsyncNameManagement(toggle: boolean): void;

    LaunchAppByName(name: string): boolean;
}

export interface Frame {
    GetLineWidth(): number;
    GetBpp(): number;
    GetLines(): number;
    GetMargin(): number;
    GetBytesPerLine(): number;

    GetFirstDirtyLine(): number;
    GetLastDirtyLine(): number;
    GetHasChanges(): boolean;

    GetBuffer(): VoidPtr;
    GetBufferSize(): number;

    GetScaleX(): number;
    GetScaleY(): number;
}

export interface DbBackup {
    Init(includeRomDatabases: boolean): boolean;

    IsInProgress(): boolean;
    IsDone(): boolean;

    GetCurrentDatabase(): string;

    Save(): boolean;
    Skip(): void;

    GetArchivePtr(): VoidPtr;
    GetArchiveSize(): number;
}

export interface SuspendContextClipboardCopy {
    Cancel(): void;
    Resume(): void;

    GetClipboardContent(): string;
}

export interface SuspendContextClipboardPaste {
    Cancel(): void;
    Resume(clipboardContent: string): void;
}

export interface SuspendContextNetworkConnect {
    Cancel(): void;
    Resume(sessionId: string): void;
}

export interface SuspendContextNetworkRpc {
    Cancel(): void;
    GetRequestSize(): number;
    GetRequestData(): VoidPtr;
    ReceiveResponse(data: VoidPtr, size: number): void;
}

export interface SuspendContext {
    GetKind(): SuspendKind;
    Cancel(): void;

    AsContextClipboardCopy(): SuspendContextClipboardCopy;
    AsContextClipboardPaste(): SuspendContextClipboardPaste;
    AsContextNetworkConnect(): SuspendContextNetworkConnect;
    AsContextNetworkRpc(): SuspendContextNetworkRpc;
}

export interface ZipfileWalker {
    GetState(): ZipfileWalkerState;
    Next(): ZipfileWalkerState;

    GetCurrentEntrySize(): number;
    GetCurrentEntryName(): string;
    GetCurrentEntryContent(): VoidPtr;
}

export interface SessionImage {
    GetDeviceId(): string;
    SetDeviceId(deviceId: string): SessionImage;

    GetRomImage(): VoidPtr;
    GetRomImageSize(): number;
    SetRomImage(image: VoidPtr, size: number): SessionImage;

    GetMemoryImage(): VoidPtr;
    GetMemoryImageSize(): number;
    SetMemoryImage(image: VoidPtr, size: number): SessionImage;

    GetMetadata(): VoidPtr;
    GetMetadataSize(): number;
    SetMetadata(metadata: VoidPtr, size: number): SessionImage;

    GetSavestate(): VoidPtr;
    GetSavestateSize(): number;
    SetSavestate(savestate: VoidPtr, size: number): SessionImage;

    GetFramebufferSize(): number;
    SetFramebufferSize(framebufferSize: number): SessionImage;

    GetVersion(): number;

    Serialize(): boolean;
    GetSerializedImage(): VoidPtr;
    GetSerializedImageSize(): number;

    Deserialize(buffer: VoidPtr, size: number): boolean;
}
