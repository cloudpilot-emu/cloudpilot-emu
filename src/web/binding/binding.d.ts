declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
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
}

export interface Frame {
    bpp: number;
    lineWidth: number;
    lines: number;
    margin: number;
    bytesPerLine: number;

    GetBuffer(): VoidPtr;
    GetBufferSize(): number;
}
