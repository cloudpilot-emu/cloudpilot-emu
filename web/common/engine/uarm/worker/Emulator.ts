import { DbInstallResult, PalmButton } from '@common/bridge/Cloudpilot';
import { BackupState, Uarm } from '@common/bridge/Uarm';
import { BackupResult, FullState } from '@common/engine/Engine';
import { EngineSettings } from '@common/engine/EngineSettings';
import { DeviceId } from '@common/model/DeviceId';
import {
    StreamMessageClient,
    StreamMessageClientType,
    StreamMessageHost,
    StreamMessageHostType,
} from '@common/service/audioworklet/messages';
import { Event } from 'microevent.ts';
import 'setimmediate';

import { DirtyPageTracker } from './DirtyPageTracker';
import { ResetType } from './ResetType';
import { UarmSnapshot } from './Snapshot';

interface TimesliceProperties {
    sizeSeconds: number;
    lcdEnabled: boolean;
    frame: ArrayBuffer | undefined;

    currentIps: number;
    currentIpsMax: number;
}

export interface SystemState {
    uiInitialized: boolean;
    osVersion: number | undefined;
}

const MAX_PCM_SUSPEND_MSEC = 500;
const MAX_PCM_BUFFERS_IN_FLIGHT = 5;
const MAX_PCM_WAIT_FOR_BUFFER_MSEC = 5000;

/*
 * Determine RAM size in MB from memory size. This exploits the fact that RAM
 * size is always a power-of-two multiple of 1MB, and memory size is only a
 * a few (16) kilobtes of extra memory.
 */
function ramSizeFromMemorySize(memorySize: number): number | undefined {
    if (memorySize === 0) return undefined;
    const log2 = 31 - Math.clz32(memorySize);

    return log2 > 20 ? log2 - 20 : undefined;
}

export class Emulator {
    constructor(
        private uarm: Uarm,
        private settings: EngineSettings,
        private pcmPort: MessagePort,
    ) {
        uarm.fatalErrorEvent.addHandler(() => this.stop());

        this.sampleQueueSize = uarm.getSampleQueueSize();
        this.pcmPort.addEventListener('message', this.onPcmPortMessage);
        this.pcmPort.start();
    }

    openSession(
        rom: Uint8Array,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: [Uint8Array, string],
    ): boolean {
        let ramSize: number | undefined = undefined;
        if (memory) {
            ramSize = ramSizeFromMemorySize(memory.length);

            if (ramSize === undefined) {
                console.error(`unable to determine RAM size from memory size ${memory.length} bytes`);
                return false;
            }
        }

        if (ramSize !== undefined) this.uarm.setRamSize(ramSize);
        if (nand) this.uarm.setNand(nand);
        if (memory) this.uarm.setMemory(memory);
        if (state) this.uarm.setSavestate(state);

        if (card) {
            const [cardData, key] = card;
            this.uarm.setSd(cardData, key);
        }

        if (!this.uarm.launch(rom)) return false;

        this.deviceId = this.uarm.getDevice();

        this.pageTrackerMemory = new DirtyPageTracker(
            1024,
            this.uarm.getMemorySize() >>> 10,
            'memory',
            this.settings.memoryCrc,
            {
                getData: () => this.uarm.getMemoryData(),
                getDirtyPages: () => this.uarm.getMemoryDirtyPages(),
                isDirty: () => true,
                setDirty: () => undefined,
            },
        );

        const nandSize = this.uarm.getNandSize();
        if (nandSize > 0) {
            this.pageTrackerNand = new DirtyPageTracker(4224, (this.uarm.getNandSize() / 4224) | 0, 'nand', false, {
                getData: () => this.uarm.getNandData(),
                getDirtyPages: () => this.uarm.getNandDirtyPages(),
                isDirty: () => this.uarm.isNandDirty(),
                setDirty: (dirty) => this.uarm.setNandDirty(dirty),
            });
        }

        this.updatePageTrackerSd();

        this.uarmInitialized = true;
        this.updateSettings(this.settings);

        return true;
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
        if (!this.uarmInitialized) return;

        this.uarm.setMaxHostLoad(settings.maxHostLoad);
        this.uarm.disablePcm(settings.disableAudio);
        this.uarm.setTargetMips(settings.targetMips);
        this.pageTrackerMemory?.enableCrc(settings.memoryCrc);
    }

    buttonUp(button: PalmButton): void {
        this.uarm.buttonUp(button);
    }

    buttonDown(button: PalmButton): void {
        this.uarm.buttonDown(button);
    }

    penDown(x: number, y: number): void {
        this.uarm.penDown(x, y);
    }

    penUp(): void {
        this.uarm.penUp();
    }

    start(): void {
        this.running = true;
        this.updateRunState();
    }

    stop(): void {
        this.running = false;
        this.updateRunState();
    }

