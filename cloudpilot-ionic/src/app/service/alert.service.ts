import { AlertController } from '@ionic/angular';
import { EmulationService } from './emulation.service';
import { EmulationStateService } from './emulation-state.service';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class AlertService {
    constructor(private alertController: AlertController, private emulationState: EmulationStateService) {}

    async errorMessage(message: string) {
        const alert = await this.alertController.create({
            header: 'Error',
            message,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }

    async message(header: string, message: string) {
        const alert = await this.alertController.create({
            header,
            message,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }

    async lockLost() {
        const alert = await this.alertController.create({
            header: 'Error',
            message: `
                Cloudpilot was opened in another tab or windoow.
                <br/><br/>
                Please close or reload this window. Any changes that you make in this window will be lost.
            `,
            backdropDismiss: false,
            buttons: this.emulationState.getCurrentSession()
                ? [{ text: 'Save current session', handler: () => false }]
                : [],
        });

        await alert.present();
    }
}
