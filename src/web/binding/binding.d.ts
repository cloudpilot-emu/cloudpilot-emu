declare const __void_ptr_tag__: unique symbol;

export interface VoidPtr {
    [__void_ptr_tag__]: null;
}

export interface RomInfoNative {
    CardVersion(): number;
    CardName(): string;
    RomVersion(): number;
    RomVersionString(): string;
}

export interface CloudpilotNative {
    Malloc(size: number): VoidPtr;
    Free(buffer: VoidPtr): void;

    GetRomInfo(buffer: VoidPtr, size: number, romInfo: RomInfo): bool;
    InitializeSession(buffer: VoidPtr, size: number, deviceType: string): boolean;

    GetCyclesPerSecond(): number;
    RunEmulation(cycles: long): number;
    CopyFrame(): Frame;
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