    setBackgrounded(backgrounded: boolean): void {
        this.backgrounded = backgrounded;

        this.updateRunState();
    }

    returnFrame(frame: ArrayBuffer): void {
        this.framePool.push(frame);
    }

    returnSnapshot(snapshot: UarmSnapshot, success: boolean): void {
        if (!this.snapshotPending) return;

        try {
            this.returnSnapshotUnguarded(snapshot, success);

            if (this.returnSnapshotCallbacks.length > 0) {
                this.returnSnapshotCallbacks.forEach((cb) => cb());
                this.returnSnapshotCallbacks = [];
            }
        } finally {
            this.snapshotPending = false;
        }
    }

    takeSnapshot(): [UarmSnapshot, Array<Transferable>] | undefined {
        if (this.snapshotPending) return undefined;

        try {
            return this.takeSnapshotUnguarded();
        } finally {
            this.snapshotPending = true;
        }
    }

    refreshSnapshot(snapshot: UarmSnapshot): [UarmSnapshot, Array<Transferable>] {
        if (!this.snapshotPending) throw new Error('refresh snapshot called, but no pending snapshot!');

        this.returnSnapshotUnguarded(snapshot, false);

        return this.takeSnapshotUnguarded(snapshot.timestamp, snapshot.time);
    }

    async waitForPendingSnapshot(): Promise<void> {
        if (!this.snapshotPending) return;

        return new Promise((resolve) => this.returnSnapshotCallbacks.push(resolve));
    }

    getMemorySize(): number {
        return this.uarm.getMemorySize();
    }

    getNandSize(): number {
        return this.uarm.getNandSize();
    }

    sdCardInsert(data: Uint32Array, key: string): boolean {
        const success = this.uarm.sdCardInsert(data, key);
        this.updatePageTrackerSd();

        return success;
    }

    sdCardEject(): void {
        this.uarm.sdCardEject();
        this.updatePageTrackerSd();
    }

    sdCardInserted(): boolean {
        return this.uarm.sdCardInserted();
    }

    getSdCardKey(): string | undefined {
        return this.uarm.getSdCardKey();
    }

    reset(resetType: ResetType): void {
        switch (resetType) {
            case ResetType.normal:
                break;

            case ResetType.noExtensions:
                this.uarm.jamButton(PalmButton.up, 2000);
                break;

            case ResetType.hard:
                this.uarm.jamButton(PalmButton.power, 2000);
                break;

            default:
                resetType satisfies never;
        }

        this.uarm.reset();
    }

    installDb(data: Uint8Array): DbInstallResult {
        return this.uarm.installDb(data);
    }

    backup(includeRom: boolean): BackupResult | undefined {
        const backup = this.uarm.createDbBackup(includeRom);
        if (!backup) return;

        try {
            const failedDatabases: Array<string> = [];
            while (backup.getState() === BackupState.inProgress) {
                if (!backup.continue()) {
                    const lastProcessed = backup.getLastProcessedDb();
                    if (lastProcessed !== undefined) failedDatabases.push(lastProcessed);
                }
            }

            const archive = backup.getArchive();
            return archive ? { archive: archive.slice(), failedDatabases } : undefined;
        } finally {
            backup.destroy();
        }
    }

    getSdCardData(): Uint32Array | undefined {
        const data = this.uarm.getSdCardData();

        return data ? new Uint32Array(data.buffer, data.byteOffset, data.byteLength >>> 2).slice() : undefined;
    }

    getFullState(): FullState {
        return {
            rom: this.uarm.getRomData().slice(),
            memory: this.uarm.getMemoryData().slice(),
            nand: this.uarm.getNandSize() > 0 ? this.uarm.getNandData().slice() : undefined,
            savestate: this.uarm.saveState().slice(),
        };
    }

    enablePcmStreaming(): void {
        if (this.pcmStreaming) return;
        this.pcmStreaming = true;

        this.dispatchPcmPortMessage({ type: StreamMessageHostType.flush });
        this.suspendPcm(false);
    }

    disablePcmStreaming(): void {
        if (!this.pcmStreaming) return;
        this.pcmStreaming = false;

        this.suspendPcm(false);
    }

    launchAppByName(name: string): boolean {
        return this.uarm.launchAppByName(name);
    }

    launchAppByHeader(header: Uint8Array): boolean {
        return this.uarm.launchAppByDbHeader(header);
    }

