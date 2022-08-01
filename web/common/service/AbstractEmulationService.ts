import { AnimationFrameScheduler, Scheduler, SchedulerKind } from '@common/helper/scheduler';
import { Cloudpilot, PalmButton, PwmUpdate } from '@common/Cloudpilot';
import { GRAYSCALE_PALETTE_HEX, GRAYSCALE_PALETTE_RGBA } from '@common/helper/palette';
import { deviceDimensions, isColor } from '@common/helper/deviceProperties';

import { Average } from '@common/helper/Average';
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { Event } from 'microevent.ts';
import { Fifo } from '@common/helper/Fifo';
import { TimeoutScheduler } from '@common/helper/scheduler';

const PEN_MOVE_THROTTLE = 25;
const PWM_FIFO_SIZE = 10;
const SPEED_AVERAGE_N = 20;
const TIME_PER_FRAME_AVERAGE_N = 60;
const MIN_FPS = 30;
const DUMMY_SPEED = 1000;
const MIN_MILLISECONDS_PER_PWD_UPDATE = 10;

export abstract class AbstractEmulationService {
    handlePointerMove(x: number, y: number, isSilkscreen: boolean): void {
        if (!this.handleInput()) return;

        const ts = performance.now();
        this.penDown = true;

        if (ts - this.lastPenUpdate < PEN_MOVE_THROTTLE || !this.cloudpilotInstance) return;

        if (isSilkscreen) {
            x = this.transformSilkscreenX(x);
        }

        this.cloudpilotInstance.queuePenMove(Math.floor(x), Math.floor(y));
        this.lastPenUpdate = ts;
    }

    handlePointerUp(): void {
        if (!this.handleInput()) return;

        if (this.cloudpilotInstance && this.penDown) this.cloudpilotInstance.queuePenUp();

        this.lastPenUpdate = 0;
        this.penDown = false;
    }

    handleButtonDown(button: PalmButton): void {
        if (!this.handleInput()) return;

        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonDown(button);
    }

    handleButtonUp(button: PalmButton): void {
        if (!this.handleInput()) return;

        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonUp(button);
    }

    handleKeyDown(key: number, ctrl = false) {
        if (!this.handleInput()) return;

        if (this.cloudpilotInstance) this.cloudpilotInstance.queueKeyboardEvent(key, ctrl);
    }

    reset(): void {
        if (this.cloudpilotInstance) this.cloudpilotInstance.reset();
    }

    resetNoExtensions(): void {
        if (this.cloudpilotInstance) this.cloudpilotInstance.resetNoExtensions();
    }

    resetHard(): void {
        if (this.cloudpilotInstance) this.cloudpilotInstance.resetHard();
    }

    isRunning(): boolean {
        return this.running;
    }

    isPowerOff(): boolean {
        return this.powerOff;
    }

    isUiInitialized(): boolean {
        return this.uiInitialized;
    }

    getCanvas(): HTMLCanvasElement {
        return this.canvas;
    }

    getStatistics(): EmulationStatistics {
        return {
            hostSpeed: this.speedAverage.calculateAverage(),
            emulationSpeed: this.emulationSpeed,
            averageFps: this.calculateFPS(),
        };
    }

    getEmulationSpeed(): number {
        return this.emulationSpeed;
    }

    isSuspended(): boolean {
        return this.cloudpilotInstance ? this.cloudpilotInstance.isSuspended() : false;
    }

    protected abstract getConfiguredSpeed(): number;

    protected abstract manageHotsyncName(): boolean;

    protected abstract getConfiguredHotsyncName(): string | undefined;

    protected abstract updateConfiguredHotsyncName(hotsyncName: string): void;

    protected abstract getConfiguredSchdedulerKind(): SchedulerKind;

    protected abstract hasFatalError(): boolean;

    protected abstract skipEmulationUpdate(): boolean;

    protected onAfterAdvanceEmulation(timestamp: number, cycles: number): void {}

