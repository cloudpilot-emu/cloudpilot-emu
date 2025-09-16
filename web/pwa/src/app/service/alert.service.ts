import { Injectable } from '@angular/core';
import { AlertController, IonicSafeString } from '@ionic/angular';
import { Event } from 'microevent.ts';

import { debounce } from '@pwa/helper/debounce';

import { EmulationStateService } from './emulation-state.service';

class Verbatim {
    constructor(public markup: string) {}
}

@Injectable({
    providedIn: 'root',
})
export class AlertService {
    constructor(
        private alertController: AlertController,
        private emulationState: EmulationStateService,
    ) {}

    verbatim(markup: string): Verbatim {
        return new Verbatim(markup);
    }

    async errorMessage(message: string | Verbatim, buttonLabel = 'Close') {
        const alert = await this.alertController.create({
            header: 'Error',
            backdropDismiss: false,
            message: this.sanitizeMessage(message),
            buttons: [{ text: buttonLabel, role: 'cancel' }],
            cssClass: 'alert-error',
        });

        await alert.present();
        await alert.onDidDismiss();
    }

    async message(
        header: string,
        message: string | Verbatim,
        extraButtons: Record<string, () => void> = {},
        closeButtonLabel = 'Close',
    ): Promise<void> {
        let onDismiss: () => void = () => undefined;
        const dismissed = new Promise<void>((resolve) => {
            onDismiss = resolve;
        });

        const alert = await this.alertController.create({
            header,
            message: this.sanitizeMessage(message),
            backdropDismiss: false,
            buttons: [
                { text: closeButtonLabel, role: 'cancel', handler: onDismiss },
                ...Object.keys(extraButtons).map((text) => ({
                    text,
                    handler: () => {
                        extraButtons[text]();
                        onDismiss();
                    },
                })),
            ],
            cssClass: 'cpe-alert-message',
        });

        await alert.present();
        await alert.onDidDismiss();
        await dismissed;
    }

    async messageWithChoice(
        header: string,
        message: string | Verbatim,
        choiceLabel: string,
        choice: boolean,
        extraButtons: Record<string, () => void> = {},
        closeButtonLabel = 'Close',
    ): Promise<boolean> {
        let onDismiss: () => void = () => undefined;
        const dismissed = new Promise<void>((resolve) => {
            onDismiss = resolve;
        });

        const alert = await this.alertController.create({
            header,
            message: this.sanitizeMessage(message),
            backdropDismiss: false,
            buttons: [
                { text: closeButtonLabel, role: 'cancel', handler: onDismiss },
                ...Object.keys(extraButtons).map((text) => ({
                    text,
                    handler: () => {
                        extraButtons[text]();
                        onDismiss();
                    },
                })),
            ],
            inputs: [
                {
                    type: 'checkbox',
                    label: choiceLabel,
                    checked: false,
                    handler: (inpt) => (choice = inpt.checked === true),
                    cssClass: 'alert-checkbox',
                },
            ],
            cssClass: 'alert-checkbox-no-border cpe-alert-message',
        });

        await alert.present();
        await alert.onDidDismiss();
        await dismissed;

        return choice;
    }

    @debounce()
    async updateAvailable(installHandler: () => void) {
        const alert = await this.alertController.create({
            header: 'Update available',
            message: 'An update is ready for installation. Select "install" to install it and restart the app now.',
            backdropDismiss: false,
            buttons: [
                { text: 'Install', handler: installHandler },
                { text: 'Close', role: 'cancel' },
            ],
        });

        await alert.present();
        await alert.onDidDismiss();
    }

    @debounce()
    async fatalError(reason: string, emergencySave = true) {
        const haveCurrentSession = !!this.emulationState.currentSession();

        const alert = await this.alertController.create({
            header: 'Error',
            message: this.sanitizeMessage(`
                ${reason}
                <br/><br/>
                Please close or reload this window.
                ${
                    haveCurrentSession && emergencySave
                        ? ' You may save an image of your current session before reloading.'
                        : ''
                }
            `),
            backdropDismiss: false,
            buttons: [
                ...(haveCurrentSession && emergencySave
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
            cssClass: 'alert-error',
        });

        await alert.present();
        await alert.onDidDismiss();
    }

    @debounce()
    async errorInNativeCode(reason: string) {
        const alert = await this.alertController.create({
            header: 'Error',
            message: this.sanitizeMessage(`
                You encountered a bug in CloudpilotEmu:
                <br/><br/>
                ${reason}.
                <br/><br/>
                Please close or reload this window.
            `),
            backdropDismiss: false,
            buttons: [{ text: 'Reload', handler: () => window.location.reload() }],
        });

        await alert.present();
        await alert.onDidDismiss();
    }

    @debounce()
    async snapshotTimeout() {
        await this.message(
            'Timeout while saving state',
            `Your browser's IndexedDB is either slow to respond or has crashed. You can either retry or reload CloudpilotEmu.`,
            { Reload: () => window.location.reload() },
            'Retry',
        );
    }

    proxyVersionMismatchError() {
        return this.errorMessage(`
            Server version does not match CloudpilotEmu. Please make sure that you are using the latest
            versions of CloudpilotEmu and of the server.
        `);
    }

    cardHasNoValidFileSystem(): Promise<void> {
        return this.message('No filesystem', 'This card does not contain a valid filesystem.', {}, 'Continue');
    }

    cardHasUncorrectableErrors(): Promise<void> {
        return this.message(
            'Uncorrectable errors',
            'The filesystem on this card contains uncorrectable errors.',
            {},
            'Continue',
        );
    }

    cardIsClean(): Promise<void> {
        return this.message('Card clean', 'No filesystem errors were found.', {}, 'Continue');
    }

    sanitizeMessage(message: string | Verbatim): IonicSafeString {
        if (message instanceof Verbatim) return new IonicSafeString(message.markup);

        return new IonicSafeString(
            message
                .replace(/<br\s*\/?>/g, '<br/>')
                .replace(/\n/gm, ' ')
                .split('<br/>')
                .map((part) => {
                    this.sanitizeDiv.textContent = part;
                    return this.sanitizeDiv.innerHTML;
                })
                .join('<br/>'),
        );
    }

    emergencySaveEvent = new Event<void>();

    private sanitizeDiv = document.createElement('div');
}