    private takeSnapshotUnguarded(timestamp?: number, timeOffset = 0): [UarmSnapshot, Array<Transferable>] {
        const timestampStart = Date.now();
        const savestate = this.uarm.saveState();

        if (!this.savestate) this.savestate = savestate.slice();
        else this.savestate.set(savestate);

        if (!this.pageTrackerMemory) throw new Error('unreachable');

        const snapshotMemory = this.pageTrackerMemory.takeSnapshot();
        const snapshotNand = this.pageTrackerNand?.takeSnapshot();
        const snapshotSd = this.pageTrackerSd?.takeSnapshot();

        const now = Date.now();

        const sdCardKey = this.uarm.getSdCardKey();
        const uarmSnaphost: UarmSnapshot = {
            memory: snapshotMemory,
            nand: snapshotNand,
            sd:
                snapshotSd && sdCardKey !== undefined
                    ? { snapshot: snapshotSd, key: sdCardKey, size: this.uarm.getSdCardSize() }
                    : undefined,
            savestate: this.savestate.buffer,
            time: now - timestampStart + timeOffset,
            timestamp: timestamp ?? now,
        };

        const transferrables = [
            ...this.pageTrackerMemory.getTransferables(),
            ...(this.pageTrackerNand?.getTransferables() ?? []),
            ...(this.pageTrackerSd?.getTransferables() ?? []),
            this.savestate.buffer,
        ];

        return [uarmSnaphost, transferrables];
    }

    private returnSnapshotUnguarded(snapshot: UarmSnapshot, success: boolean): void {
        this.pageTrackerMemory?.returnSnapshot(success, snapshot.memory);
        if (this.pageTrackerNand && snapshot.nand) this.pageTrackerNand.returnSnapshot(success, snapshot.nand);
        if (this.pageTrackerSd && snapshot.sd) this.pageTrackerSd.returnSnapshot(success, snapshot.sd.snapshot);

        this.savestate = new Uint8Array(snapshot.savestate);
    }

    private updateRunState(): void {
        if (this.uarm.dead()) return;

        const shoulBeRunning = this.running && (!this.backgrounded || this.settings.runInBackground);

        if (shoulBeRunning && this.timeoutHandle === undefined && this.immediateHandle === undefined) {
            this.immediateHandle = setImmediate(this.timesliceTask) as unknown as number;

            this.dispatchPcmPortMessage({ type: StreamMessageHostType.flush });
            this.suspendPcm(false);

            if (this.pcmWaitingForBuffersDrain) {
                this.pcmLastBufferSentAt = performance.now();
            }
        }

        if (!shoulBeRunning) {
            if (this.timeoutHandle !== undefined) clearTimeout(this.timeoutHandle);
            if (this.immediateHandle !== undefined) clearImmediate(this.immediateHandle);

            this.timeoutHandle = this.immediateHandle = undefined;
        }
    }

    private updatePageTrackerSd(): void {
        this.pageTrackerSd = undefined;
        if (this.uarm.getSdCardSize() === 0) return;

        this.pageTrackerSd = new DirtyPageTracker(1 << 13, this.uarm.getSdCardSize() >>> 13, 'sdcard', false, {
            getData: () => this.uarm.getSdCardData(),
            getDirtyPages: () => this.uarm.getSdCardDirtyPages(),
            isDirty: () => this.uarm.isSdCardDirty(),
            setDirty: (dirty) => this.uarm.setSdCardDirty(dirty),
        });
    }

    private timesliceTask = () => {
        const now64 = this.uarm.getTimestampUsec();
        const now = Number(now64);

        const sizeSeconds = this.uarm.cycle(now64);

        const timesliceRemaining =
            (this.uarm.getTimesliceSizeUsec() - Number(this.uarm.getTimestampUsec()) + now) / 1000;
        this.timeoutHandle = this.immediateHandle = undefined;

        if (timesliceRemaining < 5) this.immediateHandle = setImmediate(this.timesliceTask) as unknown as number;
        else this.timeoutHandle = setTimeout(this.timesliceTask, timesliceRemaining) as unknown as number;

        const timestamp = performance.now();
        if (
            this.settings.automaticSnapshotInterval > 0 &&
            timestamp - this.lastSnapshotAt > this.settings.automaticSnapshotInterval &&
            !this.snapshotPending
        ) {
            const snapshot = this.takeSnapshot();
            if (snapshot) this.snapshotEvent.dispatch(snapshot);

            this.lastSnapshotAt = timestamp;
        }

        this.processSamples(sizeSeconds);
        this.updateSystemState();

        this.timesliceEvent.dispatch({
            sizeSeconds,
            lcdEnabled: this.uarm.isLcdEnabled(),
            frame: this.backgrounded ? undefined : this.getFrame(),
            currentIps: this.uarm.getCurrentIps(),
            currentIpsMax: this.uarm.getCurrentIpsMax(),
        });
    };

