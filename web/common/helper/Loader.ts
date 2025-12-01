import { Mutex } from 'async-mutex';

export interface LoaderAdapter {
    show(message: string): Promise<void>;
    hide(): Promise<void>;
}

export type LoaderHandle = number;

interface StackEntry {
    message: string | undefined;
    handle: LoaderHandle;

    resolved: boolean;
}

export class Loader {
    constructor(
        private adapter: LoaderAdapter,
        private hideDelay: number,
        private now: () => number = () => performance.now(),
    ) {}

    async add(message?: string, delay = 0): Promise<LoaderHandle> {
        const handle = this.nextHandle++;
        this.stack.push({ message, handle, resolved: false });

        await this.scheduleShow(delay);

        return handle;
    }

    resolve(handle: LoaderHandle): void {
        let i: number;
        for (i = this.stack.length - 1; i >= 0; i--) {
            if (this.stack[i].handle === handle) break;
        }

        const entry = this.stack[i];

        if (entry === undefined || entry.resolved) throw new Error('double resolve loader');
        if (i !== this.stack.length - 1) console.error('BUG: loader resolved out of order');

        entry.resolved = true;

        for (i = this.stack.length - 1; i >= 0; i--) {
            if (!this.stack[i].resolved) break;

            this.stack.pop();
        }

        if (this.stack.length === 0) this.scheduleHide();
    }

    async showWhile<T>(fn: () => Promise<T>, message?: string, delay?: number): Promise<T> {
        const handle = await this.add(message, delay);

        try {
            return await fn();
        } finally {
            this.resolve(handle);
        }
    }

    private scheduleShow(delay: number): Promise<void> {
        const now = this.now();

        if (this.hideHandle !== undefined) {
            clearTimeout(this.hideHandle);
            this.hideHandle = undefined;
        }

        if (this.showHandle !== undefined && delay > 0 && now + delay >= this.showAt) return Promise.resolve();

        if (this.showHandle !== undefined) clearTimeout(this.showHandle);
        this.showHandle = undefined;
        this.showAt = now + delay;

        if (delay <= 0) return this.show();

        this.showHandle = setTimeout(this.show, delay);

        return Promise.resolve();
    }

    private scheduleHide(): void {
        if (this.showHandle) {
            clearTimeout(this.showHandle);
            this.showHandle = undefined;
        }

        if (this.hideHandle !== undefined) return;

        if (this.hideDelay <= 0) {
            this.hide();
        } else {
            this.hideHandle = setTimeout(this.hide, this.hideDelay);
        }
    }

    private getMessage(): string {
        for (const entry of this.stack) {
            if (entry.message !== undefined && !entry.resolved) return entry.message;
        }

        return '';
    }

    private show = async (): Promise<void> => {
        this.showHandle = undefined;
        if (this.stack.length <= 0) return;

        const message = this.getMessage();
        await this.mutex.runExclusive(() => this.adapter.show(message));
    };

    private hide = (): void => {
        this.hideHandle = undefined;
        if (this.stack.length > 0) return;

        void this.mutex.runExclusive(() => this.adapter.hide());
    };

    private stack: Array<StackEntry> = [];
    private nextHandle = 0;

    private showAt: number = 0;

    private showHandle: ReturnType<typeof setTimeout> | undefined;
    private hideHandle: ReturnType<typeof setTimeout> | undefined;

    private mutex = new Mutex();
}
