import { Cloudpilot, PwmUpdate } from '@common/bridge/Cloudpilot';
import { Engine, StorageCardProvider } from '@common/engine/Engine';
import { EngineSettings } from '@common/engine/EngineSettings';
import { SnapshotContainer } from '@common/engine/Snapshot';
import { EngineCloudpilotImpl } from '@common/engine/cloudpilot/EngineCloudpilot';
import { EngineUarmImpl } from '@common/engine/uarm/EngineUarm';
import { deviceDimensions, engineType, isColor } from '@common/helper/deviceProperties';
import { GRAYSCALE_PALETTE_HEX } from '@common/helper/palette';
import { SchedulerKind } from '@common/helper/scheduler';
import { DeviceId } from '@common/model/DeviceId';
import { Dimensions } from '@common/model/Dimensions';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { EngineType } from '@common/model/EngineType';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { SerialPort } from '@common/serial/SerialPort';
import { PalmButton } from '@native/cloudpilot_web';
import { Event, EventInterface } from 'microevent.ts';

const PEN_MOVE_THROTTLE = 25;

const DEFAULT_ENGINE_SETTINGS: EngineSettings = {
    automaticSnapshotInterval: 0,
    memoryCrc: false,
    speed: 1,
    hotsyncName: '',
    manageHotsyncName: false,
    schedulerKind: SchedulerKind.animationFrame,
    networkIntegration: false,
    clipboardIntegration: false,
    disableAudio: false,
    maxHostLoad: 1,
    targetMips: 100,
    warnSlowdownThreshold: 0.5,
    runInBackground: false,
};

export type Executor = <T>(fn: () => T) => T;

export abstract class AbstractEmulationService {
    handlePointerMove(x: number, y: number, isSilkscreen: boolean): void {
        if (!this.engine?.isRunning()) return;

        const ts = performance.now();
        this.penDown = true;

        if (ts - this.lastPenUpdate < PEN_MOVE_THROTTLE) return;

        if (isSilkscreen) {
            const dimensions = deviceDimensions(this.deviceId());

            x = this.transformSilkscreenX(x, dimensions);
            y = this.transformSilkscreenY(y, dimensions);
        }

        this.engine.penDown(Math.floor(x), Math.floor(y));
        this.lastPenUpdate = ts;
    }

    handlePointerUp(): void {
        if (!this.engine?.isRunning()) return;

        if (this.engine && this.penDown) this.engine.penUp();

        this.lastPenUpdate = 0;
        this.penDown = false;
    }

    handleButtonDown(button: PalmButton): void {
        if (!this.engine?.isRunning()) return;

        this.engine?.buttonDown(button);
    }

    handleButtonUp(button: PalmButton): void {
        if (!this.engine?.isRunning()) return;

        this.engine?.buttonUp(button);
    }

    handleKeyDown(key: number) {
        if (!this.engine?.isRunning()) return;

        this.engine?.keyStroke(key);
    }

    reset(): void {
        this.engine?.reset();
    }

    resetNoExtensions(): void {
        this.engine?.resetNoExtensions();
    }

    resetHard(): void {
        this.engine?.resetHard();
    }

    isRunning(): boolean {
        return this.engine?.isRunning() ?? false;
    }

    isPowerOff(): boolean {
        return this.engine?.isPowerOff() ?? false;
    }

    isUiInitialized(): boolean {
        return this.engine?.isUIInitialized() ?? false;
    }

    getCanvas(): HTMLCanvasElement {
        return this.canvas;
    }

    getStatistics(): EmulationStatistics | undefined {
        return this.engine?.getStatistics();
    }

    isSlowdown(): boolean {
        return this.engine?.isSlowdown() ?? false;
    }

    isSuspended(): boolean {
        return this.engine?.type === 'cloudpilot' ? this.engine.isSuspended() : false;
    }

    isBlocked(): boolean {
        return this.blocked;
    }

    getSerialPortSerial(): SerialPort | undefined {
        return this.engine?.type === 'cloudpilot' ? this.engine.getSerialPortSerial() : undefined;
    }

    getSerialPortIR(): SerialPort | undefined {
        return this.engine?.type === 'cloudpilot' ? this.engine.getSerialPortIR() : undefined;
    }

