import { Cloudpilot } from '../helper/Cloudpilot';
import { Injectable } from '@angular/core';
import { Mutex } from 'async-mutex';
import { Session } from '../model/Session';
import { StorageService } from './storage.service';

@Injectable({
    providedIn: 'root',
})
export class EmulationService {
    constructor(private storageService: StorageService) {}

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

    private onAnimationFrame = (timestamp: number): void => {
        if (timestamp - this.clockEmulator > 500) this.clockEmulator = timestamp - 10;

        const cyclesToRun = ((timestamp - this.clockEmulator) / 1000) * this.cloudpilotInstance.cyclesPerSecond();

        let cycles = 0;
        while (cycles < cyclesToRun) {
            cycles += this.cloudpilotInstance.runEmulation(Math.ceil(cyclesToRun - cycles));
        }

        this.clockEmulator += (cycles / this.cloudpilotInstance.cyclesPerSecond()) * 1000;

        this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame);
    };

    readonly cloudpilot = Cloudpilot.create();
    private cloudpilotInstance!: Cloudpilot;

    private currentSession: Session | undefined;

    private clockEmulator = 0;
    private animationFrameHandle = -1;

    private mutex = new Mutex();
}
