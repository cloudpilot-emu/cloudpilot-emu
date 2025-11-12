import { Uarm } from '@common/bridge/Uarm';
import { EngineSettings } from '@common/engine/EngineSettings';
import { DeviceId } from '@common/model/DeviceId';
import { PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';
import 'setimmediate';

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
            this.uarm.setSd(cardData, key);
        }

        if (!this.uarm.launch(rom)) return false;

        this.deviceId = this.uarm.getDevice();

        return true;
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
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

        //if (now - this.lastSnapshot > 1000000) {
        //    this.triggerSnapshot();
        //    this.updateSpeedDisplay();
        //
        //    this.lastSnapshot = now;
        //}

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

    private timeoutHandle: number | undefined;
    private immediateHandle: number | undefined;

    private framePool: Array<ArrayBuffer> = [];
    private deviceId = DeviceId.te2;
}