    updateEngineSettings(settings: Partial<EngineSettings>): void {
        this.engineSettings = { ...this.engineSettings, ...settings };
        this.engine?.updateSettings(this.engineSettings);
    }

    deviceId(): DeviceId {
        return this.engine?.getDeviceId() ?? DeviceId.m515;
    }

    pwmUpdateEvent(): EventInterface<PwmUpdate> | undefined {
        return this.engine?.type === 'cloudpilot' ? this.engine.pwmUpdateEvent : undefined;
    }

    pcmPort(): MessagePort | undefined {
        return this.engine?.type === 'uarm' ? this.engine.getPcmPort() : undefined;
    }

    enablePcmStreaming(): void {
        if (this.engine?.type === 'uarm') this.engine.enablePcmStreaming();
    }

    disablePcmStreaming(): void {
        if (this.engine?.type === 'uarm') this.engine.disablePcmStreaming();
    }

    supportsKeyboardIO(): boolean {
        return this.engine?.supportsKeyboardIO() ?? false;
    }

    protected abstract updateConfiguredHotsyncName(hotsyncName: string): void;

    protected abstract onAfterAdvanceEmulation(sliceSizeSeconds: number): void;

    protected abstract handleSuspend(): void;

    protected abstract handleSnapshot(snapshot: SnapshotContainer): void;

    protected abstract getCloudpilotInstance(): Promise<Cloudpilot>;

    protected abstract getUarmModule(): Promise<WebAssembly.Module>;

    protected abstract handleFatalInNativeCode(error: Error): void;

    protected abstract handlePalmosStateChange(): void;

    protected processTimesliceNow(): void {
        this.engine?.processTimesliceNow();
    }

    protected async openSession(
        rom: Uint8Array,
        deviceId: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: StorageCardProvider,
    ): Promise<boolean> {
        this.unbindEngineHandlers(this.engine);
        this.engine?.shutdown();
        this.engine = undefined;
        this.shouldRun = false;

        const engine = await this.createEngine(engineType(deviceId));
        engine.updateSettings(this.engineSettings);

        if (!(await engine.openSession(rom, deviceId, nand, memory, state, card))) {
            return false;
        }

        this.bindEngineHandlers(engine);
        this.engine = engine;

        this.resetCanvas();

        this.openSessionEvent.dispatch();

        return true;
    }

    protected resetCanvas(): void {
        const dimensions = deviceDimensions(this.deviceId());

        this.canvas.width = dimensions.width;
        this.canvas.height = dimensions.height;

        this.clearCanvas();
    }

    protected async doResume(): Promise<void> {
        this.shouldRun = true;
        await this.enforceRunState();
    }

    protected async doStop(): Promise<void> {
        this.shouldRun = false;
        await this.enforceRunState();
    }

    protected async doBlock() {
        this.blocked = true;
        await this.enforceRunState();
    }

    protected async doUnblock() {
        this.blocked = false;
        await this.enforceRunState();
    }

    protected clearCanvas(): void {
        const deviceId = this.deviceId();

        const context = this.canvas.getContext('2d');
        if (!context) throw new Error('Unable to get rendering context. Get a new browser.');

        context.beginPath();
        context.rect(0, 0, this.canvas.width, this.canvas.height);
        context.fillStyle = isColor(deviceId) ? 'white' : GRAYSCALE_PALETTE_HEX[0];
        context.fill();
    }

    protected transformSilkscreenX(x: number, dimensions: Dimensions): number {
        if (dimensions.silkscreenOvershoot === undefined) {
            return x;
        }

        const factor = dimensions.width / (dimensions.width + 2 * dimensions.silkscreenOvershoot);
        return (x - dimensions.width / 2) / factor + dimensions.width / 2;
    }

    protected transformSilkscreenY(y: number, dimensions: Dimensions): number {
        if (dimensions.siklscreenShift === undefined) {
            return y;
        }

        return y + dimensions.siklscreenShift;
    }

    protected async requestSnapshot(): Promise<SnapshotContainer | undefined> {
        return await this.engine?.requestSnapshot();
    }

    protected getOSVersion(): number {
        return this.engine?.getOSVersion() ?? 0;
    }

