import { Cloudpilot, PalmButton, PwmUpdate, SuspendKind } from '@common/bridge/Cloudpilot';
import { Average } from '@common/helper/Average';
import { Fifo } from '@common/helper/Fifo';
import { deviceDimensions } from '@common/helper/deviceProperties';
import { GRAYSCALE_PALETTE_RGBA } from '@common/helper/palette';
import { AnimationFrameScheduler, Scheduler, SchedulerKind, TimeoutScheduler } from '@common/helper/scheduler';
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsCloudpilot } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { SerialPort } from '@common/serial/SerialPort';
import { Executor } from '@common/service/AbstractEmulationService';
import { Event, EventInterface } from 'microevent.ts';

import { EngineCloudpilot, StorageCardProvider } from '../Engine';
import { EngineSettings } from '../EngineSettings';
import { SnapshotContainer } from '../Snapshot';
import { SerialPortImpl } from './SerialPort';
import { SnapshotContainerImpl } from './Snapshot';

const PWM_FIFO_SIZE = 10;
const SPEED_AVERAGE_N = 20;
const TIME_PER_FRAME_AVERAGE_N = 60;
const SERIAL_SYNC_TIMEOUT_MSEC = 250;
const MIN_FPS = 30;
const DUMMY_SPEED = 1000;
const MIN_MILLISECONDS_PER_PWM_UPDATE = 10;

export class EngineCloudpilotImpl implements EngineCloudpilot {
    readonly type = 'cloudpilot';

    constructor(
        private cloudpilotInstance: Cloudpilot,
        private clandestineExecute: Executor,
        private settings: EngineSettings,
    ) {
        this.snapshotContainer = new SnapshotContainerImpl(cloudpilotInstance);

        cloudpilotInstance.clearExternalStorage();

        this.serialPortIR.bind(cloudpilotInstance.getTransportIR());
        this.serialPortSerial.bind(cloudpilotInstance.getTransportSerial());

        this.serialPortIR.dispatchEvent.addHandler(this.onSerialPortDispatch);
        this.serialPortSerial.dispatchEvent.addHandler(this.onSerialPortDispatch);

        cloudpilotInstance.pwmUpdateEvent.addHandler(this.onPwmUpdate);
    }

    shutdown(): void {
        this.serialPortIR.unbind();
        this.serialPortSerial.unbind();

        this.serialPortIR.dispatchEvent.removeHandler(this.onSerialPortDispatch);
        this.serialPortSerial.dispatchEvent.removeHandler(this.onSerialPortDispatch);

        this.cloudpilotInstance.pwmUpdateEvent.removeHandler(this.onPwmUpdate);
    }

    get fatalError(): EventInterface<Error> {
        return (this, this.cloudpilotInstance.fatalErrorEvent);
    }

    get snapshotSuccessEvent(): EventInterface<SnapshotStatistics> {
        return this.snapshotContainer.snapshotSuccessEvent;
    }

    penDown(x: number, y: number): void {
        this.cloudpilotInstance.queuePenMove(x, y);
    }

    penUp(): void {
        this.cloudpilotInstance.queuePenUp();
    }

    buttonDown(button: PalmButton): void {
        this.cloudpilotInstance.queueButtonDown(button);
    }

    buttonUp(button: PalmButton): void {
        this.cloudpilotInstance.queueButtonUp(button);
    }

    keyStroke(key: number, ctrl: boolean = false): void {
        this.cloudpilotInstance.queueKeyboardEvent(key, ctrl);
    }

    async reset(): Promise<void> {
        this.cloudpilotInstance.reset();
    }

    async resetNoExtensions(): Promise<void> {
        this.cloudpilotInstance.resetNoExtensions();
    }

    async resetHard(): Promise<void> {
        this.cloudpilotInstance.resetHard();
    }

    isRunning(): boolean {
        return this.running;
    }

    isPowerOff(): boolean {
        return this.powerOff;
    }

    isUIInitialized(): boolean {
        return this.uiInitialized;
    }

    isSuspended(): boolean {
        return this.suspended;
    }

    isSlowdown(): boolean {
        return this.emulationSpeed < 1;
    }

    getDeviceId(): DeviceId {
        return this.deviceId;
    }

