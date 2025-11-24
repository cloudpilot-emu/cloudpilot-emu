// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>
import { DeviceId } from '@common/model/DeviceId';
import {
    BackupState,
    BackupType,
    Bridge,
    DbInstallResult as DbInstallResultUarm,
    DeviceType5,
    KeyId,
    Module,
    Uarm as UarmNative,
    VoidPtr,
    default as createModule,
} from '@native-uarm/index';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';

export { BackupState } from '@native-uarm/index';

let nextId = 0;

export interface DbBackup {
    getState(): BackupState;
    continue(): boolean;
    getLastProcessedDb(): string | undefined;

    getArchive(): Uint8Array | undefined;

    destroy(): void;
}

function mapDbInstallResult(result: DbInstallResultUarm): DbInstallResult {
    switch (result) {
        case DbInstallResultUarm.ok:
            return DbInstallResult.success;

        case DbInstallResultUarm.needsReset:
            return DbInstallResult.needsReboot;

        case DbInstallResultUarm.errCorrupt:
            return DbInstallResult.failureDbIsCorrupt;

        case DbInstallResultUarm.errIsOpen:
            return DbInstallResult.failureDbIsOpen;

        case DbInstallResultUarm.errCouldNotOverwrite:
            return DbInstallResult.failedCouldNotOverwrite;

        case DbInstallResultUarm.errNotCurrentlyPossible:
            return DbInstallResult.failureInstallationNotPossible;

        case DbInstallResultUarm.errNotSupported:
            return DbInstallResult.failureInstallationNotPossible;

        case DbInstallResultUarm.errOom:
            return DbInstallResult.failureNotEnoughMemory;

        case DbInstallResultUarm.errUnknown:
            return DbInstallResult.failureUnknownReason;
    }

    result satisfies never;
}

export function mapButton(button: PalmButton): KeyId {
    switch (button) {
        case PalmButton.cal:
            return KeyId.keyIdHard1;

        case PalmButton.phone:
            return KeyId.keyIdHard2;

        case PalmButton.todo:
            return KeyId.keyIdHard3;

        case PalmButton.notes:
            return KeyId.keyIdHard4;

        case PalmButton.up:
            return KeyId.keyIdUp;

        case PalmButton.down:
            return KeyId.keyIdDown;

        case PalmButton.power:
            return KeyId.keyIdPower;

        case PalmButton.rockerLeft:
            return KeyId.keyIdLeft;

        case PalmButton.rockerRight:
            return KeyId.keyIdRight;

        case PalmButton.rockerEnter:
            return KeyId.keyIdSelect;

        default:
            return KeyId.keyInvalid;
    }
}

function guard(): MethodDecorator {
    return (target: unknown, propertyKey: string | symbol, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = function (this: Uarm, p1: unknown, p2: unknown) {
            return this.guard(() => oldMethod.call(this, p1, p2));
        };

        return desc;
    };
}

export class Uarm {
    private constructor(private module: Module) {
        this.uarm = new module.Uarm();
        this.bridge = new module.Bridge();
    }

    static async create(wasmModule: WebAssembly.Module): Promise<Uarm> {
        const id = nextId++;

        return new Uarm(
            await createModule({
                print: (x: string) => console.log(`${id}: ${x}`),
                printErr: (x: string) => console.warn(`${id}: ${x}`),
                instantiateWasm: (imports, callback) =>
                    void WebAssembly.instantiate(wasmModule, imports).then((instance) => callback(instance)),
            }),
        );
    }

    @guard()
    setRamSize(ramSize: number): void {
        this.uarm.SetRamSize(ramSize);
    }

    @guard()
    setMemory(data: Uint8Array): void {
        this.uarm.SetMemory(data.length, this.copyIn(data));
    }

    @guard()
    setNand(data: Uint8Array): void {
        this.uarm.SetNand(data.length, this.copyIn(data));
    }

    @guard()
    setSd(data: Uint8Array, key: string): void {
        this.uarm.SetSd(data.length, this.copyIn(data), key);
    }

    @guard()
    setSavestate(data: Uint8Array): void {
        this.uarm.SetSavestate(data.length, this.copyIn(data));
    }

    @guard()
    launch(nor: Uint8Array): boolean {
        return this.uarm.Launch(nor.length, this.copyIn(nor));
    }

    @guard()
    buttonUp(button: PalmButton): void {
        const key = mapButton(button);
        if (key >= 0) this.uarm.KeyUp(key);
    }

    @guard()
    buttonDown(button: PalmButton): void {
        const key = mapButton(button);
        if (key >= 0) this.uarm.KeyDown(key);
    }

