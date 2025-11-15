import { Injectable, NgZone, OnDestroy } from '@angular/core';

import { EmulationService } from './emulation.service';

const TRIPPOINT = 3;
const INTERVAL = 1000;

@Injectable({ providedIn: 'root' })
export class PerformanceWatchdogService implements OnDestroy {
    constructor(
        private emulationService: EmulationService,
        private ngZone: NgZone,
    ) {
        this.onEmulationStateChanged(this.emulationService.isRunning());

        this.emulationService.emulationStateChangeEvent.addHandler(this.onEmulationStateChanged);
        this.emulationService.openSessionEvent.addHandler(this.onOpenSession);
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
        if (this.emulationService.isSlowdown()) {
            if (this.level < TRIPPOINT) this.level++;
            if (this.level === TRIPPOINT) this.ngZone.run(() => (this.slowdownDetected = true));
        } else {
            if (this.level > 0) this.level--;
            if (this.level === 0) this.ngZone.run(() => (this.slowdownDetected = false));
        }
    };

    private onEmulationStateChanged = (running: boolean) => {
        if (running && this.intervalHandle === undefined) {
            this.onInterval();
            this.ngZone.runOutsideAngular(() => (this.intervalHandle = window.setInterval(this.onInterval, INTERVAL)));
        }

        if (!running && this.intervalHandle !== undefined) {
            clearInterval(this.intervalHandle);
            this.intervalHandle = undefined;
        }
    };

    private onOpenSession = (): void => {
        this.reset();
    };

    private slowdownDetected = false;
    private level = 0;
    private intervalHandle: number | undefined;
}
