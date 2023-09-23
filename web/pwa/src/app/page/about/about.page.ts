import { Component } from '@angular/core';
import { HelpComponent } from '@pwa/component/help/help.component';
import { ModalController } from '@ionic/angular';
import { VERSION } from '@pwa/helper/version';
import { ServiceWorkerService } from '@pwa/service/service-worker.service';

@Component({
    selector: 'app-about',
    templateUrl: './about.page.html',
    styleUrls: ['./about.page.scss'],
})
export class AboutPage {
    constructor(
        private modalController: ModalController,
        private serviceWorkerService: ServiceWorkerService,
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
        return this.serviceWorkerService.isRegistered();
    }

    loading = true;
}
