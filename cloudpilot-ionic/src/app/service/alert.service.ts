import { AlertController } from '@ionic/angular';
import { EmulationService } from './emulation.service';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class AlertService {
    constructor(private alertController: AlertController) {}

    setEmulationService(emulationService: EmulationService): void {
        this.emulationService = emulationService;
    }

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
            buttons: this.emulationService?.getCurrentSession()
                ? [{ text: 'Save current session', handler: () => false }]
                : [],
        });

        await alert.present();
    }

    private emulationService: EmulationService | undefined;
}
