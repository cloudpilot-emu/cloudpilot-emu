import { Mutex } from 'async-mutex';

export interface LoaderAdapter<T> {
    show(message: string | undefined, loaderOptions?: T): Promise<void>;
    hide(): Promise<void>;
}

export type LoaderHandle = number;

interface StackEntry<T> {
    message: string | undefined;
    handle: LoaderHandle;
    loaderOptions?: T;

    resolved: boolean;
}

export class Loader<T> {
    constructor(
        private adapter: LoaderAdapter<T>,
        private hideDelay: number,
        private now: () => number = () => performance.now(),
    ) {}

    async add(message?: string, delay = 0, loaderOptions?: T): Promise<LoaderHandle> {
        if (delay < 0 || this.loaderVisible) delay = 0;

        const handle = this.nextHandle++;
        this.stack.push({ message, handle, loaderOptions, resolved: false });

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

        if (this.stack.length === 0) {
            this.scheduleHide();
        } else if (this.showHandle === undefined) {
            void this.show();
        }
    }

    updateMessage(handle: LoaderHandle, message: string | undefined): void {
        const entry = this.stack.find((e) => e.handle === handle);

        if (!entry) throw new Error(`invalid handle ${handle}`);
        if (entry.resolved) throw new Error(`handle ${handle} already resolved`);

        entry.message = message;

        if (this.showHandle !== undefined) return;
        void this.show();
    }

    async showWhile<U>(
        fn: (handle: LoaderHandle) => Promise<U>,
        message?: string,
        delay?: number,
        loaderOptions?: T,
    ): Promise<U> {
        const handle = await this.add(message, delay, loaderOptions);

        try {
            return await fn(handle);
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

    private getMessage(): string | undefined {
        for (const entry of this.stack) {
            if (entry.message !== undefined && !entry.resolved) return entry.message.replace(/\.\.\./g, '…');
        }

        return undefined;
    }

    private getLoaderOptions(): T | undefined {
        return this.stack.length > 0 ? this.stack[this.stack.length - 1].loaderOptions : undefined;
    }

    private show = async (): Promise<void> => {
        this.showHandle = undefined;
        if (this.stack.length <= 0) return;

        this.loaderVisible = true;

        const message = this.getMessage();
        await this.mutex.runExclusive(() => this.adapter.show(message, this.getLoaderOptions()));
    };

    private hide = (): void => {
        this.hideHandle = undefined;
        if (this.stack.length > 0) return;

        this.loaderVisible = false;

        void this.mutex.runExclusive(() => this.adapter.hide());
    };

    private stack: Array<StackEntry<T>> = [];
    private nextHandle = 0;

    private showAt: number = 0;
    private loaderVisible = false;

    private showHandle: ReturnType<typeof setTimeout> | undefined;
    private hideHandle: ReturnType<typeof setTimeout> | undefined;

    private mutex = new Mutex();
}
