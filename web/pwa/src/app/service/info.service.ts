import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { isIOS, isIOSNative } from '@common/helper/browser';
import { AlertService } from './alert.service';
import { EmulationService } from './emulation.service';
import { PwaService } from './pwa.service';
import { InstallationMode } from '@pwa/model/InstallationMode';

@Injectable({ providedIn: 'root' })
export class InfoService {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private emulationService: EmulationService,
        private pwaService: PwaService,
    ) {}

    async start(): Promise<void> {
        const infoId = this.kvsService.kvs.infoId ?? 0;
        this.kvsService.kvs.infoId = await this.showInfo(infoId);

        if (this.pwaService.getInstallationMode() === InstallationMode.pwa && isIOS && !isIOSNative) {
            this.kvsService.kvs.ios174UpdateWarningId = await this.showIOS174UpdateWarning(
                this.kvsService.kvs.ios174UpdateWarningId,
            );
        }
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

    private async showIOS174UpdateWarning(warningId: number | undefined): Promise<number> {
        if ((warningId ?? 0) > 0) {
            await this.emulationService.bootstrapComplete();

            await this.alertService.message(
                'Update to iOS 17.4',
                `
                    Apple has backpedaled and will not disable homescreen apps in the European
                    Union. The update to 17.4 is expected to be safe, and CloudpilotEmu will
                    continue to work as before.
                `,
            );
        }

        return 0;
    }
}
