export class Throttle {
    constructor(
        private fn: () => void,
        private timeout: number,
        private timer: () => number = () => performance.now(),
    ) {}

    trigger(): void {
        if (this.timeoutHandle !== undefined) return;

        const delta = this.timer() - this.lastExecution;
        if (delta > this.timeout) {
            this.execute();
            return;
        }

        this.timeoutHandle = setTimeout(() => {
            this.timeoutHandle = undefined;
            this.execute();
        }, this.timeout - delta);
    }

    private execute(): void {
        try {
            this.fn();
        } finally {
            this.lastExecution = this.timer();
        }
    }

    private lastExecution = 0;
    private timeoutHandle: ReturnType<typeof setTimeout> | undefined;
}
