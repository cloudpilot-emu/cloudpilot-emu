export class DynamicTimeout {
    constructor(
        private timeoutRemaining: number,
        private cb: () => void,
    ) {
        this.handle = setTimeout(this.onTimeout, timeoutRemaining);
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
            this.handle = setTimeout(this.onTimeout, this.timeoutRemaining);

            return;
        }

        this.cb();
    };

    private timeoutTimestamp = performance.now();
    private handle: ReturnType<typeof setTimeout>;
}
