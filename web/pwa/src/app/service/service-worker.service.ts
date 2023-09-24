import { Event } from 'microevent.ts';
import { environment } from '../../environments/environment';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';

const WORKER_URL = 'ngsw-worker.js';
const REGISTRATION_DELAY_MILLISECONDS = 3000;

@Injectable({ providedIn: 'root' })
export class ServiceWorkerService {
    constructor(private loadingController: LoadingController) {
        void this.register();
    }

    isEnabled(): boolean {
        return environment.production && !!navigator.serviceWorker;
    }

    isRegistered(): boolean {
        return !!navigator.serviceWorker?.controller;
    }

    async update(): Promise<void> {
        if (!this.registration) return;

        void this.registration.update();
    }

    async activateUpdate(): Promise<void> {
        if (!this.registration?.waiting) return;

        const loader = await this.loadingController.create({ message: 'Updating...' });
        await loader.present();

        this.registration.waiting.postMessage({ type: 'SKIP_WAITING' });
    }

    private async register(): Promise<void> {
        if (!(this.isEnabled() && navigator.serviceWorker)) return;

        navigator.serviceWorker.addEventListener(
            'controllerchange',
            () => this.reloadOnControllerChange && window.location.reload(),
        );

        await new Promise((r) => setTimeout(r, REGISTRATION_DELAY_MILLISECONDS));

        try {
            this.setupRegistration(await this.doRegister());
        } catch (e) {
            console.error(e);
        }
    }

    private async doRegister(): Promise<ServiceWorkerRegistration> {
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

    private dispatchUpdate(): void {
        if (!(navigator.serviceWorker.controller && this.registration?.waiting)) return;

        this.reloadOnControllerChange = true;

        this.updateAvailableEvent.dispatch(undefined);
    }

    private registration: ServiceWorkerRegistration | undefined;
    private reloadOnControllerChange = false;

    readonly updateAvailableEvent = new Event();
}