    async openSession(
        rom: Uint8Array,
        device: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: StorageCardProvider,
    ): Promise<boolean> {
        if (nand) throw new Error('Cloudpilot devices do not support NAND');

        let memoryLoaded = false;

        this.cloudpilotInstance.clearExternalStorage();
        if (card) await card.load(this);

        if (!this.cloudpilotInstance.initializeSession(rom, device)) return false;

        if (memory) {
            if (memory.length % 4 !== 0) {
                console.warn(`memory size is not a multiple of 4; ignoring image`);
            } else {
                memoryLoaded = this.cloudpilotInstance.loadMemory(
                    new Uint32Array(memory.buffer, memory.byteOffset, memory.length >>> 2),
                );

                if (!memoryLoaded) {
                    console.warn('failed to load memory');
                }
            }
        }

        if (memoryLoaded && state) {
            this.cloudpilotInstance.loadState(state);
        }

        this.cloudpilotInstance.remountCards();
        if (this.cloudpilotInstance.getMountedKey() === undefined && card && !this.cloudpilotInstance.remountFailed()) {
            // Card was not remounted, and there was no error? -> card was not mounted in the last saved state of the
            // emulator, so mount it now.
            this.cloudpilotInstance.mountCard(card.storageKey);
        }

        this.deviceId = device;

        this.speedAverage.reset(1);
        this.powerOff = this.cloudpilotInstance.isPowerOff();
        this.uiInitialized = this.cloudpilotInstance.isUiInitialized();

        const dimensions = deviceDimensions(device);
        this.canvasTmp.width = dimensions.width;
        this.canvasTmp.height = dimensions.height;

        this.snapshotContainer.initializeMemory();

        return true;
    }

    async resume(): Promise<void> {
        if (this.running) return;

        this.clockEmulator = performance.now();

        this.schedule();

        this.running = true;
        this.resetFPS();
    }

    async stop(): Promise<void> {
        if (!this.running) return;

        this.scheduler.cancel();

        if (this.advanceEmulationHandle !== undefined) {
            window.clearTimeout(this.advanceEmulationHandle);

            this.advanceEmulationHandle = undefined;
        }

        this.running = false;
    }

    processTimesliceNow(): void {
        this.advanceEmulation(performance.now());
    }

    async requestSnapshot(): Promise<SnapshotContainer> {
        return this.snapshotContainer.schedule(this.settings.memoryCrc);
    }

