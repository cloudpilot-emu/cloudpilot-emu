import { Injectable, OnDestroy } from '@angular/core';

import { EmulationService } from './emulation.service';

const TRIPPOINT = 3;
const INTERVAL = 1000;

@Injectable({ providedIn: 'root' })
export class PerformanceWatchdogService implements OnDestroy {
    constructor(private emulationService: EmulationService) {
        this.onEmulationStateChanged(this.emulationService.isRunning());

        this.emulationService.emulationStateChangeEvent.addHandler(this.onEmulationStateChanged);
    }

    isSlowdownDetected(): boolean {
        return this.slowdownDetected;
    }

    reset(): void {
        this.slowdownDetected = false;
        this.level = 0;
    }

    ngOnDestroy(): void {
        this.emulationService.emulationStateChangeEvent.removeHandler(this.onEmulationStateChanged);

        if (this.intervalHandle !== undefined) {
            clearInterval(this.intervalHandle);
            this.intervalHandle = undefined;
        }
    }

    private onInterval = () => {
        if (this.emulationService.getEmulationSpeed() < 1) {
            if (this.level < TRIPPOINT) this.level++;
            if (this.level === TRIPPOINT) this.slowdownDetected = true;
        } else {
            if (this.level > 0) this.level--;
            if (this.level === 0) this.slowdownDetected = false;
        }
    };

    private onEmulationStateChanged = (running: boolean) => {
        this.reset();

        if (running && this.intervalHandle === undefined) {
            this.onInterval();
            this.intervalHandle = window.setInterval(this.onInterval, INTERVAL);
        }

        if (!running && this.intervalHandle !== undefined) {
            clearInterval(this.intervalHandle);
            this.intervalHandle = undefined;
        }
    };

    private slowdownDetected = false;
    private level = 0;
    private intervalHandle: number | undefined;
}
