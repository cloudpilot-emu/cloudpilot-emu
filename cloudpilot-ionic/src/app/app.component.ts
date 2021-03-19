import { AfterViewInit, Component, OnInit } from '@angular/core';

import { AlertService } from 'src/app/service/alert.service';
import { EmulationService } from './service/emulation.service';
import { KvsService } from './service/kvs.service';
import { PwaService } from './service/pwa.service';
import { REVISION } from './../revision';
import { SwUpdate } from '@angular/service-worker';

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
            this.kvsService.kvs.version = REVISION;

            return;
        }

        if (storedVersion === REVISION) return;

        this.kvsService.kvs.version = REVISION;

        // wait for a possible loader to disappear
        await this.emulationService.bootstrapComplete();

        this.alertService.message('Update complete', `Cloudpilot was updated to revision ${REVISION}.`);
    }

    private registerForUpdates(): void {
        this.updates.available.subscribe(async () => {
            await this.emulationService.bootstrapComplete();

            this.alertService.updateAvailable();
        });
    }
}
