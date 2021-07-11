import { AlertController } from '@ionic/angular';
import { EmulationService } from './emulation.service';
import { EmulationStateService } from './emulation-state.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class AlertService {
    constructor(private alertController: AlertController, private emulationState: EmulationStateService) {}

    async errorMessage(message: string) {
        const alert = await this.alertController.create({
            header: 'Error',
            backdropDismiss: false,
            message,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }

    async message(
        header: string,
        message: string,
        extraButtons: Record<string, () => void> = {},
        closeButtonLabel = 'Close'
    ) {
        const alert = await this.alertController.create({
            header,
            message,
            backdropDismiss: false,
            buttons: [
                ...Object.keys(extraButtons).map((text) => ({
                    text,
                    handler: () => {
                        alert.dismiss();
                        extraButtons[text]();
                    },
                })),
                { text: closeButtonLabel, role: 'cancel' },
            ],
        });

        await alert.present();
    }

    async updateAvailable() {
        const alert = await this.alertController.create({
            header: 'Update available',
            message: 'An update is available. Please close cloudpilot and reload in order to update.',
            backdropDismiss: false,
            buttons: [
                { text: 'Reload', handler: () => window.location.reload() },
                { text: 'Close', role: 'cancel' },
            ],
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
                ...(haveCurrentSession
                    ? [
                          {
                              text: 'Save image',
                              handler: () => {
                                  this.emergencySaveEvent.dispatch();
                                  return false;
                              },
                          },
                      ]
                    : []),
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

    proxyVersionMismatchError() {
        return this.errorMessage(`
            Server version does not match Cloudpilot. Please make sure that you are using the latest
            versions of Cloudpilot and of the server.
        `);
    }

    emergencySaveEvent = new Event<void>();
}
