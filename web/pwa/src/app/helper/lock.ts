import { InjectionToken } from '@angular/core';
import { Event } from 'microevent.ts';

export const EMULATOR_LOCK_TOKEN = new InjectionToken<Lock>('emulator lock');

export class Lock {
    lock(): void {
        this.lockCount++;
        if (this.lockCount === 1) this.lockEvent.dispatch();
    }

    unlock(): void {
        if (this.lockCount === 0) return;
        if (--this.lockCount === 0) this.unlockEvent.dispatch();
    }

    isLocked(): boolean {
        return this.lockCount > 0;
    }

    async runGuarded<T>(fn: () => Promise<T>): Promise<T> {
        this.lock();

        try {
            return await fn();
        } finally {
            this.unlock();
        }
    }

    readonly lockEvent = new Event<void>();
    readonly unlockEvent = new Event<void>();

    private lockCount = 0;
}
