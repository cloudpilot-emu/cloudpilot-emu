export class Timed {
    reset() {
        this.timer = 0;
    }

    exec<T>(fn: () => T): T {
        const start = performance.now();

        try {
            return fn();
        } finally {
            this.timer += performance.now() - start;
        }
    }

    get(): number {
        return this.timer;
    }

    private timer = 0;
}
