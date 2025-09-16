import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { Event } from 'microevent.ts';

import { environment } from '../../environments/environment';
import { AlertService } from './alert.service';
import { NativeAppService } from './native-app.service';

const WORKER_URL = 'ngsw-worker.js';
const REGISTRATION_DELAY_MILLISECONDS = 3000;

@Injectable({ providedIn: 'root' })
export class ServiceWorkerService {
    constructor(
        private loadingController: LoadingController,
        private alertService: AlertService,
        private nativeAppService: NativeAppService,
    ) {
        void this.register();

        if (this.isEnabled()) void this.updateNativeAppRegistrationStatus(this.isRegistered());
    }

    isEnabled(): boolean {
        return environment.production && !!navigator.serviceWorker;
    }

    isRegistered(): boolean {
        return !!navigator.serviceWorker?.controller;
    }

    async reset(): Promise<void> {
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const registration = await navigator.serviceWorker.getRegistration();

            if (!(await registration?.unregister())) throw new Error('failed to unregister worker');

            await this.updateNativeAppRegistrationStatus(false);
        } catch (e) {
            console.error(e);
            await this.alertService.errorMessage('Failed to unregister service worker.');
            return;
        } finally {
            await loader.dismiss();
        }

        await this.alertService.message(
            'Service worker unregistered',
            `
            The current service worker has been unregistered. Please reload the app
            in order to register a new worker for offline usage.
        `,
            {},
            'Reload',
        );

        window.location.reload();
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

    private updateNativeAppRegistrationStatus = (workerRegistered: boolean) =>
        this.updateNativeAppRegistrationStatusMutex.runExclusive(() =>
            this.nativeAppService.setWorkerRegistered(workerRegistered),
        );

    private register = () =>
        this.registerMutex.runExclusive(async () => {
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
        });

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
    private registerMutex = new Mutex();
    private updateNativeAppRegistrationStatusMutex = new Mutex();

    readonly updateAvailableEvent = new Event();
}