    @guard()
    penDown(x: number, y: number): void {
        this.uarm.PenDown(x, y);
    }

    @guard()
    penUp(): void {
        this.uarm.PenUp();
    }

    @guard()
    getTimestampUsec(): bigint {
        return this.uarm.GetTimestampUsec();
    }

    @guard()
    cycle(now: bigint): number {
        return this.uarm.Cycle(now);
    }

    @guard()
    getTimesliceSizeUsec(): number {
        return this.uarm.GetTimesliceSizeUsec();
    }

    @guard()
    getDevice(): DeviceId {
        const deviceType = this.uarm.GetDeviceType();

        switch (deviceType) {
            case DeviceType5.deviceTypeE2:
                return DeviceId.te2;

            case DeviceType5.deviceTypeFrankenE2:
                return DeviceId.frankene2;

            default:
                throw new Error(`invalid device ID ${deviceType}`);
        }
    }

    @guard()
    getFrame(height: number): Uint32Array | undefined {
        const framePtr = this.module.getPointer(this.uarm.GetFrame()) >>> 2;

        return framePtr === 0 ? undefined : this.module.HEAPU32.subarray(framePtr, framePtr + 320 * height);
    }

    @guard()
    resetFrame(): void {
        this.uarm.ResetFrame();
    }

    @guard()
    setTargetMips(targetMips: number): void {
        this.uarm.SetCyclesPerSecondLimit(targetMips * 1000000);
    }

    @guard()
    setMaxHostLoad(maxHostLoad: number): void {
        this.uarm.SetMaxLoad(maxHostLoad * 100);
    }

    @guard()
    getMemorySize(): number {
        return this.uarm.GetMemoryDataSize();
    }

    @guard()
    getMemoryData(): Uint8Array {
        const ptr = this.module.getPointer(this.uarm.GetMemoryData());

        return this.module.HEAPU8.subarray(ptr, ptr + this.uarm.GetMemoryDataSize());
    }

    @guard()
    getMemoryDirtyPages(): Uint32Array {
        const ptr = this.module.getPointer(this.uarm.GetMemoryDirtyPages());
        const pageCount = this.uarm.GetMemoryDataSize() >>> 10;

        const ptr4 = ptr >>> 2;
        let size4 = pageCount >>> 5;
        if (size4 * 32 < pageCount) size4++;

        return this.module.HEAPU32.subarray(ptr4, ptr4 + size4);
    }

    @guard()
    getSdCardSize(): number {
        return this.uarm.GetSdCardDataSize();
    }

    @guard()
    getSdCardData(): Uint8Array | undefined {
        const ptr = this.module.getPointer(this.uarm.GetSdCardData());
        if (ptr === 0) return undefined;

        return this.module.HEAPU8.subarray(ptr, ptr + this.uarm.GetSdCardDataSize());
    }

    @guard()
    getSdCardDirtyPages(): Uint32Array | undefined {
        const ptr = this.module.getPointer(this.uarm.GetSdCardDirtyPages());
        if (ptr === 0) return undefined;

        const pageCount = this.uarm.GetSdCardDataSize() >>> 13;

        const ptr4 = ptr >>> 2;
        let size4 = pageCount >>> 5;
        if (size4 * 32 < pageCount) size4++;

        return this.module.HEAPU32.subarray(ptr4, ptr4 + size4);
    }

    @guard()
    isSdCardDirty(): boolean {
        return this.uarm.IsSdCardDirty();
    }

    @guard()
    setSdCardDirty(dirty: boolean): void {
        this.uarm.SetSdCardDirty(dirty);
    }

    @guard()
    getNandSize(): number {
        return this.uarm.GetNandDataSize();
    }

    @guard()
    getNandData(): Uint8Array {
        const ptr = this.module.getPointer(this.uarm.GetNandData());

        return this.module.HEAPU8.subarray(ptr, ptr + this.uarm.GetNandDataSize());
    }

    @guard()
    getNandDirtyPages(): Uint32Array {
        const ptr = this.module.getPointer(this.uarm.GetNandDirtyPages());
        const pageCount = (this.uarm.GetNandDataSize() / 4224) | 0;

        const ptr4 = ptr >>> 2;
        let size4 = pageCount >>> 5;
        if (size4 * 32 < pageCount) size4++;

        return this.module.HEAPU32.subarray(ptr4, ptr4 + size4);
    }

    @guard()
    isNandDirty(): boolean {
        return this.uarm.IsNandDirty();
    }

