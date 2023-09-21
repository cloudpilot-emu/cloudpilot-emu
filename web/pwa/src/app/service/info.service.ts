import { Injectable } from '@angular/core';
import { ModalController } from '@ionic/angular';
import { EmulationService } from './emulation.service';
import { KvsService } from './kvs.service';
import { isIOS } from '@common/helper/browser';
import { HelpComponent } from '@pwa/component/help/help.component';

@Injectable({ providedIn: 'root' })
export class InfoService {
    constructor(
        private kvsService: KvsService,
        private emulationService: EmulationService,
        private modalController: ModalController,
    ) {}

    async start(): Promise<void> {
        const infoId = this.kvsService.kvs.infoId ?? 0;
        this.kvsService.kvs.infoId = await this.showInfo(infoId);
    }

    private async showInfo(infoId: number): Promise<number> {
        if (infoId < 1 && isIOS) {
            const modal = await this.modalController.create({
                component: HelpComponent,
                componentProps: {
                    url: 'assets/doc/bug-ios17.md',
                    title: 'Browser bug on iOS 17',
                },
            });

            await this.emulationService.bootstrapComplete();
            await modal.present();
            await modal.onDidDismiss();
        }

        return 1;
    }
}
