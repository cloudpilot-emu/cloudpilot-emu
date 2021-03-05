import { Component, OnInit } from '@angular/core';
import { StorageService, keyKvs } from './service/storage.service';

import { AlertService } from 'src/app/service/alert.service';
import { EmulationService } from './service/emulation.service';
import { REVISION } from './../revision';
import { SwUpdate } from '@angular/service-worker';

@Component({
    selector: 'app-root',
    templateUrl: 'app.component.html',
    styleUrls: ['app.component.scss'],
})
export class AppComponent implements OnInit {
    constructor(
        private storageService: StorageService,
        private alertService: AlertService,
        private emulationService: EmulationService,
        private updates: SwUpdate
    ) {}

    ngOnInit(): void {
        this.registerForUpdates();
        this.checkForUpdate();
    }

    private async checkForUpdate(): Promise<void> {
        const storedVersion = await this.storageService.kvsGet(keyKvs.version);

        if (storedVersion === undefined) {
            await this.storageService.kvsSet(keyKvs.version, REVISION);

            return;
        }

        if (storedVersion === REVISION) return;

        await this.storageService.kvsSet(keyKvs.version, REVISION);

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
