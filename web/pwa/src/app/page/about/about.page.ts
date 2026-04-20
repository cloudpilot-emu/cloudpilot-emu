import { Component } from '@angular/core';
import aboutUrl from '@assets/doc/about.md';
import { ModalController } from '@ionic/angular';
import changelogUrl from '@root/CHANGELOG.md';

import { HelpComponent } from '@pwa/component/help/help.component';
import { debounce } from '@pwa/helper/debounce';
import { VERSION } from '@pwa/helper/version';
import { InstallationMode } from '@pwa/model/InstallationMode';
import { PlatformService } from '@pwa/service/platform.service';
import { PwaService } from '@pwa/service/pwa.service';
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
        private platformService: PlatformService,
        pwaService: PwaService,
    ) {
        this.isApp = pwaService.determineInstallationMode() === InstallationMode.app;
    }

    get version(): string {
        return VERSION;
    }

    get url(): string {
        return aboutUrl;
    }

    get appType(): string | undefined {
        if (!this.isApp) return undefined;

        const version = this.platformService.getAppVersion();
        const platform = this.platformService.getAppPlatform();

        if (version === undefined && platform === undefined) return 'Native app';
        if (version === undefined) return `Native app for ${platform}`;
        if (platform === undefined) return `Native app version ${version}`;

        return `Native app for ${platform} version ${version}`;
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
    private readonly isApp: boolean;
}
