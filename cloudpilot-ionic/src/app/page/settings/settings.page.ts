import { Component } from '@angular/core';
import { HelpComponent } from 'src/app/component/help/help.component';
import { ModalController } from '@ionic/angular';
import { REVISION } from './../../../revision.gen';

@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
})
export class SettingsPage {
    constructor(private modalController: ModalController) {}

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/settings.md',
            },
        });
        await modal.present();
    }

    get version(): string {
        return REVISION;
    }
}