    @guard()
    setNandDirty(dirty: boolean): void {
        this.uarm.SetNandDirty(dirty);
    }

    @guard()
    saveState(): Uint8Array {
        this.uarm.Save();

        const ptr = this.module.getPointer(this.uarm.GetSavestateData());
        return this.module.HEAPU8.subarray(ptr, ptr + this.uarm.GetSavestateSize());
    }

    @guard()
    getCurrentIps(): number {
        return this.uarm.CurrentIps();
    }

    @guard()
    getCurrentIpsMax(): number {
        return Number(this.uarm.CurrentIpsMax());
    }

    @guard()
    getSdCardKey(): string | undefined {
        return this.uarm.SdCardInitialized() ? this.uarm.GetSdCardId() : undefined;
    }

    @guard()
    sdCardInsert(data: Uint32Array, key: string): boolean {
        const ptr = this.copyIn(new Uint8Array(data.buffer, data.byteOffset, data.byteLength));

        if (!this.uarm.SdCardInsert(ptr, data.length * 4, key)) {
            this.bridge.Free(ptr);
            return false;
        }

        return true;
    }

    @guard()
    sdCardEject(): void {
        this.uarm.SdCardEject();
    }

    @guard()
    sdCardInserted(): boolean {
        return this.uarm.IsSdInserted();
    }

    @guard()
    clearSampleQueue(): void {
        this.uarm.PopQueuedSamples();
    }

    @guard()
    getQueuedSamples(): Uint32Array | undefined {
        const count = this.uarm.PendingSamples();
        if (count === 0) return undefined;

        const ptr = this.module.getPointer(this.uarm.PopQueuedSamples()) >>> 2;
        return this.module.HEAPU32.subarray(ptr, ptr + count);
    }

    @guard()
    getSampleQueueSize(): number {
        return this.uarm.GetSampleQueueSize();
    }

    @guard()
    disablePcm(disable: boolean): void {
        this.uarm.SetPcmOutputEnabled(!disable);
    }

    @guard()
    suspendPcm(suspend: boolean): void {
        this.uarm.SetPcmSuspended(suspend);
    }

    @guard()
    reset(): void {
        this.uarm.Reset();
    }

    @guard()
    jamButton(button: PalmButton, durationMsec: number): void {
        this.uarm.JamKey(mapButton(button), durationMsec);
    }

    @guard()
    installDb(data: Uint8Array): DbInstallResult {
        const ptr = this.copyIn(data);

        const result = this.uarm.InstallDatabase(data.length, ptr);
        this.bridge.Free(ptr);

        return mapDbInstallResult(result);
    }

    @guard()
    createDbBackup(includeRom: boolean): DbBackup | undefined {
        const nativeBackup = this.uarm.NewDbBackup(includeRom ? BackupType.ramRom : BackupType.ram);
        if (!nativeBackup.Init()) {
            this.module.destroy(nativeBackup);
            return undefined;
        }

        return {
            getState: () => this.guard(() => nativeBackup.GetState()),
            continue: () => this.guard(() => nativeBackup.Continue()),
            getLastProcessedDb: () =>
                this.guard(() => (nativeBackup.HasLastProcessedDb() ? nativeBackup.GetLastProcessedDb() : undefined)),
            getArchive: () =>
                this.guard(() => {
                    const ptr = this.module.getPointer(nativeBackup.GetArchiveData());
                    if (ptr === 0) return undefined;

                    return this.module.HEAPU8.subarray(ptr, ptr + nativeBackup.GetArchiveSize());
                }),
            destroy: () => this.guard(() => this.module.destroy(nativeBackup)),
        };
    }

    @guard()
    getRomData(): Uint8Array {
        const ptr = this.module.getPointer(this.uarm.GetRomData());

        return new Uint8Array(this.module.HEAPU8.subarray(ptr, ptr + this.uarm.GetRomDataSize()));
    }

    dead(): boolean {
        return this.amIdead;
    }

    guard<T>(fn: () => T) {
        if (this.amIdead) throw new Error('uarm instance is dead');

        try {
            return fn();
        } catch (e) {
            this.amIdead = true;
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            this.uarm = undefined as any;

            this.fatalErrorEvent.dispatch(e instanceof Error ? e : new Error('unknown error'));

            throw e;
        }
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const ptr = this.bridge.Malloc(data.length);
        const offset = this.module.getPointer(ptr);

        this.module.HEAPU8.subarray(offset, offset + data.length).set(data);

        return ptr;
    }

    private uarm: UarmNative;
    private bridge: Bridge;

    fatalErrorEvent = new Event<Error>();
    private amIdead = false;
}
