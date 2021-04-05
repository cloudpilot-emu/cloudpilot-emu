import { Component } from '@angular/core';
import { HelpComponent } from 'src/app/component/help/help.component';
import { ModalController } from '@ionic/angular';
import { VERSION } from '../../../app/helper/version';

@Component({
    selector: 'app-about',
    templateUrl: './about.page.html',
    styleUrls: ['./about.page.scss'],
})
export class AboutPage {
    constructor(private modalController: ModalController) {}

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

    loading = true;
}
