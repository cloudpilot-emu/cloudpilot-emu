import { AlertController } from '@ionic/angular';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class AlertService {
    constructor(private alertController: AlertController) {}

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
}
