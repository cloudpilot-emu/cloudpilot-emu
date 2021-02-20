import { Cloudpilot, PalmButton } from '../helper/Cloudpilot';

import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { Mutex } from 'async-mutex';
import { Session } from '../model/Session';
import { StorageService } from './storage.service';
import { ThisReceiver } from '@angular/compiler';

const PEN_MOVE_THROTTLE = 25;

@Injectable({
    providedIn: 'root',
})
export class EmulationService {
    constructor(private storageService: StorageService) {
        this.canvas.width = 160;
        this.canvas.height = 160;

        this.imageData.data.fill(255);

        const context = this.canvas.getContext('2d');
        if (!context) {
            throw new Error('get a new browser');
        }

        this.context = context;
    }

    switchSession = (id: number): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (id === this.currentSession?.id) {
                return;
            }

            this.currentSession = await this.storageService.getSession(id);
            if (!this.currentSession) {
                throw new Error(`invalid session ${id}`);
            }

            const [rom] = await this.storageService.loadSession(this.currentSession);
            if (!rom) {
                throw new Error(`invalid ROM ${this.currentSession.rom}`);
            }

            (await this.cloudpilot).initializeSession(rom, this.currentSession.device);
        });

    getCurrentSession(): Session | undefined {
        return this.currentSession;
    }

    resume = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            console.log('resume');

            if (!this.currentSession) return;
            this.cloudpilotInstance = await this.cloudpilot;

            this.clockEmulator = performance.now();

            this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame);
        });

    pause = (): Promise<void> =>
        this.mutex.runExclusive(() => {
            console.log('pause');

            if (this.animationFrameHandle > 0) {
                cancelAnimationFrame(this.animationFrameHandle);
                this.animationFrameHandle = -1;
            }
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

    handleButtonDown(button: PalmButton) {
        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonDown(button);
    }

    handleButtonUp(button: PalmButton) {
        if (this.cloudpilotInstance) this.cloudpilotInstance.queueButtonUp(button);
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

        this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame);
    };

    private updateScreen(): void {
        const frame = this.cloudpilotInstance.getFrame();

        if (this.cloudpilotInstance.isPowerOff()) {
            this.context.beginPath();
            this.context.rect(0, 0, 160, 160);
            this.context.fillStyle = '#d2d2d2';
            this.context.fill();

            this.newFrame.dispatch(this.canvas);

            return;
        }

        if (frame.lines === this.imageData.height && frame.lineWidth === this.imageData.width) {
            switch (frame.bpp) {
                case 24:
                    for (let x = 0; x < 160; x++) {
                        for (let y = 0; y < 160; y++) {
                            const imageDataBase = 4 * (y * 160 + x);
                            const frameBufferBase = 3 * (y * 160 + x);

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

    private currentSession: Session | undefined;

    private clockEmulator = 0;
    private animationFrameHandle = -1;

    private mutex = new Mutex();

    private canvas: HTMLCanvasElement = document.createElement('canvas');
    private context!: CanvasRenderingContext2D;
    private imageData = new ImageData(160, 160);

    private lastPenUpdate = 0;
    private penDown = false;
}
