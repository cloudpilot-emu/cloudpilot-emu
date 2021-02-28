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

    async fatalError(reason: string) {
        const haveCurrentSession = !!this.emulationState.getCurrentSession();

        const alert = await this.alertController.create({
            header: 'Error',
            message: `
                ${reason}
                <br/><br/>
                Please close or reload this window.
                ${haveCurrentSession ? ' You may save an image of your current session before reloading.' : ''}
            `,
            backdropDismiss: false,
            buttons: [
                ...(haveCurrentSession ? [{ text: 'Save image', handler: () => false }] : []),
                { text: 'Reload', handler: () => window.location.reload() },
            ],
        });

        await alert.present();
    }

    async errorInNativeCode(reason: string) {
        const haveCurrentSession = !!this.emulationState.getCurrentSession();

        const alert = await this.alertController.create({
            header: 'Error',
            message: `
                You encountered a bug in cloudpilot:
                <br/><br/>
                ${reason}.
                <br/><br/>
                Please close or reload this window.
            `,
            backdropDismiss: false,
            buttons: [{ text: 'Reload', handler: () => window.location.reload() }],
        });

        await alert.present();
    }
}
