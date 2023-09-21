import { Injectable } from '@angular/core';
import { ModalController } from '@ionic/angular';
import { EmulationService } from './emulation.service';
import { KvsService } from './kvs.service';
import { isIOS } from '@common/helper/browser';
import { HelpComponent } from '@pwa/component/help/help.component';
import { AlertService } from './alert.service';

@Injectable({ providedIn: 'root' })
export class InfoService {
    constructor(
        private kvsService: KvsService,
        private emulationService: EmulationService,
        private modalController: ModalController,
        private alertService: AlertService,
    ) {}

    async start(): Promise<void> {
        const infoId = this.kvsService.kvs.infoId ?? 0;
        this.kvsService.kvs.infoId = await this.showInfo(infoId);
    }

    private async showInfo(infoId: number): Promise<number> {
        if (infoId < 2 && infoId !== 1 && isIOS) {
            await this.alertService.message(
                'Offline mode on iOS 17',
                `
                iOS 17 ships with a browser bug that breaks the caching of CloudpilotEmu
                for offline use. As a result, the app will currently not load and
                work reliably on iOS 17 without an internet connection.
                <br><br>
                This is a browser bug that can only be fixed by Apple.
            `,
            );
        }

        if (infoId === 1 && isIOS) {
            await this.alertService.message(
                'Browser bug on iOS 17',
                `
                CloudpilotEmu now includes a workaround for the browser bug that prevents
                reliable startup. However, running without an internet conection is
                still broken and can only be fixed if Apple resolves the underlying bug.
                <br><br>
                If you disabled Service Workers you can now safely reenable them.
            `,
            );
        }

        return 2;
    }
}
