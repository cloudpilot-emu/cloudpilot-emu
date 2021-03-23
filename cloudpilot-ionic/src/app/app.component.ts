import { AfterViewInit, Component, OnInit } from '@angular/core';

import { AlertService } from 'src/app/service/alert.service';
import { EmulationService } from './service/emulation.service';
import { KvsService } from './service/kvs.service';
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
        private updates: SwUpdate
    ) {}

    ngAfterViewInit(): void {
        this.pwaService.invite();

        this.registerForUpdates();
        this.checkForUpdate();
    }

    private async checkForUpdate(): Promise<void> {
        const storedVersion = this.kvsService.kvs.version;

        if (storedVersion === undefined) {
            this.kvsService.kvs.version = VERSION;

            return;
        }

        if (storedVersion === VERSION) return;

        this.kvsService.kvs.version = VERSION;

        // wait for a possible loader to disappear
        await this.emulationService.bootstrapComplete();

        this.alertService.message('Update complete', `Cloudpilot was updated to version ${VERSION}.`);
    }

    private registerForUpdates(): void {
        this.updates.available.subscribe(async () => {
            await this.emulationService.bootstrapComplete();

            this.alertService.updateAvailable();
        });
    }
}
