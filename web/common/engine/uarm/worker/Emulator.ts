import { Uarm } from '@common/bridge/Uarm';
import { EngineSettings } from '@common/engine/EngineSettings';
import { DeviceId } from '@common/model/DeviceId';
import { PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';
import 'setimmediate';

import { DirtyPageTracker } from './DirtyPageTracker';
import { UarmSnapshot } from './Snapshot';

interface TimesliceProperties {
    sizeSeconds: number;
    frame: ArrayBuffer | undefined;
}

export class Emulator {
    constructor(
        private uarm: Uarm,
        private settings: EngineSettings,
    ) {
        uarm.fatalErrorEvent.addHandler(() => this.stop());
    }

    openSession(
        rom: Uint8Array,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: [Uint8Array, string],
    ): boolean {
        // This is a slight hack, but I see no cleaner way to do this. If the session is new, then
        // memory will be undefined, and the logic in the app will default to
        // RomInfo5::GetRecommendedRamSize . The default in Uarm::launch is the same, which is
        // why this works.
        const ramSize = memory ? (memory.length >= 32 << 20 ? 32 << 20 : 16 << 20) : undefined;

        if (ramSize !== undefined) this.uarm.setRamSize(ramSize);
        if (nand) this.uarm.setNand(nand);
        if (memory) this.uarm.setMemory(memory);
        if (state) this.uarm.setSavestate(state);

        if (card) {
            const [cardData, key] = card;

            this.sdCardKey = key;
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

        this.pageTrackerNand = new DirtyPageTracker(4224, (this.uarm.getNandSize() / 4224) | 0, 'nand', false, {
            getData: () => this.uarm.getNandData(),
            getDirtyPages: () => this.uarm.getNandDirtyPages(),
            isDirty: () => this.uarm.isNandDirty(),
            setDirty: (dirty) => this.uarm.setNandDirty(dirty),
        });

        this.updatePageTrackerSd();

        return true;
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;

        this.uarm.setMaxHostLoad(settings.maxHostLoad);
        this.uarm.setDisablePcm(settings.disableAudio);
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

    start(): boolean {
        if (!this.isRunning() || !this.uarm.dead()) {
            this.immediateHandle = setImmediate(this.timesliceTask) as unknown as number;
        }

        return this.isRunning();
    }

    stop(): boolean {
        if (this.timeoutHandle !== undefined) clearTimeout(this.timeoutHandle);
        if (this.immediateHandle !== undefined) clearImmediate(this.immediateHandle);

        this.timeoutHandle = this.immediateHandle = undefined;

        return this.isRunning();
    }

    isRunning(): boolean {
        return this.timeoutHandle !== undefined || this.immediateHandle !== undefined;
    }

    returnFrame(frame: ArrayBuffer): void {
        this.framePool.push(frame);
    }

    returnSnapshot(snapshot: UarmSnapshot, success: boolean): void {
        if (!this.snapshotPending) return;

        this.pageTrackerMemory?.returnSnapshot(success, snapshot.memory);
        this.pageTrackerNand?.returnSnapshot(success, snapshot.nand);
        if (this.pageTrackerSd && snapshot.sd) this.pageTrackerSd.returnSnapshot(success, snapshot.sd.snapshot);

        this.savestate = new Uint8Array(snapshot.savestate);

        if (this.returnSnapshotCallbacks.length > 0) {
            this.returnSnapshotCallbacks.forEach((cb) => cb());
            this.returnSnapshotCallbacks = [];
        }

        this.snapshotPending = false;
    }

    takeSnapshot(): [UarmSnapshot, Array<Transferable>] | undefined {
        if (this.snapshotPending) return undefined;

        const timestampStart = Date.now();
        const savestate = this.uarm.saveState();

        if (!this.savestate) this.savestate = savestate.slice();
        else this.savestate.set(savestate);

        if (!this.pageTrackerMemory || !this.pageTrackerNand) throw new Error('unreachable');

        const snapshotMemory = this.pageTrackerMemory.takeSnapshot();
        const snapshotNand = this.pageTrackerNand.takeSnapshot();
        const snapshotSd = this.pageTrackerSd?.takeSnapshot();

        if (!snapshotMemory) throw new Error('incpmplete snapshot');

        const now = Date.now();
        const uarmSnaphost: UarmSnapshot = {
            memory: snapshotMemory,
            nand: snapshotNand,
            sd:
                snapshotSd && this.sdCardKey !== undefined
                    ? { snapshot: snapshotSd, key: this.sdCardKey, size: this.uarm.getSdCardSize() }
                    : undefined,
            savestate: this.savestate.buffer,
            time: now - timestampStart,
            timestamp: now,
        };

        const transferrables = [
            ...this.pageTrackerMemory.getTransferables(),
            ...this.pageTrackerNand.getTransferables(),
            ...(this.pageTrackerSd?.getTransferables() ?? []),
            this.savestate.buffer,
        ];

        this.snapshotPending = true;

        return [uarmSnaphost, transferrables];
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

        //        this.processAudio();

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

        this.timesliceEvent.dispatch({ sizeSeconds, frame: this.getFrame() });
    };

    private getFrame(): ArrayBuffer | undefined {
        const frame = this.uarm.getFrame(this.deviceId === DeviceId.frankene2 ? 480 : 320);
        if (!frame) return undefined;

        if (this.framePool.length === 0) {
            return frame.slice().buffer;
        } else {
            const frameCopy = new Uint32Array(this.framePool.pop()!);
            frameCopy.set(frame);

            return frameCopy.buffer;
        }
    }

    timesliceEvent = new Event<TimesliceProperties>();
    snapshotEvent = new Event<[UarmSnapshot, Array<Transferable>]>();

    private timeoutHandle: number | undefined;
    private immediateHandle: number | undefined;

    private framePool: Array<ArrayBuffer> = [];
    private deviceId = DeviceId.te2;

    private pageTrackerMemory: DirtyPageTracker | undefined;
    private pageTrackerNand: DirtyPageTracker | undefined;
    private pageTrackerSd: DirtyPageTracker | undefined;

    private sdCardKey: string | undefined;

    private savestate: Uint8Array | undefined;
    private snapshotPending = false;
    private lastSnapshotAt = 0;

    private returnSnapshotCallbacks: Array<() => void> = [];
}
