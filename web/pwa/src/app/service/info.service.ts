import { Injectable } from '@angular/core';
import { isIOS, isIOSNative } from '@common/helper/browser';

import { InstallationMode } from '@pwa/model/InstallationMode';

import { AlertService } from './alert.service';
import { EmulationService } from './emulation.service';
import { KvsService } from './kvs.service';
import { NativeAppService } from './native-app.service';
import { PwaService } from './pwa.service';
import { ServiceWorkerService } from './service-worker.service';

@Injectable({ providedIn: 'root' })
export class InfoService {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private emulationService: EmulationService,
        private pwaService: PwaService,
        private nativeAppService: NativeAppService,
        private serviceWorkerService: ServiceWorkerService,
    ) {}

    async start(): Promise<void> {
        await this.handleiOSNativeAppServiceWorkerBug();

        const infoId = this.kvsService.kvs.infoId ?? 0;
        this.kvsService.kvs.infoId = await this.showInfo(infoId);

        if (this.pwaService.getInstallationMode() === InstallationMode.pwa && isIOS && !isIOSNative) {
            this.kvsService.kvs.ios174UpdateWarningId = await this.showIOS174UpdateWarning(
                this.kvsService.kvs.ios174UpdateWarningId,
            );
        }
    }

    private async handleiOSNativeAppServiceWorkerBug(): Promise<void> {
        if (!this.serviceWorkerService.isEnabled()) return;

        const workerFailed = await this.nativeAppService.getWorkerFailed();
        if (!workerFailed) return;

        let resetWorker = false;

        await this.alertService.message(
            'App failed to load offline',
            `
            It seems the app failed to load without internet. This is caused by an iOS bug.
            <br><br>
            You can fix by resetting the service worker now or by tapping
            "reset worker" at the bottom of the "About" tab later.
            `,
            { 'Fix now': () => (resetWorker = true) },
            'Close',
        );

        if (resetWorker) {
            await this.serviceWorkerService.reset();
        } else {
            this.nativeAppService.clearWorkerFailed();
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
