// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../../../node_modules/@types/emscripten/index.d.ts"/>

import createModule, { Cloudpilot as CloudpilotNative, Module, PalmButton, VoidPtr } from '../../../../src';

import { DeviceId } from '../model/DeviceId';
import { Event } from 'microevent.ts';

export { PalmButton } from '../../../../src';

export interface RomInfo {
    cardVersion: number;
    cardName: string;
    romVersion: number;
    romVersionString: string;
    supportedDevices: Array<DeviceId>;
}

export interface Frame {
    bpp: number;
    lineWidth: number;
    lines: number;
    margin: number;
    bytesPerLine: number;

    buffer: Uint8Array;
}

const SUPPORTED_DEVICES = [DeviceId.palmV, DeviceId.m515];

function guard(): any {
    return (target: any, propertyKey: string, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = function (this: any, p1: any, p2: any) {
            return this.guard(() => oldMethod.call(this, p1, p2));
        };

        return desc;
    };
}

export class Cloudpilot {
    private constructor(private module: Module) {
        this.cloudpilot = new module.Cloudpilot();
    }

    static async create(): Promise<Cloudpilot> {
        return new Cloudpilot(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
            })
        );
    }

    destroy(): void {
        this.module.destroy(this.cloudpilot);
    }

    @guard()
    getRomInfo(rom: Uint8Array): RomInfo | undefined {
        const buffer = this.copyIn(rom);

        const romInfoNative = new this.module.RomInfo(buffer, rom.length);
        let romInfo: RomInfo | undefined;

        if (romInfoNative.IsValid()) {
            romInfo = {
                cardVersion: romInfoNative.CardVersion(),
                cardName: romInfoNative.CardName(),
                romVersion: romInfoNative.RomVersion(),
                romVersionString: romInfoNative.RomVersionString(),
                supportedDevices: SUPPORTED_DEVICES.filter(romInfoNative.Supports.bind(romInfoNative)),
            };
        }

        this.cloudpilot.Free(buffer);
        this.module.destroy(romInfoNative);

        return romInfo;
    }

    @guard()
    initializeSession(rom: Uint8Array, deviceType: string): boolean {
        const buffer = this.copyIn(rom);

        const result = this.cloudpilot.InitializeSession(buffer, rom.length, deviceType);

        this.cloudpilot.Free(buffer);

        return result;
    }

    @guard()
    cyclesPerSecond(): number {
        return this.cloudpilot.GetCyclesPerSecond();
    }

    @guard()
    runEmulation(cycles: number): number {
        return this.cloudpilot.RunEmulation(cycles);
    }

    @guard()
    getFrame(): Frame {
        const nativeFrame = this.cloudpilot.CopyFrame();

        const bufferPtr = this.module.getPointer(nativeFrame.GetBuffer());

        return {
            bpp: nativeFrame.GetBpp(),
            bytesPerLine: nativeFrame.GetBytesPerLine(),
            lines: nativeFrame.GetLines(),
            lineWidth: nativeFrame.GetLineWidth(),
            margin: nativeFrame.GetMargin(),
            buffer: this.module.HEAPU8.subarray(bufferPtr, bufferPtr + nativeFrame.GetBufferSize()),
        };
    }

    @guard()
    isScreenDirty(): boolean {
        return this.cloudpilot.IsScreenDirty();
    }

    @guard()
    markScreenClean(): void {
        this.cloudpilot.MarkScreenClean();
    }

    @guard()
    minRamForDevice(id: DeviceId): number {
        return this.cloudpilot.MinMemoryForDevice(id) * 1024;
    }

    @guard()
    queuePenMove(x: number, y: number): void {
        this.cloudpilot.QueuePenMove(x, y);
    }

    @guard()
    queuePenUp(): void {
        this.cloudpilot.QueuePenUp();
    }

    @guard()
    queueButtonDown(button: PalmButton): void {
        this.cloudpilot.QueueButtonDown(button);
    }

    @guard()
    queueButtonUp(button: PalmButton): void {
        this.cloudpilot.QueueButtonUp(button);
    }

    @guard()
    isPowerOff(): boolean {
        return !!this.cloudpilot.IsPowerOff();
    }

    @guard()
    isUiInitialized(): boolean {
        return !!this.cloudpilot.IsUIInitialized();
    }

    @guard()
    reset(): void {
        this.cloudpilot.Reset();
    }

    @guard()
    resetNoExtensions(): void {
        this.cloudpilot.ResetNoExtensions();
    }

    @guard()
    resetHard(): void {
        this.cloudpilot.ResetHard();
    }

    @guard()
    installFile(data: Uint8Array): number {
        const buffer = this.copyIn(data);

        const result = this.cloudpilot.InstallFile(buffer, data.length);

        this.cloudpilot.Free(buffer);

        return result;
    }

    @guard()
    getPalette2bitMapping(): number {
        return this.cloudpilot.GetPalette2bitMapping();
    }

    @guard()
    getMemory(): Uint8Array {
        const ptr = this.module.getPointer(this.cloudpilot.GetMemoryPtr());

        return this.module.HEAPU8.subarray(ptr, ptr + this.cloudpilot.GetMemorySize());
    }

    @guard()
    getDirtyPages(): Uint8Array {
        const ptr = this.module.getPointer(this.cloudpilot.GetDirtyPagesPtr());
        const memorySize = this.cloudpilot.GetMemorySize();

        return this.module.HEAPU8.subarray(ptr, ptr + (memorySize >>> 13));
    }

    @guard()
    getSavestate(): Uint8Array {
        const ptr = this.module.getPointer(this.cloudpilot.GetSavestatePtr());

        return this.module.HEAPU8.subarray(ptr, ptr + this.cloudpilot.GetSavestateSize());
    }

    @guard()
    saveState(): boolean {
        return !!this.cloudpilot.SaveState();
    }

    @guard()
    loadState(state: Uint8Array): boolean {
        const ptr = this.copyIn(state);

        const result = this.cloudpilot.LoadState(ptr, state.length);

        this.cloudpilot.Free(ptr);

        return result;
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const buffer = this.cloudpilot.Malloc(data.length);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAP8.subarray(bufferPtr, bufferPtr + data.length).set(data);

        return buffer;
    }

    private guard<T>(fn: () => T) {
        if (this.amIdead) throw new Error('cloudpilot instance is dead');

        try {
            return fn();
        } catch (e) {
            this.amIdead = true;
            this.cloudpilot = undefined as any;

            this.fatalErrorEvent.dispatch(e instanceof Error ? e : new Error('unknown error'));

            throw e;
        }
    }

    fatalErrorEvent = new Event<Error>();

    private cloudpilot: CloudpilotNative;
    private amIdead = false;
}