    private async enforceRunState(): Promise<void> {
        if (!this.engine) return;

        if (this.shouldRun && !this.blocked) {
            await this.engine.resume();
        } else {
            await this.engine.stop();
        }

        this.emulationStateChangeEvent.dispatch(this.engine.isRunning());
    }

    private async createEngine(engineType: EngineType): Promise<Engine> {
        switch (engineType) {
            case 'cloudpilot':
                return new EngineCloudpilotImpl(
                    await this.getCloudpilotInstance(),
                    this.clandestineExecute,
                    this.engineSettings,
                );

            case 'uarm':
                return EngineUarmImpl.create(await this.getUarmModule(), this.engineSettings, this.clandestineExecute);

            default:
                throw new Error('unreachable');
        }
    }

    private onEngineNewFrameEvent = () => {
        if (!this.engine) throw new Error('invalid state');

        if (!this.engine.isLcdEnabled()) {
            this.clearCanvas();
        } else {
            this.engine.blitFrame(this.canvas);
        }

        this.newFrameEvent.dispatch(this.canvas);
    };

    private onEngineTimesliceEvent = (sliceSizeSeconds: number) => this.onAfterAdvanceEmulation(sliceSizeSeconds);

    private onEngineSuspendEvent = () => this.handleSuspend();

    private onEngineConfigureHotsyncNameEvent = (hotsyncName: string) => this.updateConfiguredHotsyncName(hotsyncName);

    private onSnapshotEvent = (snapshot: SnapshotContainer) => this.handleSnapshot(snapshot);

    private onSnapshotSuccessEvent = (statistics: SnapshotStatistics) => this.snapshotSuccessEvent.dispatch(statistics);

    private onPalmosStateChangeEvent = () => {
        this.handlePalmosStateChange();
        this.palmosStateChangeEvent.dispatch();
    };

    private onFatalError = (error: Error) => this.handleFatalInNativeCode(error);

    private bindEngineHandlers(engine?: Engine) {
        if (!engine) return;

        engine.newFrameEvent.addHandler(this.onEngineNewFrameEvent);
        engine.suspendEvent.addHandler(this.onEngineSuspendEvent);
        engine.timesliceEvent.addHandler(this.onEngineTimesliceEvent);
        engine.snapshotEvent.addHandler(this.onSnapshotEvent);
        engine.palmosStateChangeEvent.addHandler(this.onPalmosStateChangeEvent);
        engine.fatalError.addHandler(this.onFatalError);
        engine.snapshotSuccessEvent.addHandler(this.onSnapshotSuccessEvent);

        if (engine.type === 'cloudpilot') {
            engine.configuredHotsyncNameUpdateEvent.addHandler(this.onEngineConfigureHotsyncNameEvent);
        }
    }

    private unbindEngineHandlers(engine?: Engine) {
        if (!engine) return;

        engine.newFrameEvent.removeHandler(this.onEngineNewFrameEvent);
        engine.suspendEvent.removeHandler(this.onEngineSuspendEvent);
        engine.timesliceEvent.removeHandler(this.onEngineTimesliceEvent);
        engine.snapshotEvent.removeHandler(this.onSnapshotEvent);
        engine.palmosStateChangeEvent.removeHandler(this.onPalmosStateChangeEvent);
        engine.fatalError.removeHandler(this.onFatalError);
        engine.snapshotSuccessEvent.removeHandler(this.onSnapshotSuccessEvent);

        if (engine.type === 'cloudpilot') {
            engine.configuredHotsyncNameUpdateEvent.removeHandler(this.onEngineConfigureHotsyncNameEvent);
        }
    }

    newFrameEvent = new Event<HTMLCanvasElement>();
    newSessionEvent = new Event<void>();
    emulationStateChangeEvent = new Event<boolean>();
    palmosStateChangeEvent = new Event<void>();
    openSessionEvent = new Event<void>();
    snapshotSuccessEvent = new Event<SnapshotStatistics>();

    protected abstract readonly clandestineExecute: Executor;

    protected canvas: HTMLCanvasElement = document.createElement('canvas');

    protected lastPenUpdate = 0;
    protected penDown = false;

    protected engine?: Engine;
    protected engineSettings: EngineSettings = { ...DEFAULT_ENGINE_SETTINGS };

    private shouldRun = false;
    private blocked = false;
}
