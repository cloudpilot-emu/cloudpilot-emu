import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { isIOS } from '@common/helper/browser';
import { AlertService } from './alert.service';
import { EmulationService } from './emulation.service';

@Injectable({ providedIn: 'root' })
export class InfoService {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private emulationService: EmulationService,
    ) {}

    async start(): Promise<void> {
        const infoId = this.kvsService.kvs.infoId ?? 0;
        this.kvsService.kvs.infoId = await this.showInfo(infoId);
    }

    private async showInfo(infoId: number): Promise<number> {
        if (infoId === 2 && isIOS) {
            await this.emulationService.bootstrapComplete();

            await this.alertService.message(
                'iOS 17 bug resolved',
                `
                The browser bug that impacted startup and offline use on iOS 17
                has been fully resolved by switching to another service worker
                implementation that does not trigger it.
                <br><br>
                CloudpilotEmu now is fully functional on iOS 17 again.
            `,
            );
        }

        return 3;
    }
}
