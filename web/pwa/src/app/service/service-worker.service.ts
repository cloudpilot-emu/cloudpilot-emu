import { Event } from 'microevent.ts';
import { environment } from '../../environments/environment';
import { Injectable } from '@angular/core';

const WORKER_URL = 'ngsw-worker.js';
const REGISTRATION_DELAY_MILLISECONDS = 3000;

const enum State {
    failed,
    unregistered,
    registering,
    registered,
}

@Injectable({ providedIn: 'root' })
export class ServiceWorkerService {
    constructor() {
        void this.register();
    }

    isEnabled(): boolean {
        return environment.production && !!navigator.serviceWorker;
    }

    isRegistered(): boolean {
        return this.state > State.registering;
    }

    async update(): Promise<void> {
        if (this.state !== State.registered || !this.registration) return;

        void this.registration.update();
    }

    private async register(): Promise<void> {
        if (!(this.isEnabled() && navigator.serviceWorker)) return;
        if (this.state > State.unregistered) return;

        void navigator.serviceWorker.ready.then(() => {
            const now = Date.now();

            navigator.serviceWorker.addEventListener(
                'controllerchange',
                () => Date.now() - now > 500 && this.updateAvailableEvent.dispatch(undefined),
            );
        });

        this.state = State.registering;

        await new Promise((r) => setTimeout(r, REGISTRATION_DELAY_MILLISECONDS));

        try {
            this.registration = await this.doRegister();
            this.state = State.registered;
        } catch (e) {
            console.error(e);
            this.state = State.failed;
        }
    }

    private async doRegister(): Promise<ServiceWorkerRegistration> {
        if (!navigator.serviceWorker) throw new Error('service worker not supported');

        return navigator.serviceWorker.register(WORKER_URL, {
            scope: './',
            updateViaCache: 'none',
        });
    }

    private state: State = State.unregistered;
    private registration: ServiceWorkerRegistration | undefined;

    readonly updateAvailableEvent = new Event();
}
