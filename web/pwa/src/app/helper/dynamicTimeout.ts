export class DynamicTimeout {
    constructor(private timeoutRemaining: number, private cb: () => void) {
        this.handle = window.setTimeout(this.onTimeout, timeoutRemaining);
    }

    increase(delta: number): void {
        this.timeoutRemaining += delta;
    }

    cancel(): void {
        clearTimeout(this.handle);
    }

    private onTimeout = () => {
        const timestamp = performance.now();
        this.timeoutRemaining -= timestamp - this.timeoutTimestamp;

        if (this.timeoutRemaining > 0) {
            this.timeoutTimestamp = performance.now();
            this.handle = window.setTimeout(this.onTimeout, this.timeoutTimestamp);

            return;
        }

        this.cb();
    };

    private timeoutTimestamp = performance.now();
    private handle: number;
}