    blitFrame(canvas: HTMLCanvasElement): void {
        if (!this.cloudpilotInstance.isScreenDirty() || this.cloudpilotInstance.isPowerOff()) return;

        const frame = this.cloudpilotInstance.getFrame();

        if (!frame.hasChanges) {
            return;
        }

        if (frame.lines * frame.scaleY === canvas.height && frame.lineWidth * frame.scaleX === canvas.width) {
            if (!this.imageData || this.imageData.width !== frame.lineWidth || this.imageData.height !== frame.lines) {
                this.imageData = new ImageData(frame.lineWidth, frame.lines);
            }

            const imageData32 = new Uint32Array(this.imageData.data.buffer);

            switch (frame.bpp) {
                case 1: {
                    const fg = GRAYSCALE_PALETTE_RGBA[15];
                    const bg = GRAYSCALE_PALETTE_RGBA[0];
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                frame.buffer[offsetSrc + (i >>> 3)] & (0x80 >>> (i & 0x07)) ? fg : bg;
                        }
                    }

                    break;
                }

                case 2: {
                    const mapping = this.cloudpilotInstance.getPalette2bitMapping();
                    const palette = [
                        GRAYSCALE_PALETTE_RGBA[mapping & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 4) & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 8) & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 12) & 0x000f],
                    ];
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                palette[(frame.buffer[offsetSrc + (i >>> 2)] >> (6 - 2 * (i & 0x03))) & 0x03];
                        }
                    }

                    break;
                }

                case 4: {
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                GRAYSCALE_PALETTE_RGBA[
                                    (frame.buffer[offsetSrc + (i >>> 1)] >>> (4 - 4 * (i & 0x01))) & 0x0f
                                ];
                        }
                    }

                    break;
                }

                case 24:
                    {
                        const buffer32 = new Uint32Array(
                            frame.buffer.buffer,
                            frame.buffer.byteOffset,
                            frame.buffer.byteLength >>> 2,
                        );

                        if (frame.margin === 0) {
                            imageData32
                                .subarray(0, frame.lineWidth * (frame.lastDirtyLine - frame.firstDirtyLine + 1))
                                .set(
                                    buffer32.subarray(
                                        frame.firstDirtyLine * frame.lineWidth,
                                        frame.lineWidth * (frame.lastDirtyLine + 1),
                                    ),
                                );
                        } else {
                            let iDest = 0;

                            for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                                let j = y * (frame.bytesPerLine >>> 2) + frame.margin;

                                for (let x = 0; x < frame.lineWidth; x++) {
                                    imageData32[iDest++] = buffer32[j++];
                                }
                            }
                        }
                    }

                    break;
            }
        }

        if (!this.imageData) return;

        const scaling = frame.scaleX !== 1 || frame.scaleY !== 1;

        const context = canvas.getContext('2d');
        const contextTmp = scaling ? this.canvasTmp.getContext('2d') : undefined;

        if (!context || (scaling && !contextTmp)) throw new Error('unable to retrieve rendering context');

        (scaling ? contextTmp! : context).putImageData(
            this.imageData,
            0,
            frame.firstDirtyLine,
            0,
            0,
            frame.lineWidth,
            frame.lastDirtyLine - frame.firstDirtyLine + 1,
        );

        if (scaling) {
            context.imageSmoothingEnabled = false;
            context.drawImage(
                this.canvasTmp,
                0,
                frame.firstDirtyLine,
                frame.lineWidth,
                frame.lastDirtyLine - frame.firstDirtyLine + 1,
                0,
                frame.firstDirtyLine * frame.scaleY,
                frame.lineWidth * frame.scaleX,
                (frame.lastDirtyLine - frame.firstDirtyLine + 1) * frame.scaleY,
            );
        }

        this.cloudpilotInstance.markScreenClean();
        this.frameCounter++;
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
    }

    allocateCard(key: string, size: number): Uint32Array {
        if (!this.cloudpilotInstance.allocateCard(key, size)) {
            throw new Error(`failed to allocate card "${key}" @ ${size} bytes`);
        }

        const data = this.cloudpilotInstance.getCardData(key);
        if (!data) throw new Error(`failed to get card data for key ${key}`);

        return data;
    }

    async releaseCard(key: string): Promise<void> {
        if (!this.cloudpilotInstance.removeCard(key)) {
            console.warn(`failed to release card for key ${key}`);
        }
    }

    async getMountedKey(): Promise<string | undefined> {
        return this.cloudpilotInstance.getMountedKey();
    }

    async getCardData(key: string): Promise<Uint32Array | undefined> {
        return this.cloudpilotInstance.getCardData(key);
    }

    async mountCard(key: string): Promise<boolean> {
        return this.cloudpilotInstance.mountCard(key);
    }

    getSerialPortSerial(): SerialPort {
        return this.serialPortSerial;
    }

    getSerialPortIR(): SerialPort {
        return this.serialPortIR;
    }

    getStatistics(): EmulationStatisticsCloudpilot {
        return {
            type: 'cloudpilot',
            hostSpeed: this.speedAverage.calculateAverage(),
            emulationSpeed: this.emulationSpeed,
            averageFps: this.calculateFPS(),
        };
    }

    getOSVersion(): number {
        return this.cloudpilotInstance.getOSVersion();
    }

    private schedule() {
        this.scheduler.cancel();

        if (this.settings.schedulerKind !== this.scheduler.getKind()) {
            this.scheduler =
                this.settings.schedulerKind === SchedulerKind.animationFrame
                    ? new AnimationFrameScheduler((timestamp) => this.onSchedule(timestamp))
                    : new TimeoutScheduler((timestamp) => this.onSchedule(timestamp));
        }

        this.clandestineExecute(() => this.scheduler.schedule());
    }

    private resetFPS(): void {
        this.timePerFrameAverage.reset();
        this.timestampLastFrame = performance.now();
    }

    private calculateFPS(): number {
        const timePerFrame = this.timePerFrameAverage.calculateAverage();

        return timePerFrame && 1000 / timePerFrame;
    }

    private onPwmUpdate = (pwmUpdate: PwmUpdate) => {
        if (this.pendingPwmUpdates.count() > 0 && this.pendingPwmUpdates.peekLast()!.frequency < 0) {
            this.pendingPwmUpdates.replaceLast(pwmUpdate);
        } else {
            this.pendingPwmUpdates.push(pwmUpdate);
        }
    };

    private onSerialPortDispatch = (): void => {
        this.processTimesliceNow();
    };

    private updateEmulationSpeed(currentSpeed: number) {
        if (currentSpeed >= 1 && this.emulationSpeed >= 1) return;

        // Speed < 1 ? -> bin it in 0.1 .. 0.9
        const normalizedSpeed = Math.min(Math.max(Math.floor(currentSpeed * 10) / 10, 0.1), 1);

        if (normalizedSpeed <= this.emulationSpeed) {
            // speed decline is accepted directly
            this.emulationSpeed = normalizedSpeed;
        } else {
            // speed increase is subject to 5% hysteresis
            this.emulationSpeed = currentSpeed - normalizedSpeed >= 0.05 ? normalizedSpeed : this.emulationSpeed;
        }
    }

    private checkAndUpdateHotsyncName(): void {
        if (!this.cloudpilotInstance) return;

        if (this.deviceHotsyncName === undefined) {
            this.deviceHotsyncName = this.cloudpilotInstance.getHotsyncName();
        }

        const configuredHotsyncName = this.settings.hotsyncName;

        if (configuredHotsyncName === undefined) {
            this.configuredHotsyncNameUpdateEvent.dispatch(this.deviceHotsyncName);
        } else if (
            configuredHotsyncName !== this.deviceHotsyncName &&
            configuredHotsyncName.length <= 40 &&
            this.cloudpilotInstance.isSetupComplete()
        ) {
            this.cloudpilotInstance.setHotsyncName(configuredHotsyncName);
            this.deviceHotsyncName = configuredHotsyncName;
        }
    }

    private advanceEmulation(timestamp: number): void {
        if (!this.isRunning) return;

        if (timestamp < this.clockEmulator) return;

        if (
            this.cloudpilotInstance.isSuspended() &&
            this.cloudpilotInstance.getSuspendKind() === SuspendKind.serialSync &&
            timestamp - this.serialSyncStartedAt > SERIAL_SYNC_TIMEOUT_MSEC
        ) {
            console.log('serial sync timeout, waking emulator');
            this.cloudpilotInstance.getSuspendContextSerialSync().Cancel();
        }

        const wasSuspended = this.cloudpilotInstance.isSuspended();
        let isSuspended = false;

        // Scale the clock by the calculated emulation speed
        this.cloudpilotInstance.setClockFactor(this.emulationSpeed * this.settings.speed);

        // Limit the time that we try to catch up. This will avoid that we lock onto a low
        // FPS if the emulation cannot run at full speed
        if (timestamp - this.clockEmulator > 1000 / MIN_FPS) this.clockEmulator = timestamp - 1000 / MIN_FPS;

        const cyclesToRun = ((timestamp - this.clockEmulator) / 1000) * this.cloudpilotInstance.cyclesPerSecond();

        const timestampBeforeCycle = performance.now();

        let cycles = 0;
        while (cycles < cyclesToRun) {
            cycles += this.cloudpilotInstance.runEmulation(Math.ceil(cyclesToRun - cycles));

            if (this.cloudpilotInstance.isSuspended()) {
                isSuspended = true;

                break;
            }
        }

        const virtualTimePassed = (cycles / this.cloudpilotInstance.cyclesPerSecond()) * 1000;
        const realTimePassed = performance.now() - timestampBeforeCycle;

        // If the emulation runs too slowly the amount of real time that passed will exceed the
        // emulated time difference. In this case we compensate by advancing the emulated clock
        // by the actual time difference; otherwise, the differences will pile up,
        // resulting in jerky emulation. Our dynamic speed correction will make sure that
        // this does not happen too often.
        const slizeSizeSeconds = Math.max(virtualTimePassed, realTimePassed);
        this.clockEmulator += slizeSizeSeconds;

        // Update the speed average. Note that we need to compensate this for the factor
        // by which we scaled the clock --- the factor represents the ratio for a device
        // running at ful speed
        this.speedAverage.push(
            (virtualTimePassed / (realTimePassed > 0 ? realTimePassed : virtualTimePassed / DUMMY_SPEED)) *
                this.emulationSpeed,
        );

        // Normalize the speed and apply hysteresis
        this.updateEmulationSpeed(this.speedAverage.calculateAverage());

        if (isSuspended && !wasSuspended) {
            switch (this.cloudpilotInstance.getSuspendKind()) {
                case SuspendKind.serialSync:
                    this.serialSyncStartedAt = performance.now();
                    break;

                default:
                    this.suspendEvent.dispatch();
                    break;
            }
        }

        const powerOff = this.cloudpilotInstance.isPowerOff();
        const uiInitialized = this.cloudpilotInstance.isUiInitialized();

        if (powerOff !== this.powerOff || uiInitialized !== this.uiInitialized) {
            this.powerOff = powerOff;
            this.uiInitialized = uiInitialized;

            this.palmosStateChangeEvent.dispatch();
        }

        if (uiInitialized && !powerOff && this.settings.manageHotsyncName && !isSuspended) {
            this.checkAndUpdateHotsyncName();
        }

        this.serialPortIR!.dispatch();
        this.serialPortSerial!.dispatch();

        if (
            this.settings.automaticSnapshotInterval > 0 &&
            timestamp - this.lastSnapshotAt > this.settings.automaticSnapshotInterval &&
            !this.cloudpilotInstance.isSuspended()
        ) {
            if (this.snapshotContainer.isIdle()) {
                this.snapshotEvent.dispatch(this.snapshotContainer.schedule(this.settings.memoryCrc));
            }
            this.lastSnapshotAt = timestamp;
        }

        this.timesliceEvent.dispatch(slizeSizeSeconds);
    }

    private onSchedule = (timestamp: number) => {
        if (this.cloudpilotInstance.isScreenDirty()) this.newFrameEvent.dispatch();

        if (
            this.pendingPwmUpdates.count() > 0 &&
            timestamp - this.lastPwmUpdateEvent > MIN_MILLISECONDS_PER_PWM_UPDATE
        ) {
            this.pwmUpdateEvent.dispatch(this.pendingPwmUpdates.pop()!);
            this.lastPwmUpdateEvent = timestamp;
        }

        if (this.advanceEmulationHandle === undefined) {
            this.advanceEmulationHandle = window.setTimeout(() => {
                this.advanceEmulationHandle = undefined;
                this.advanceEmulation(timestamp);
            }, 0);
        }

        this.timePerFrameAverage.push(Math.max(0, timestamp - this.timestampLastFrame));
        this.timestampLastFrame = timestamp;

        this.schedule();
    };

    readonly pwmUpdateEvent = new Event<PwmUpdate>();
    readonly configuredHotsyncNameUpdateEvent = new Event<string>();
    readonly newFrameEvent = new Event<void>();
    readonly timesliceEvent = new Event<number>();
    readonly suspendEvent = new Event<void>();
    readonly snapshotEvent = new Event<SnapshotContainer>();
    readonly palmosStateChangeEvent = new Event<void>();

    private running = false;
    private powerOff = false;
    private uiInitialized = false;
    private suspended = false;

    private emulationSpeed = 1;
    private speedAverage = new Average(SPEED_AVERAGE_N);

    private timePerFrameAverage = new Average(TIME_PER_FRAME_AVERAGE_N);
    private timestampLastFrame = 0;

    private frameCounter = 0;
    private clockEmulator = 0;

    private deviceId = DeviceId.m515;
    private deviceHotsyncName: string | undefined;

    private scheduler: Scheduler = new AnimationFrameScheduler((timestamp) => this.onSchedule(timestamp));
    private advanceEmulationHandle: number | undefined;

    private serialPortIR = new SerialPortImpl();
    private serialPortSerial = new SerialPortImpl();

    private pendingPwmUpdates = new Fifo<PwmUpdate>(PWM_FIFO_SIZE);
    private lastPwmUpdateEvent = 0;

    private imageData: ImageData | undefined;
    private canvasTmp: HTMLCanvasElement = document.createElement('canvas');

    private serialSyncStartedAt = 0;

    private snapshotContainer: SnapshotContainerImpl;
    private lastSnapshotAt = 0;
}
