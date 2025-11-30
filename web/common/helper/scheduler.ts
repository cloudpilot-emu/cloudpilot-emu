import { clearTimeout, setTimeout } from 'worker-timers';

export const enum SchedulerKind {
    animationFrame,
    timeout,
}

export interface Scheduler {
    schedule(): void;
    cancel(): void;

    isScheduled(): boolean;
    getKind(): SchedulerKind;
}

export class AnimationFrameScheduler implements Scheduler {
    constructor(private callback: (timestamp: number) => void) {}

    schedule(): void {
        if (this.handle !== undefined) return;

        this.handle = requestAnimationFrame((timestamp) => {
            this.handle = undefined;
            this.callback(timestamp);
        });
    }

    cancel(): void {
        if (this.handle === undefined) return;

        cancelAnimationFrame(this.handle);
        this.handle = undefined;
    }

    isScheduled(): boolean {
        return this.handle !== undefined;
    }

    getKind(): SchedulerKind {
        return SchedulerKind.animationFrame;
    }

    private handle: number | undefined;
}

export class TimeoutScheduler implements Scheduler {
    constructor(
        private callback: (timestamp: number) => void,
        private fps = 60,
    ) {}

    schedule(): void {
        if (this.handle !== undefined) return;

        const now = performance.now();
        const nextTick = (Math.ceil((now / 1000) * this.fps) * 1000) / this.fps;

        this.handle = setTimeout(
            () => {
                this.handle = undefined;
                this.callback(performance.now());
            },
            Math.max(nextTick - now, 0),
        );
    }

    cancel(): void {
        if (this.handle === undefined) return;

        clearTimeout(this.handle);
        this.handle = undefined;
    }

    isScheduled(): boolean {
        return this.handle !== undefined;
    }

    getKind(): SchedulerKind {
        return SchedulerKind.timeout;
    }

    private handle: number | undefined;
}