    private processSamples(timesliceSizeSeconds: number): void {
        const now = performance.now();

        if (this.pcmWaitingForBuffersDrain && now - this.pcmLastBufferSentAt >= MAX_PCM_WAIT_FOR_BUFFER_MSEC) {
            this.pcmBuffersInFlight = 0;
            this.pcmWaitingForBuffersDrain = false;
        }

        if (this.pcmBuffersInFlight >= MAX_PCM_BUFFERS_IN_FLIGHT) {
            this.pcmWaitingForBuffersDrain = true;
        }

        if (this.pcmWaitingForBuffersDrain && this.pcmBuffersInFlight === 0) {
            this.pcmWaitingForBuffersDrain = false;
        }

        if (!this.pcmStreaming || this.settings.disableAudio || this.pcmWaitingForBuffersDrain) {
            this.uarm.clearSampleQueue();
            return;
        }

        if (this.pcmSuspended) {
            this.pcmSuspendedForMsec += timesliceSizeSeconds * 1000;
            if (this.pcmSuspendedForMsec > MAX_PCM_SUSPEND_MSEC) this.suspendPcm(false);
        }

        const samples = this.uarm.getQueuedSamples();
        if (!samples) return;

        const sampleBuffer = this.getSampleBuffer();
        sampleBuffer.set(samples);

        this.dispatchPcmPortMessage(
            {
                type: StreamMessageHostType.sampleData,
                buffer: sampleBuffer.buffer,
                count: samples.length,
            },
            [sampleBuffer.buffer],
        );

        this.pcmLastBufferSentAt = now;
        this.pcmBuffersInFlight++;
    }

    private updateSystemState(): void {
        const oldUiInitialized = this.uiInitialized;
        this.uiInitialized = this.uarm.isUiInitialized();

        if (this.uiInitialized === oldUiInitialized) return;

        this.osVersion = this.uarm.getOsVersion();

        this.systemStateChangeEvent.dispatch({
            osVersion: this.osVersion,
            uiInitialized: this.uiInitialized,
        });
    }

    private getSampleBuffer(): Uint32Array {
        if (this.sampleBufferPool.length > 0) return new Uint32Array(this.sampleBufferPool.pop()!);

        return new Uint32Array(this.sampleQueueSize);
    }

    private getFrame(): ArrayBuffer | undefined {
        // CSTODO: resolution fudge
        const frame = this.uarm.getFrame(this.deviceId === DeviceId.te2 ? 320 : 480);
        if (!frame) return undefined;

        let buffer: ArrayBuffer;

        if (this.framePool.length === 0) {
            buffer = frame.slice().buffer;
        } else {
            const frameCopy = new Uint32Array(this.framePool.pop()!);
            frameCopy.set(frame);

            buffer = frameCopy.buffer;
        }

        this.uarm.resetFrame();
        return buffer;
    }

    private suspendPcm(suspended: boolean): void {
        this.uarm.suspendPcm(suspended);

        this.pcmSuspended = suspended;
        if (suspended) this.pcmSuspendedForMsec = 0;
    }

    private dispatchPcmPortMessage(message: StreamMessageHost, transferables?: Array<Transferable>): void {
        this.pcmPort.postMessage(message, transferables as Array<Transferable>);
    }

    private onPcmPortMessage = (e: MessageEvent): void => {
        if (!this.pcmStreaming || this.settings.disableAudio) return;

        const message: StreamMessageClient = e.data;
        switch (message.type) {
            case StreamMessageClientType.resumePcm:
                this.suspendPcm(false);

                break;

            case StreamMessageClientType.suspendPcm:
                this.uarm.suspendPcm(true);

                break;

            case StreamMessageClientType.returnBuffer:
                this.sampleBufferPool.push(message.buffer);
                this.pcmBuffersInFlight--;

                break;

            default:
                message satisfies never;
        }
    };

    timesliceEvent = new Event<TimesliceProperties>();
    snapshotEvent = new Event<[UarmSnapshot, Array<Transferable>]>();
    systemStateChangeEvent = new Event<SystemState>();

    private running = false;
    private backgrounded = false;
    private uarmInitialized = false;

    private timeoutHandle: number | undefined;
    private immediateHandle: number | undefined;

    private framePool: Array<ArrayBuffer> = [];
    private deviceId = DeviceId.te2;

    private pageTrackerMemory: DirtyPageTracker | undefined;
    private pageTrackerNand: DirtyPageTracker | undefined;
    private pageTrackerSd: DirtyPageTracker | undefined;

    private savestate: Uint8Array | undefined;
    private snapshotPending = false;
    private lastSnapshotAt = 0;

    private readonly sampleQueueSize: number;
    private pcmStreaming = false;
    private pcmSuspended = false;
    private pcmSuspendedForMsec = 0;
    private sampleBufferPool: Array<ArrayBufferLike> = [];
    private pcmBuffersInFlight = 0;
    private pcmLastBufferSentAt = 0;
    private pcmWaitingForBuffersDrain = false;

    private returnSnapshotCallbacks: Array<() => void> = [];

    private uiInitialized = false;
    private osVersion: number | undefined;
}
