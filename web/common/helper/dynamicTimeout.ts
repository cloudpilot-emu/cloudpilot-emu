const enum State {
    created = 'created',
    running = 'running',
    paused = 'paused',
    finished = 'finished',
}

export interface DocumentInterface {
    addEventListener(event: 'visibilitychange', handler: () => void): void;
    removeEventListener(event: 'visibilitychange', handler: () => void): void;

    visibilityState: DocumentVisibilityState;
}

export class DynamicTimeout {
    constructor(
        private timeoutRemaining: number,
        private cb: () => void,
        private document: DocumentInterface = window.document,
    ) {}

    start(): this {
        this.setState(State.running);

        this.schedule();
        this.document.addEventListener('visibilitychange', this.onVisibilityChange);

        return this;
    }

    increase(delta: number): this {
        this.timeoutRemaining += delta;

        return this;
    }

    cancel(): this {
        this.setState(State.finished);

        return this;
    }

    isTainted(): boolean {
        return this.tainted;
    }

    getAccumulatedTimeout(): number {
        return this.accumulatedTimeout;
    }

    private schedule(): void {
        if (this.handle !== undefined) return;

        this.timoutStartedAt = performance.now();
        this.handle = setTimeout(() => this.onTimeout(), this.timeoutRemaining);
    }

    private update(): void {
        if (this.state !== State.running) return;

        const delta = performance.now() - this.timoutStartedAt;
        this.timeoutRemaining -= delta;
        this.accumulatedTimeout += delta;

        if (this.timeoutRemaining <= 0) {
            this.setState(State.finished);
            this.cb();
        }
    }

    private onVisibilityChange = () => {
        this.tainted = true;

        switch (this.document.visibilityState) {
            case 'hidden':
                if (this.state !== State.running) return;

                this.cancelTimeout();
                this.update();

                if ((this.state as State) !== State.finished) this.state = State.paused;

                break;

            case 'visible':
                if (this.state !== State.paused || this.timeoutRemaining < 0) return;

                this.schedule();
                this.state = State.running;

                break;
        }
    };

    private onTimeout = () => {
        this.handle = undefined;
        this.update();

        if (this.timeoutRemaining > 0 && this.state === State.running) this.schedule();
    };

    private cancelTimeout(): void {
        if (this.handle === undefined) return;

        clearTimeout(this.handle);
        this.handle = undefined;
    }

    private setState(newState: State) {
        if (newState === this.state) return;

        if (this.state === State.finished || newState === State.created) {
            throw new Error(`invalid state transition: ${this.state} -> ${newState}`);
        }

        if (newState === State.finished) {
            this.cancelTimeout();
            this.document.removeEventListener('visibilitychange', this.onVisibilityChange);
        }

        this.state = newState;
    }

    private state = State.created;

    private timoutStartedAt = performance.now();
    private handle: ReturnType<typeof setTimeout> | undefined;

    private tainted = false;
    private accumulatedTimeout = 0;
}
