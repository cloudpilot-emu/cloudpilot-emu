// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../../../node_modules/@types/emscripten/index.d.ts"/>

import createModule, {
    Cloudpilot as CloudpilotNative,
    DbBackup,
    DbInstallResult,
    Module,
    PalmButton,
    SuspendContextClipboardCopy,
    SuspendContextClipboardPaste,
    SuspendKind,
    VoidPtr,
} from '../../../../src';

import { DeviceId } from '../model/DeviceId';
import { Event } from 'microevent.ts';

export {
    PalmButton,
    DbBackup,
    DbInstallResult,
    SuspendKind,
    SuspendContextClipboardCopy,
    SuspendContextClipboardPaste,
} from '../../../../src';

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

export interface PwmUpdate {
    frequency: number;
    dutyCycle: number;
}

export const SUPPORTED_DEVICES = [
    DeviceId.palmVx,
    DeviceId.palmV,
    DeviceId.iiix,
    DeviceId.iiie,
    DeviceId.m515,
    DeviceId.iiic,
    DeviceId.m130,
    DeviceId.i710,
];

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

        this.cloudpilot.RegisterPwmHandler(
            module.addFunction(
                (frequency: number, dutyCycle: number) => this.pwmUpdateEvent.dispatch({ frequency, dutyCycle }),
                'vdd'
            )
        );
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
    queueKeyboardEvent(char: number, ctrl: boolean): void {
        this.cloudpilot.QueueKeyboardEvent(char, ctrl);
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
    isSetupComplete(): boolean {
        return !!this.cloudpilot.IsSetupComplete();
    }

    @guard()
    getOSVersion(): number {
        return this.cloudpilot.GetOSVersion();
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
    installDb(data: Uint8Array): DbInstallResult {
        const buffer = this.copyIn(data);

        const result = this.cloudpilot.InstallDb(buffer, data.length);

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
    getMemory32(): Uint32Array {
        let ptr = this.module.getPointer(this.cloudpilot.GetMemoryPtr());

        if (ptr & 0x03) {
            throw new Error('unaligned pointer');
        }

        ptr >>>= 2;

        return this.module.HEAPU32.subarray(ptr, ptr + (this.cloudpilot.GetMemorySize() >>> 2));
    }

    @guard()
    getDirtyPages(): Uint8Array {
        const ptr = this.module.getPointer(this.cloudpilot.GetDirtyPagesPtr());
        const memorySize = this.cloudpilot.GetMemorySize();

        return this.module.HEAPU8.subarray(ptr, ptr + (memorySize >>> 13));
    }

    @guard()
    getRomImage(): Uint8Array {
        const ptr = this.module.getPointer(this.cloudpilot.GetRomPtr());

        return this.module.HEAPU8.subarray(ptr, ptr + this.cloudpilot.GetRomSize());
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

    @guard()
    getHotsyncName(): string {
        return this.cloudpilot.GetHotsyncName();
    }

    @guard()
    setClockFactor(factor: number): void {
        this.cloudpilot.SetClockFactor(factor);
    }

    @guard()
    setHotsyncName(name: string): void {
        this.cloudpilot.SetHotsyncName(name);
    }

    async backup<T>(worker: (dbBackup: DbBackup) => Promise<T>): Promise<T> {
        const dbBackup = this.guard(() => this.cloudpilot.StartBackup());

        try {
            return await worker(this.wrap(dbBackup));
        } finally {
            this.module.destroy(dbBackup);
        }
    }

    getArchive(dbBackup: DbBackup): Uint8Array | undefined {
        const size = dbBackup.GetArchiveSize();

        if (size <= 0) return undefined;

        const ptr = this.module.getPointer(dbBackup.GetArchivePtr());

        return this.module.HEAPU8.subarray(ptr, ptr + size).slice();
    }

    @guard()
    setClipboardIntegration(toggle: boolean): void {
        this.cloudpilot.SetClipboardIntegration(toggle);
    }

    @guard()
    isSuspended(): boolean {
        return this.cloudpilot.IsSuspended();
    }

    @guard()
    getSuspendKind(): SuspendKind {
        return this.cloudpilot.GetSuspendContext().GetKind();
    }

    @guard()
    cancelSuspend(): void {
        this.cloudpilot.GetSuspendContext().Cancel();
    }

    @guard()
    getSuspendContextClipboardCopy(): SuspendContextClipboardCopy {
        return this.wrap(this.cloudpilot.GetSuspendContext().AsContextClipboardCopy());
    }

    @guard()
    getSuspendContextClipboardPaste(): SuspendContextClipboardPaste {
        return this.wrap(this.cloudpilot.GetSuspendContext().AsContextClipboardPaste());
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const buffer = this.cloudpilot.Malloc(data.length);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAP8.subarray(bufferPtr, bufferPtr + data.length).set(data);

        return buffer;
    }

    private wrap<T extends object>(unguarded: T): T {
        return new Proxy<T>(unguarded, {
            get: (target: T, p: string) => {
                const val = target[p as keyof T];

                if (typeof val !== 'function') {
                    return val;
                }

                return (...args: Array<any>) => this.guard(() => val.apply(target, args));
            },
        });
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
    pwmUpdateEvent = new Event<PwmUpdate>();

    private cloudpilot: CloudpilotNative;
    private amIdead = false;
}
