// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>
import { InstantiateFunction } from '@common/helper/wasm';
import createModule, {
    CardSupportLevel,
    Cloudpilot as CloudpilotNative,
    DbBackup,
    DbInstallResult,
    EmSerialTransport,
    GunzipContext,
    GunzipState,
    Module,
    PalmButton,
    SuspendContextClipboardCopy,
    SuspendContextClipboardPaste,
    SuspendContextNetworkConnect,
    SuspendContextNetworkRpc,
    SuspendContextSerialSync,
    SuspendKind,
    VoidPtr,
} from '@native/index';
import { Event, EventInterface } from 'microevent.ts';

import { DeviceId } from '../model/DeviceId';
import { ZipfileWalker, decorateZipfileWalker } from './ZipfileWalker';
import { dirtyPagesSize } from './util';

let nextId = 0;

export {
    PalmButton,
    DbBackup,
    DbInstallResult,
    SuspendKind,
    SuspendContextClipboardCopy,
    SuspendContextClipboardPaste,
    VoidPtr,
    CardSupportLevel,
} from '@native/index';

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

    firstDirtyLine: number;
    lastDirtyLine: number;
    hasChanges: boolean;

    scaleX: number;
    scaleY: number;

    buffer: Uint8Array;
}

export interface PwmUpdate {
    frequency: number;
    dutyCycle: number;
}

export interface SessionImage<T> {
    metadata?: T;
    deviceId: DeviceId;
    rom: Uint8Array;
    memory?: Uint8Array;
    savestate?: Uint8Array;
    version: number;
}

export const SUPPORTED_DEVICES = [
    DeviceId.palmPilot,
    DeviceId.pilot,
    DeviceId.iii,
    DeviceId.palmVx,
    DeviceId.palmV,
    DeviceId.palmVII,
    DeviceId.palmVIIEZ,
    DeviceId.palmVIIx,
    DeviceId.iiixe,
    DeviceId.iiix,
    DeviceId.iiie,
    DeviceId.m500,
    DeviceId.m505,
    DeviceId.m515,
    DeviceId.m520,
    DeviceId.iiic,
    DeviceId.m100,
    DeviceId.m105,
    DeviceId.m125,
    DeviceId.m130,
    DeviceId.i705,
    DeviceId.i710,
    DeviceId.handera330,
    DeviceId.handera330c,
    DeviceId.pegS300,
    DeviceId.pegS320,
    DeviceId.pegS500c,
    DeviceId.pegT400,
    DeviceId.pegN600c,
    DeviceId.pegT600c,
    DeviceId.pegN700c,
    DeviceId.pegT650c,
    DeviceId.pegNR70,
    DeviceId.acerS11,
    DeviceId.lp168,
];

export interface SerialTransport extends Omit<EmSerialTransport, 'Receive' | 'Send'> {
    Receive(): Uint8Array;
    Send(data: Uint8Array | undefined, isFrameComplete: boolean): void;

    frameCompleteEvent: EventInterface<void>;
}