    protected handleSuspend(): void {}

    protected callScheduler(): void {
        this.scheduler.schedule();
    }

    protected handleInput(): boolean {
        return true;
    }

    protected openSession(
        cloudpilot: Cloudpilot,
        rom: Uint8Array,
        device: DeviceId,
        memory?: Uint8Array,
        state?: Uint8Array
    ): boolean {
        let memoryLoaded = false;

        if (!cloudpilot.initializeSession(rom, device)) return false;

        if (memory) {
            const emulatedMemory = cloudpilot.getMemory32();

            if (emulatedMemory.length * 4 === memory.length) {
                emulatedMemory.set(new Uint32Array(memory.buffer, memory.byteOffset, emulatedMemory.length));
                memoryLoaded = true;
            } else {
                console.warn(
                    `memory size mismatch; ${emulatedMemory.length * 4} vs. ${memory.length} - ignoring image`
                );
            }
        }

        if (memoryLoaded && state) {
            cloudpilot.loadState(state);
        }

        if (this.cloudpilotInstance) this.cloudpilotInstance.pwmUpdateEvent.removeHandler(this.onPwmUpdate);
        cloudpilot.pwmUpdateEvent.addHandler(this.onPwmUpdate);

        this.cloudpilotInstance = cloudpilot;

        this.deviceId = device;
        this.resetCanvas();

        this.pendingPwmUpdates.flush();
        this.lastPwmUpdateEvent = 0;
        this.deviceHotsyncName = undefined;
        this.emulationSpeed = 1;
        this.frameCounter = 0;
        this.speedAverage.reset(1);
        this.powerOff = cloudpilot.isPowerOff();
        this.uiInitialized = cloudpilot.isUiInitialized();
        this.frameCounter = 0;

        return true;
    }

    protected resetCanvas(): void {
        const dimensions = deviceDimensions(this.deviceId);

        this.canvas.width = this.canvasTmp.width = dimensions.width;
        this.canvas.height = this.canvasTmp.height = dimensions.height;

        const context = this.canvas.getContext('2d');
        if (!context) {
            throw new Error('get a new browser');
        }

        this.context = context;

        const contextTmp = this.canvasTmp.getContext('2d');
        if (!contextTmp) {
            throw new Error('get a new browser');
        }

        this.contextTmp = contextTmp;

        this.clearCanvas();

        this.imageData = undefined;
    }

    protected doResume(): void {
        if (!this.cloudpilotInstance) return;
        this.clockEmulator = performance.now();

        this.schedule();

        this.setRunning(true);
        this.resetFPS();
    }

    protected doPause(): void {
        this.stopLoop();
    }

    protected doStop(): void {
        this.stopLoop();
    }

    protected stopLoop(): void {
        if (!this.running) return;

        this.scheduler.cancel();

        if (this.advanceEmulationHandle !== undefined) {
            window.clearTimeout(this.advanceEmulationHandle);

            this.advanceEmulationHandle = undefined;
        }

        this.setRunning(false);
    }

    protected onPwmUpdate = (pwmUpdate: PwmUpdate) => {
        if (this.pendingPwmUpdates.count() > 0 && this.pendingPwmUpdates.peekLast()!.frequency < 0) {
            this.pendingPwmUpdates.replaceLast(pwmUpdate);
        } else {
            this.pendingPwmUpdates.push(pwmUpdate);
        }
    };

    protected resetFPS(): void {
        this.timePerFrameAverage.reset();
        this.timestampLastFrame = performance.now();
    }

    protected calculateFPS(): number {
        const timePerFrame = this.timePerFrameAverage.calculateAverage();

        return timePerFrame && 1000 / timePerFrame;
    }

    protected transformSilkscreenX(x: number): number {
        const dimensions = deviceDimensions(this.deviceId);
        if (dimensions.silkscreenOvershoot === undefined) {
            return x;
        }

        const factor = dimensions.width / (dimensions.width + 2 * dimensions.silkscreenOvershoot);
        return (x - dimensions.width / 2) / factor + dimensions.width / 2;
    }

