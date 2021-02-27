import { Cloudpilot, PalmButton } from '../helper/Cloudpilot';
import { Injectable, NgZone, OnDestroy } from '@angular/core';

import { AlertService } from 'src/app/service/alert.service';
import { DeviceId } from '../model/DeviceId';
import { EmulationStateService } from './emulation-state.service';
import { Event } from 'microevent.ts';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { PageLockService } from './page-lock.service';
import { Session } from '../model/Session';
import { StorageService } from './storage.service';

export const GRAYSCALE_PALETTE_RGBA = [
    0xffd2d2d2,
    0xffc4c4c4,
    0xffb6b6b6,
    0xffa8a8a8,
    0xff9a9a9a,
    0xff8c8c8c,
    0xff7e7e7e,
    0xff707070,
    0xff626262,
    0xff545454,
    0xff464646,
    0xff383838,
    0xff2a2a2a,
    0xff1c1c1c,
    0xff0e0e0e,
    0xff000000,
];

export const GRAYSCALE_PALETTE_HEX = GRAYSCALE_PALETTE_RGBA.map(
    (x) => '#' + (x & 0xffffff).toString(16).padStart(6, '0')
);

const PEN_MOVE_THROTTLE = 25;

@Injectable({
    providedIn: 'root',
})
export class EmulationService {
    constructor(
        private storageService: StorageService,
        private ngZone: NgZone,
        private loadingController: LoadingController,
        private emulationState: EmulationStateService,
        alertService: AlertService,
        pageLockService: PageLockService
    ) {
        this.canvas.width = 160;
        this.canvas.height = 160;

        this.imageData.data.fill(255);

        const context = this.canvas.getContext('2d');
        if (!context) {
            throw new Error('get a new browser');
        }

        this.context = context;

        storageService.sessionChangeEvent.addHandler(this.onSessionChange);
        pageLockService.lockLostEvent.addHandler(this.pause);
    }

    switchSession = (id: number): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (id === this.emulationState.getCurrentSession()?.id) {
                return;
            }

            const loader = await this.loadingController.create({ message: 'Loading...' });
            await loader.present();

