import { Component } from '@angular/core';
import { HelpComponent } from '@pwa/component/help/help.component';
import { ModalController } from '@ionic/angular';
import { SwUpdate } from '@angular/service-worker';
import { VERSION } from '@pwa/helper/version';

@Component({
    selector: 'app-about',
    templateUrl: './about.page.html',
    styleUrls: ['./about.page.scss'],
})
export class AboutPage {
    constructor(
        private modalController: ModalController,
        private updates: SwUpdate,
    ) {}

    get version(): string {
        return VERSION;
    }

    async showChangelog(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/CHANGELOG.md',
                title: 'Changelog',
            },
        });

        await modal.present();
    }

    get serviceWorkerAvailable(): boolean {
        return this.updates.isEnabled;
    }

    loading = true;
}
