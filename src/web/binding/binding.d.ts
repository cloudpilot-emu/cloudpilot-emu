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

    InitializeSession(buffer: VoidPtr, size: number, deviceType: string): boolean;

    GetCyclesPerSecond(): number;
    RunEmulation(cycles: number): number;

    CopyFrame(): Frame;
    IsScreenDirty(): boolean;
    MarkScreenClean(): void;

    MinMemoryForDevice(id: string): number;

    QueuePenMove(x: number, y: number): void;
    QueuePenUp(): void;

    QueueButtonDown(button: PalmButton): void;
    QueueButtonUp(button: PalmButton): void;

    IsPowerOff(): boolean;
    IsUIInitialized(): boolean;

    Reset(): void;
    ResetNoExtensions(): void;
    ResetHard(): void;

    InstallFile(buffer: VoidPtr, len: number): number;
}

export interface Frame {
    GetLineWidth(): number;
    GetBpp(): number;
    GetLines(): number;
    GetMargin(): number;
    GetBytesPerLine(): number;

    GetBuffer(): VoidPtr;
    GetBufferSize(): number;
}