            try {
                const session = await this.storageService.getSession(id);
                this.emulationState.setCurrentSession(session);

                if (!session) {
                    throw new Error(`invalid session ${id}`);
                }

                const [rom, memory, state] = await this.storageService.loadSession(session);
                if (!rom) {
                    throw new Error(`invalid ROM ${session.rom}`);
                }

                const cloudpilot = await this.cloudpilot;
                let memoryLoaded = false;

                cloudpilot.initializeSession(rom, session.device);

                if (memory) {
                    const emulatedMemory = cloudpilot.getMemory();

                    if (emulatedMemory.length === memory.length) {
                        emulatedMemory.set(memory);
                        memoryLoaded = true;
                    } else {
                        console.error(
                            `memory size mismatcH; ${emulatedMemory.length} vs. ${memory.length} - ignoring image`
                        );
                    }
                }

                if (memoryLoaded && state) {
                    cloudpilot.loadState(state);
                }
            } finally {
                await loader.dismiss();
            }
        });

    resume = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            console.log('resume');

            if (!this.emulationState.getCurrentSession() || this.running) return;
            this.cloudpilotInstance = await this.cloudpilot;

            this.clockEmulator = performance.now();

            this.ngZone.runOutsideAngular(
                () => (this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame))
            );

            this.running = true;
        });

    pause = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            console.log('pause');

            await this._pause();
        });

    handlePointerMove(x: number, y: number): void {
        const ts = performance.now();
        this.penDown = true;

        if (ts - this.lastPenUpdate < PEN_MOVE_THROTTLE || !this.cloudpilotInstance) return;

        this.cloudpilotInstance.queuePenMove(x, y);
        this.lastPenUpdate = ts;
    }

    handlePointerUp(): void {
        if (this.cloudpilotInstance && this.penDown) this.cloudpilotInstance.queuePenUp();

        this.lastPenUpdate = 0;
        this.penDown = false;
    }

    handleButtonDown(button: PalmButton): void {
        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonDown(button);
    }

    handleButtonUp(button: PalmButton): void {
        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonUp(button);
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

    installFile(data: Uint8Array): Promise<number> {
        return this.cloudpilot.then((c) => c.installFile(data));
    }

    private onSessionChange = (sessionId: number): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (sessionId !== this.emulationState.getCurrentSession()?.id) return;

            this.emulationState.setCurrentSession(await this.storageService.getSession(sessionId));

            if (!this.emulationState.getCurrentSession()) await this._pause();
        });

    private _pause(): void {
        if (!this.running) return;

        if (this.animationFrameHandle > 0) {
            cancelAnimationFrame(this.animationFrameHandle);
            this.animationFrameHandle = -1;
        }

        this.running = false;
    }

    private onAnimationFrame = (timestamp: number): void => {
        if (timestamp - this.clockEmulator > 500) this.clockEmulator = timestamp - 10;

        const cyclesToRun = ((timestamp - this.clockEmulator) / 1000) * this.cloudpilotInstance.cyclesPerSecond();

        let cycles = 0;
        while (cycles < cyclesToRun) {
            cycles += this.cloudpilotInstance.runEmulation(Math.ceil(cyclesToRun - cycles));
        }

        this.clockEmulator += (cycles / this.cloudpilotInstance.cyclesPerSecond()) * 1000;

        if (this.cloudpilotInstance.isScreenDirty()) {
            this.updateScreen();
            this.cloudpilotInstance.markScreenClean();
        }

        const poweroff = this.cloudpilotInstance.isPowerOff();
        const uiInitialized = this.cloudpilotInstance.isUiInitialized();

        if (poweroff !== this.powerOff || uiInitialized !== this.uiInitialized) {
            this.ngZone.run(() => {
                this.powerOff = poweroff;
                this.uiInitialized = uiInitialized;
            });
        }

        this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame);
    };

    private updateScreen(): void {
        const frame = this.cloudpilotInstance.getFrame();

        if (this.cloudpilotInstance.isPowerOff()) {
            this.context.beginPath();
            this.context.rect(0, 0, 160, 160);
            this.context.fillStyle =
                this.emulationState.getCurrentSession()?.device === DeviceId.m515 ? 'white' : GRAYSCALE_PALETTE_HEX[0];
            this.context.fill();

            this.newFrame.dispatch(this.canvas);

            return;
        }

        if (frame.lines === this.imageData.height && frame.lineWidth === this.imageData.width) {
            switch (frame.bpp) {
                case 1: {
                    const fg = GRAYSCALE_PALETTE_RGBA[15];
                    const bg = GRAYSCALE_PALETTE_RGBA[0];

                    for (let y = 0; y < 160; y++) {
                        for (let x = 0; x < 160; x++) {
                            this.imageData32[y * 160 + x + frame.margin] =
                                frame.buffer[y * frame.bytesPerLine + ((x + frame.margin) >>> 3)] &
                                (0x80 >>> ((x + frame.margin) & 0x07))
                                    ? fg
                                    : bg;
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

                    for (let y = 0; y < 160; y++) {
                        for (let x = 0; x < 160; x++) {
                            this.imageData32[y * 160 + x + frame.margin] =
                                palette[
                                    (frame.buffer[y * frame.bytesPerLine + ((x + frame.margin) >>> 2)] >>
                                        (6 - 2 * ((x + frame.margin) & 0x03))) &
                                        0x03
                                ];
                        }
                    }

                    break;
                }

                case 4: {
                    for (let y = 0; y < 160; y++) {
                        for (let x = 0; x < 160; x++) {
                            this.imageData32[y * 160 + x + frame.margin] =
                                GRAYSCALE_PALETTE_RGBA[
                                    (frame.buffer[y * frame.bytesPerLine + ((x + frame.margin) >>> 1)] >>>
                                        (4 - 4 * ((x + frame.margin) & 0x01))) &
                                        0x0f
                                ];
                        }
                    }

                    break;
                }

                case 24:
                    for (let y = 0; y < 160; y++) {
                        for (let x = 0; x < 160; x++) {
                            const imageDataBase = 4 * (y * 160 + x + frame.margin);
                            const frameBufferBase = 3 * (y * 160 + x + frame.margin);

                            this.imageData.data[imageDataBase] = frame.buffer[frameBufferBase];
                            this.imageData.data[imageDataBase + 1] = frame.buffer[frameBufferBase + 1];
                            this.imageData.data[imageDataBase + 2] = frame.buffer[frameBufferBase + 2];
                        }
                    }

                    break;
            }
        }

        this.context.putImageData(this.imageData, 0, 0);

        this.newFrame.dispatch(this.canvas);
    }

    readonly cloudpilot = Cloudpilot.create();
    newFrame = new Event<HTMLCanvasElement>();

    private cloudpilotInstance!: Cloudpilot;

    private clockEmulator = 0;
    private animationFrameHandle = -1;

    private mutex = new Mutex();

    private canvas: HTMLCanvasElement = document.createElement('canvas');
    private context!: CanvasRenderingContext2D;
    private imageData = new ImageData(160, 160);
    private imageData32 = new Uint32Array(this.imageData.data.buffer);

    private lastPenUpdate = 0;
    private penDown = false;

    private running = false;
    private powerOff = false;
    private uiInitialized = false;
}
