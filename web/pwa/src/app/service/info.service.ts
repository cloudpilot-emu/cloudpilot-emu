import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { isIOS, isIOS_174 } from '@common/helper/browser';
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

        if (this.pwaService.getInstallationMode() === InstallationMode.pwa && isIOS && !isIOS_174) {
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
        if ((warningId ?? 0) < 2) {
            await this.emulationService.bootstrapComplete();

            await this.alertService.message(
                'Update to iOS 17.4',
                `
            Starting with iOS 17.4, homescreen apps will open in Safari on all iPhones in the
            European Union. CloudpilotEmu will continue to work as a web page, but all data stored by the
            homescreen app will be lost.
            <br><br>
            Please export all your sessions and card images before updating to iOS 17.4.
            <br><br>
            You can find more information under "Known issues" on the "About" tab.`,
            );
        }

        return 2;
    }
}
