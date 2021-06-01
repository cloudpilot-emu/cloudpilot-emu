import { AfterViewInit, Component, OnInit } from '@angular/core';

import { AlertService } from 'src/app/service/alert.service';
import { EmulationService } from './service/emulation.service';
import { HelpComponent } from './component/help/help.component';
import { KvsService } from './service/kvs.service';
import { ModalController } from '@ionic/angular';
import { PwaService } from './service/pwa.service';
import { SwUpdate } from '@angular/service-worker';
import { VERSION } from './helper/version';

@Component({
    selector: 'app-root',
    templateUrl: 'app.component.html',
    styleUrls: ['app.component.scss'],
})
export class AppComponent implements AfterViewInit {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private emulationService: EmulationService,
        private pwaService: PwaService,
        private updates: SwUpdate,
        private modalController: ModalController
    ) {}

    ngAfterViewInit(): void {
        this.pwaService.invite();

        this.registerForUpdates();
        this.checkForUpdate();
    }

    private async checkForUpdate(): Promise<void> {
        const storedVersion = this.kvsService.kvs.version;
        const previousVersion = this.kvsService.kvs.previousVersion;
        const latestVersion = this.kvsService.kvs.latestVersion;

        if (storedVersion === undefined) {
            this.kvsService.kvs.version = VERSION;

            return;
        }

        if (storedVersion === VERSION) return;

        if (previousVersion === VERSION && latestVersion !== VERSION) {
            this.updateReverted = true;

            await this.emulationService.bootstrapComplete();

            this.alertService.message(
                'Version mismatch',
                `Cloudpilot has reverted to an old version. This indicates a browser bug.
                Please wait a few seconds for the browser to update Cloudpilot in the background,
                then reload to switch back to the current version.
                `,
                { Reload: () => window.location.reload() }
            );

            return;
        }

        this.kvsService.kvs.version = VERSION;
        this.kvsService.kvs.previousVersion = storedVersion;
        this.kvsService.kvs.latestVersion = VERSION;

        // wait for a possible loader to disappear
        await this.emulationService.bootstrapComplete();

        this.alertService.message('Update complete', `Cloudpilot was updated to version ${VERSION}.`, {
            Changes: () => this.showChangelog(),
        });
    }

    private registerForUpdates(): void {
        this.updates.available.subscribe(async () => {
            await this.emulationService.bootstrapComplete();

            if (this.updateReverted) {
                this.alertService.updateComplete();
            } else {
                this.alertService.updateAvailable();
            }
        });
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

    private updateReverted = false;
}