    protected setRunning(running: boolean): void {
        if (running === this.running) return;

        this.running = running;
        this.emulationStateChangeEvent.dispatch(running);
    }

    protected updateEmulationSpeed(currentSpeed: number) {
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

    protected performScreenUpdate(): void {
        if (this.cloudpilotInstance?.isScreenDirty()) {
            this.updateScreen();
            this.cloudpilotInstance.markScreenClean();

            this.frameCounter++;
        }
    }

    protected onSchedule(timestamp: number): void {
        if (this.hasFatalError()) return;

        if (!this.skipEmulationUpdate()) {
            this.performScreenUpdate();

            if (
                this.pendingPwmUpdates.count() > 0 &&
                timestamp - this.lastPwmUpdateEvent > MIN_MILLISECONDS_PER_PWD_UPDATE
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
        }

        this.schedule();
    }

    protected schedule() {
        this.scheduler.cancel();

        if (this.getConfiguredSchdedulerKind() !== this.scheduler.getKind()) {
            this.scheduler =
                this.getConfiguredSchdedulerKind() === SchedulerKind.animationFrame
                    ? new AnimationFrameScheduler((timestamp) => this.onSchedule(timestamp))
                    : new TimeoutScheduler((timestamp) => this.onSchedule(timestamp));
        }

        this.callScheduler();
    }

    protected advanceEmulation = (timestamp: number): void => {
        this.isDirty = false;

        if (!this.cloudpilotInstance || this.hasFatalError() || timestamp < this.clockEmulator) return;

        const wasSuspended = this.cloudpilotInstance.isSuspended();
        let isSuspended = false;

        // Scale the clock by the calculated emulation speed
        this.cloudpilotInstance.setClockFactor(this.emulationSpeed * this.getConfiguredSpeed());

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
        this.clockEmulator += Math.max(virtualTimePassed, realTimePassed);

        // Update the speed average. Note that we need to compensate this for the factor
        // by which we scaled the clock --- the factor represents the ratio for a device
        // running at ful speed
        this.speedAverage.push(
            (virtualTimePassed / (realTimePassed > 0 ? realTimePassed : virtualTimePassed / DUMMY_SPEED)) *
                this.emulationSpeed
        );

        // Normalize the speed an apply hysteresis
        this.updateEmulationSpeed(this.speedAverage.calculateAverage());

        if (isSuspended && !wasSuspended) {
            this.handleSuspend();
            return;
        }

        const powerOff = this.cloudpilotInstance.isPowerOff();
        const uiInitialized = this.cloudpilotInstance.isUiInitialized();

        if (powerOff !== this.powerOff || uiInitialized !== this.uiInitialized) {
            const oldPowerOff = this.powerOff;

            this.powerOff = powerOff;
            this.uiInitialized = uiInitialized;

            if (oldPowerOff !== this.powerOff) this.powerOffChangeEvent.dispatch(this.powerOff);

            this.isDirty = true;
        }

        if (uiInitialized && !powerOff && this.manageHotsyncName()) {
            this.checkAndUpdateHotsyncName();
        }

        this.onAfterAdvanceEmulation(timestamp, cycles);
    };

    protected checkAndUpdateHotsyncName(): void {
        if (!this.cloudpilotInstance) return;

        if (this.deviceHotsyncName === undefined) {
            this.deviceHotsyncName = this.cloudpilotInstance.getHotsyncName();
        }

        const configuredHotsyncName = this.getConfiguredHotsyncName();

        if (configuredHotsyncName === undefined) {
            this.updateConfiguredHotsyncName(this.deviceHotsyncName);
        } else if (
            configuredHotsyncName !== this.deviceHotsyncName &&
            configuredHotsyncName.length <= 40 &&
            this.cloudpilotInstance.isSetupComplete()
        ) {
            this.cloudpilotInstance.setHotsyncName(configuredHotsyncName);
            this.deviceHotsyncName = configuredHotsyncName;
        }
    }

    protected clearCanvas(): void {
        const deviceId = this.deviceId;
        const dimensions = deviceDimensions(deviceId);

        this.context.beginPath();
        this.context.rect(0, 0, dimensions.width, dimensions.height);
        this.context.fillStyle = isColor(deviceId) ? 'white' : GRAYSCALE_PALETTE_HEX[0];
        this.context.fill();
    }

    protected updateScreen(): void {
        if (!this.cloudpilotInstance) return;

        const frame = this.cloudpilotInstance.getFrame();

        if (this.cloudpilotInstance.isPowerOff()) {
            this.clearCanvas();

            this.newFrameEvent.dispatch(this.canvas);

            return;
        }

        if (!frame.hasChanges) {
            return;
        }

        if (frame.lines * frame.scaleY === this.canvas.height && frame.lineWidth * frame.scaleX === this.canvas.width) {
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
                            frame.buffer.byteLength >>> 2
                        );

                        if (frame.margin === 0) {
                            imageData32
                                .subarray(0, frame.lineWidth * (frame.lastDirtyLine - frame.firstDirtyLine + 1))
                                .set(
                                    buffer32.subarray(
                                        frame.firstDirtyLine * frame.lineWidth,
                                        frame.lineWidth * (frame.lastDirtyLine + 1)
                                    )
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

        (scaling ? this.contextTmp : this.context).putImageData(
            this.imageData,
            0,
            frame.firstDirtyLine,
            0,
            0,
            frame.lineWidth,
            frame.lastDirtyLine - frame.firstDirtyLine + 1
        );

        if (scaling) {
            this.context.imageSmoothingEnabled = false;
            this.context.drawImage(
                this.canvasTmp,
                0,
                frame.firstDirtyLine,
                frame.lineWidth,
                frame.lastDirtyLine - frame.firstDirtyLine + 1,
                0,
                frame.firstDirtyLine * frame.scaleY,
                frame.lineWidth * frame.scaleX,
                (frame.lastDirtyLine - frame.firstDirtyLine + 1) * frame.scaleY
            );
        }

        this.newFrameEvent.dispatch(this.canvas);
    }

    newFrameEvent = new Event<HTMLCanvasElement>();
    pwmUpdateEvent = new Event<PwmUpdate>();
    emulationStateChangeEvent = new Event<boolean>();
    powerOffChangeEvent = new Event<boolean>();

    protected cloudpilotInstance: Cloudpilot | undefined;
    protected deviceId = DeviceId.m515;

    protected clockEmulator = 0;
    protected scheduler: Scheduler = new AnimationFrameScheduler((timestamp) => this.onSchedule(timestamp));

    protected canvas: HTMLCanvasElement = document.createElement('canvas');
    protected canvasTmp: HTMLCanvasElement = document.createElement('canvas');
    protected context!: CanvasRenderingContext2D;
    protected contextTmp!: CanvasRenderingContext2D;
    protected imageData: ImageData | undefined;

    protected lastPenUpdate = 0;
    protected penDown = false;

    protected running = false;
    protected powerOff = false;
    protected uiInitialized = false;

    protected deviceHotsyncName: string | undefined;

    protected pendingPwmUpdates = new Fifo<PwmUpdate>(PWM_FIFO_SIZE);
    protected lastPwmUpdateEvent = 0;

    protected advanceEmulationHandle: number | undefined;

    protected emulationSpeed = 1;
    protected speedAverage = new Average(SPEED_AVERAGE_N);

    protected timePerFrameAverage = new Average(TIME_PER_FRAME_AVERAGE_N);
    protected timestampLastFrame = 0;

    protected frameCounter = 0;
    protected isDirty = false;
}