function guard(): MethodDecorator {
    return (target: unknown, propertyKey: string | symbol, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = function (this: Cloudpilot, p1: unknown, p2: unknown) {
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
                'vdd',
            ),
        );

        this.cloudpilot.RegisterProxyDisconnectHandler(
            module.addFunction(() => this.proxyDisconnectEvent.dispatch(), 'v'),
        );

        this.transportIR = this.wrapTransport(this.cloudpilot.GetTransportIR());
        this.transportSerial = this.wrapTransport(this.cloudpilot.GetTransportSerial());
    }

    static async create(wasmModuleUrl?: string): Promise<Cloudpilot>;
    static async create(instantiateWasm?: InstantiateFunction): Promise<Cloudpilot>;
    static async create(urlOrFunction?: string | InstantiateFunction): Promise<Cloudpilot> {
        const id = nextId++;

        return new Cloudpilot(
            await createModule({
                print: (x: string) => console.log(`${id}: ${x}`),
                printErr: (x: string) => console.error(`${id}: ${x}`),
                ...(typeof urlOrFunction === 'string' ? { locateFile: () => urlOrFunction } : {}),
                ...(typeof urlOrFunction === 'function' ? { instantiateWasm: urlOrFunction } : {}),
            }),
        );
    }

    destroy(): void {
        this.module.destroy(this.cloudpilot);
    }

    hasFatalError(): boolean {
        return this.amIdead;
    }

    @guard()
    enableLogging(logging: boolean): void {
        this.cloudpilot.EnableLogging(logging);
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
            firstDirtyLine: nativeFrame.GetFirstDirtyLine(),
            lastDirtyLine: nativeFrame.GetLastDirtyLine(),
            hasChanges: nativeFrame.GetHasChanges(),
            scaleX: nativeFrame.GetScaleX(),
            scaleY: nativeFrame.GetScaleY(),
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
    framebufferSizeForDevice(id: DeviceId): number {
        return this.cloudpilot.FramebufferSizeForDevice(id) * 1024;
    }

    @guard()
    totalMemorySizeForDevice(id: DeviceId): number {
        return this.cloudpilot.TotalMemoryForDevice(id) * 1024;
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

        const size = (memorySize >>> 13) + (memorySize % 8192 === 0 ? 0 : 1);

        return this.module.HEAPU8.subarray(ptr, ptr + size);
    }

    @guard()
    loadMemory(memory: Uint32Array): boolean {
        const memory32 = this.getMemory32();

        const imagesMatch = (): boolean => {
            if (memory.length !== memory32.length || memory.length < 256) return false;

            for (let i = 256; i > 0; i--) {
                if (memory[memory.length - i] !== memory32[memory32.length - i]) return false;
                if (memory32[memory32.length - i] === 0xffffffff) break;
            }

            return true;
        };

        if (imagesMatch()) {
            memory32.set(memory);
            return true;
        } else {
            const ptr = this.copyIn32(memory);
            const result = this.cloudpilot.ImportMemoryImage(ptr, memory.length << 2);
            this.cloudpilot.Free(ptr);

            return result;
        }
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

        return this.copyOut(dbBackup.GetArchivePtr(), size);
    }

    @guard()
    setClipboardIntegration(toggle: boolean): void {
        this.cloudpilot.SetClipboardIntegration(toggle);
    }

    @guard()
    getClipboardIntegration(): boolean {
        return this.cloudpilot.GetClipboardIntegration();
    }

    @guard()
    isSuspended(): boolean {
        return this.cloudpilot.IsSuspended();
    }

    @guard()
    getSuspendKind(): SuspendKind | undefined {
        return this.cloudpilot.IsSuspended() ? this.cloudpilot.GetSuspendContext().GetKind() : undefined;
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

    @guard()
    setNetworkRedirection(toggle: boolean) {
        this.cloudpilot.SetNetworkRedirection(toggle);
    }

    @guard()
    getNetworkRedirection(): boolean {
        return this.cloudpilot.GetNetworkRedirection();
    }

    @guard()
    getSuspendContextNetworkConnect(): SuspendContextNetworkConnect {
        return this.wrap(this.cloudpilot.GetSuspendContext().AsContextNetworkConnect());
    }

    @guard()
    getSuspendContextNetworkRpc(): SuspendContextNetworkRpc {
        return this.wrap(this.cloudpilot.GetSuspendContext().AsContextNetworkRpc());
    }

    @guard()
    getSuspendContextSerialSync(): SuspendContextSerialSync {
        return this.wrap(this.cloudpilot.GetSuspendContext().AsContextSerialSync());
    }

    @guard()
    copyBuffer(data: Uint8Array): VoidPtr {
        return this.copyIn(data);
    }

    @guard()
    freeBuffer(ptr: VoidPtr): void {
        this.cloudpilot.Free(ptr);
    }

    @guard()
    setHotsyncNameManagement(toggle: boolean): void {
        this.cloudpilot.SetHotsyncNameManagement(toggle);
    }

    @guard()
    resolveBuffer(ptr: VoidPtr, size: number): Uint8Array {
        const bufferPtr = this.module.getPointer(ptr);

        return this.module.HEAPU8.subarray(bufferPtr, bufferPtr + size);
    }

    async withZipfileWalker<T>(buffer: Uint8Array, callback: (walker: ZipfileWalker) => Promise<T>): Promise<T> {
        const walker = this.guard(() => {
            const ptr = this.copyIn(buffer);
            const newWalker = new this.module.ZipfileWalker(buffer.length, ptr);

            this.cloudpilot.Free(ptr);

            return newWalker;
        });

        try {
            return await callback(decorateZipfileWalker(this.wrap(walker), this.module));
        } finally {
            this.module.destroy(walker);
        }
    }

    withZipfileWalkerSync<T>(buffer: Uint8Array, callback: (walker: ZipfileWalker) => T): T {
        const walker = this.guard(() => {
            const ptr = this.copyIn(buffer);
            const newWalker = new this.module.ZipfileWalker(buffer.length, ptr);

            this.cloudpilot.Free(ptr);

            return newWalker;
        });

        try {
            return callback(
                this.wrap(
                    Object.setPrototypeOf(
                        {
                            GetCurrentEntryContent: () =>
                                this.copyOut(walker.GetCurrentEntryContent(), walker.GetCurrentEntrySize()),
                        },
                        walker,
                    ),
                ),
            );
        } finally {
            this.module.destroy(walker);
        }
    }

    @guard()
    serializeSessionImage<T>(sessionImage: Omit<SessionImage<T>, 'version'>): Uint8Array | undefined {
        const nativeImage = new this.module.SessionImage();
        const nullptr = this.cloudpilot.Nullptr();

        const romImage = this.copyIn(sessionImage.rom);
        const memoryImage = sessionImage.memory ? this.copyIn(sessionImage.memory) : nullptr;
        const savestate = sessionImage.savestate !== undefined ? this.copyIn(sessionImage.savestate) : nullptr;

        let metadata = nullptr;
        let metadataLenght = 0;
        if (sessionImage.metadata !== undefined) {
            const serializedMetadata = new TextEncoder().encode(JSON.stringify(sessionImage.metadata));

            metadataLenght = serializedMetadata.length;
            metadata = this.copyIn(serializedMetadata);
        }

        nativeImage.SetRomImage(romImage, sessionImage.rom.length);
        nativeImage.SetMemoryImage(memoryImage, sessionImage.memory?.length || 0);
        nativeImage.SetDeviceId(sessionImage.deviceId);
        nativeImage.SetSavestate(savestate, sessionImage.savestate?.length || 0);
        nativeImage.SetMetadata(metadata, metadataLenght);

        const result = nativeImage.Serialize()
            ? this.copyOut(nativeImage.GetSerializedImage(), nativeImage.GetSerializedImageSize())
            : undefined;

        this.module.destroy(nativeImage);
        [romImage, memoryImage, savestate, metadata].forEach((buffer) => this.cloudpilot.Free(buffer));

        return result;
    }

    @guard()
    deserializeSessionImage<T>(buffer: Uint8Array | undefined): SessionImage<T> | undefined {
        if (!buffer) return undefined;

        const nativeImage = new this.module.SessionImage();
        const bufferPtr = this.copyIn(buffer);

        const result = ((): SessionImage<T> | undefined => {
            if (!nativeImage.Deserialize(bufferPtr, buffer.length)) return undefined;

            const rom = this.copyOut(nativeImage.GetRomImage(), nativeImage.GetRomImageSize())!;
            const memory = this.copyOut(nativeImage.GetMemoryImage(), nativeImage.GetMemoryImageSize())!;
            const savestate = this.copyOut(nativeImage.GetSavestate(), nativeImage.GetSavestateSize());

            const deviceId = nativeImage.GetDeviceId() as DeviceId;
            if (!SUPPORTED_DEVICES.includes(deviceId)) {
                console.warn(`unsupported device ${deviceId}`);
                return undefined;
            }

            let metadata: T | undefined;
            const serializedMetadata = this.copyOut(nativeImage.GetMetadata(), nativeImage.GetMetadataSize());

            if (serializedMetadata) {
                try {
                    metadata = JSON.parse(new TextDecoder().decode(serializedMetadata));
                } catch (e) {
                    console.warn('invalid metadata', e);
                }
            }

            return {
                deviceId,
                rom,
                memory,
                savestate,
                metadata,
                version: nativeImage.GetVersion(),
            };
        })();

        this.module.destroy(nativeImage);
        this.cloudpilot.Free(bufferPtr);

        return result;
    }

    @guard()
    launchAppByName(name: string): boolean {
        return this.cloudpilot.LaunchAppByName(name);
    }

    @guard()
    launchAppByDbHeader(header: Uint8Array): boolean {
        if (header.length < 32) return false;

        const headerPtr = this.copyIn(header.subarray(0, 32));
        const result = this.cloudpilot.LaunchAppByDbHeader(headerPtr, 32);
        this.cloudpilot.Free(headerPtr);

        return result;
    }

    @guard()
    deviceSupportsCardSize(size: number): boolean {
        return this.cloudpilot.DeviceSupportsCardSize(size);
    }

    @guard()
    clearExternalStorage(): void {
        this.cloudpilot.ClearExternalStorage();
    }

    @guard()
    allocateCard(key: string, size: number): boolean {
        return this.cloudpilot.AllocateCard(key, size >>> 9);
    }

    @guard()
    decompressCard(key: string, compressedData: Uint8Array): boolean {
        const buffer = this.copyIn(compressedData);
        let gunzipContext: GunzipContext | undefined = undefined;
        try {
            gunzipContext = new this.module.GunzipContext(buffer, compressedData.length, 512 * 1024);

            while (gunzipContext.GetState() === GunzipState.more) {
                gunzipContext.Continue();
            }

            if (gunzipContext.GetState() === GunzipState.error) {
                console.error(`gunzip failed: ${gunzipContext.GetError()}`);
                return false;
            }

            if (gunzipContext.GetUncompressedSize() % 512 !== 0) {
                console.error('failed to allocate card: invalid size of decompressed image');
                return false;
            }

            return this.cloudpilot.AdoptCard(
                key,
                gunzipContext.ReleaseUncompressedData(),
                gunzipContext.GetUncompressedSize() >>> 9,
            );
        } finally {
            this.cloudpilot.Free(buffer);
            if (gunzipContext) this.module.destroy(gunzipContext);
        }
    }

    @guard()
    getCardData(key: string): Uint32Array | undefined {
        const dataPtr = this.module.getPointer(this.cloudpilot.GetCardData(key));
        if (dataPtr === 0) return undefined;

        return this.module.HEAPU32.subarray(dataPtr >>> 2, (dataPtr + this.cloudpilot.GetCardSize(key)) >> 2);
    }

    @guard()
    getCardDirtyPages(key: string): Uint8Array | undefined {
        const dirtyPagesPtr = this.module.getPointer(this.cloudpilot.GetCardDirtyPages(key));
        if (dirtyPagesPtr === 0) return undefined;

        const bufferSize = dirtyPagesSize(this.cloudpilot.GetCardSize(key));
        return this.module.HEAPU8.subarray(dirtyPagesPtr, dirtyPagesPtr + bufferSize);
    }

    @guard()
    getMountedKey(): string {
        return this.cloudpilot.GetMountedKey();
    }

    @guard()
    mountCard(key: string): boolean {
        return this.cloudpilot.MountCard(key);
    }

    @guard()
    removeCard(key: string): boolean {
        return this.cloudpilot.RemoveCard(key);
    }

    @guard()
    remountCards() {
        this.cloudpilot.RemountCards();
    }

    @guard()
    getCardSupportLevel(size: number): CardSupportLevel {
        return this.cloudpilot.GetSupportLevel(size);
    }

    @guard()
    loadSkin(name: string): string | undefined {
        const skinLoader = new this.module.SkinLoader(name);

        try {
            const dataPtr = this.module.getPointer(skinLoader.GetData());
            if (dataPtr === 0) return undefined;

            const decoder = new TextDecoder();
            return decoder.decode(this.module.HEAPU8.subarray(dataPtr, dataPtr + skinLoader.GetSize()));
        } finally {
            this.module.destroy(skinLoader);
        }
    }

    getTransportIR(): SerialTransport {
        return this.transportIR;
    }

    getTransportSerial(): SerialTransport {
        return this.transportSerial;
    }

    public guard<T>(fn: () => T) {
        if (this.amIdead) throw new Error('cloudpilot instance is dead');

        try {
            return fn();
        } catch (e) {
            this.amIdead = true;
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            this.cloudpilot = undefined as any;

            this.fatalErrorEvent.dispatch(e instanceof Error ? e : new Error('unknown error'));

            throw e;
        }
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const buffer = this.cloudpilot.Malloc(data.length);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAPU8.subarray(bufferPtr, bufferPtr + data.length).set(data);

        return buffer;
    }

    private copyIn32(data: Uint32Array): VoidPtr {
        const buffer = this.cloudpilot.Malloc(data.length << 2);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAPU32.subarray(bufferPtr >>> 2, (bufferPtr >>> 2) + data.length).set(data);

        return buffer;
    }

    private copyOut(ptr: VoidPtr, size: number): Uint8Array | undefined {
        const p = this.module.getPointer(ptr);

        return p > 0 && size > 0 ? this.module.HEAPU8.subarray(p, p + size).slice() : undefined;
    }

    private wrap<T extends object>(unguarded: T): T {
        return new Proxy<T>(unguarded, {
            get: (target: T, p: string) => {
                const val = target[p as keyof T];

                if (typeof val !== 'function') {
                    return val;
                }

                // eslint-disable-next-line @typescript-eslint/no-unsafe-function-type
                return (...args: Array<unknown>) => this.guard(() => (val as Function).apply(target, args));
            },
        });
    }

    private wrapTransport(transport: EmSerialTransport): SerialTransport {
        const frameCompleteEvent = new Event<void>();
        transport.SetRequestTransferCallback(this.module.addFunction(() => frameCompleteEvent.dispatch(), 'v'));

        return this.wrap(
            Object.setPrototypeOf(
                {
                    Receive: (): Uint8Array | undefined => {
                        const count = transport.RxBytesPending();

                        return this.copyOut(transport.Receive(), count);
                    },

                    Send: (data: Uint8Array | undefined, frameComplete: boolean): void => {
                        const buffer = data && this.copyIn(data);

                        transport.Send(data?.length ?? 0, buffer ?? this.cloudpilot.Nullptr(), frameComplete);

                        if (buffer) this.cloudpilot.Free(buffer);
                    },

                    frameCompleteEvent,
                },
                transport,
            ),
        );
    }

    fatalErrorEvent = new Event<Error>();
    pwmUpdateEvent = new Event<PwmUpdate>();
    proxyDisconnectEvent = new Event<void>();

    private cloudpilot: CloudpilotNative;

    private transportIR: SerialTransport;
    private transportSerial: SerialTransport;

    private amIdead = false;
}
