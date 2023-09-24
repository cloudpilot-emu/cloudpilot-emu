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

        this.state = State.registering;

        await new Promise((r) => setTimeout(r, REGISTRATION_DELAY_MILLISECONDS));

        try {
            this.setupRegistration(await this.doRegister());
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

    private setupRegistration(registration: ServiceWorkerRegistration): void {
        this.registration = registration;

        if (registration.waiting) this.dispatchUpdate();
        if (registration.installing) this.handleInstallingWorker(registration.installing);

        registration.addEventListener(
            'updatefound',
            () => registration.installing && this.handleInstallingWorker(registration.installing),
        );
    }

    private handleInstallingWorker(worker: ServiceWorker) {
        let notified = false;

        worker.addEventListener('statechange', () => {
            if (worker.state !== 'installed' || notified) return;

            notified = true;
            this.dispatchUpdate();
        });
    }

    private dispatchUpdate() {
        if (!navigator.serviceWorker) return;
        if (!navigator.serviceWorker.controller) return;

        this.updateAvailableEvent.dispatch(undefined);
    }

    private state: State = State.unregistered;
    private registration: ServiceWorkerRegistration | undefined;

    readonly updateAvailableEvent = new Event();
}
