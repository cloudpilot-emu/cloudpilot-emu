import { AlertService } from './alert.service';
import { EmulationService } from './emulation.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { ModalController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { VERSION } from '@pwa/helper/version';
import { ServiceWorkerService } from './service-worker.service';

const UPDATE_INTERVAL_MSEC = 15 * 60 * 1000;

@Injectable({
    providedIn: 'root',
})
export class UpdateService {
    constructor(
        private kvsService: KvsService,
        private emulationService: EmulationService,
        private alertService: AlertService,
        private modalController: ModalController,
        private serviceWorkerService: ServiceWorkerService,
    ) {
        void this.mutex.runExclusive(this.checkForDowngrade.bind(this));
    }

    public start(): void {
        void this.checkForUpgrade();
        this.startUpdateCheck();
        this.registerForUpdates();
    }

    private checkForUpgrade = () =>
        this.mutex.runExclusive(async () => {
            const storedVersion = this.kvsService.kvs.version;
            if (storedVersion === undefined) {
                this.kvsService.kvs.version = VERSION;

                return;
            }

            if (storedVersion === VERSION) return;

            await this.kvsService.set({
                version: VERSION,
                previousVersion: storedVersion,
                // This is a leftover from previous versions that is still left
                // in in order to provide a smooth transition.
                // TODO remove in a future version.
                latestVersion: VERSION,
            });

            // wait for a possible loader to disappear
            await this.emulationService.bootstrapComplete();

            await this.alertService.message('Update complete', `CloudpilotEmu was updated to version ${VERSION}.`, {
                Changes: () => this.showChangelog(),
            });
        });

    private startUpdateCheck(): void {
        this.scheduleUpdateCheck();
    }

    private registerForUpdates(): void {
        this.serviceWorkerService.updateAvailableEvent.addHandler(async () => {
            await this.emulationService.bootstrapComplete();

            void this.alertService.updateAvailable();
        });
    }

    private async checkForDowngrade(): Promise<void> {
        await this.kvsService.initialize();

        if (this.kvsService.kvs.previousVersion === VERSION) {
            const registration = await window.navigator.serviceWorker.getRegistration();

            console.log('downgrade detected');

            if (registration && (await registration.unregister())) {
                // Make sure that we cannot enter a reload loop
                await this.kvsService.set({ previousVersion: undefined });

                console.log('worker derigstered, reloading...');
                window.location.reload();
            }
        }
    }

    private async showChangelog(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/CHANGELOG.md',
                title: 'Changelog',
            },
        });

        await modal.present();
    }

    private scheduleUpdateCheck(): void {
        setTimeout(async () => {
            await this.serviceWorkerService.update();
            this.scheduleUpdateCheck();
        }, UPDATE_INTERVAL_MSEC);
    }

    private mutex = new Mutex();
}
