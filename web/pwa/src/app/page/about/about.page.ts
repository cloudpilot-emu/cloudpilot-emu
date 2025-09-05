import { Component } from '@angular/core';
import aboutUrl from '@assets/doc/about.md';
import { ModalController } from '@ionic/angular';
import changelogUrl from '@root/CHANGELOG.md';

import { HelpComponent } from '@pwa/component/help/help.component';
import { debounce } from '@pwa/helper/debounce';
import { VERSION } from '@pwa/helper/version';
import { ServiceWorkerService } from '@pwa/service/service-worker.service';

@Component({
    selector: 'app-about',
    templateUrl: './about.page.html',
    styleUrls: ['./about.page.scss'],
    standalone: false,
})
export class AboutPage {
    constructor(
        private modalController: ModalController,
        private serviceWorkerService: ServiceWorkerService,
    ) {}

    get version(): string {
        return VERSION;
    }

    get url(): string {
        return aboutUrl;
    }

    async showChangelog(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: changelogUrl,
                title: 'Changelog',
            },
        });

        await modal.present();
    }

    get serviceWorkerAvailable(): boolean {
        return this.serviceWorkerService.isRegistered();
    }

    @debounce()
    async resetWorker(): Promise<void> {
        await this.serviceWorkerService.reset();
    }

    loading = true;
}
