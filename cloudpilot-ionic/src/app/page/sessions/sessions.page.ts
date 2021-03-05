import { AlertController, ModalController } from '@ionic/angular';
import { FileDescriptor, FileService } from './../../service/file.service';
import { SessionSettings, SessionSettingsComponent } from './settings/settings/settings.component';

import { AlertService } from './../../service/alert.service';
import { Component } from '@angular/core';
import { EmulationService } from './../../service/emulation.service';
import { EmulationStateService } from './../../service/emulation-state.service';
import { Router } from '@angular/router';
import { Session } from './../../model/Session';
import { SessionService } from 'src/app/service/session.service';
import { StorageService } from './../../service/storage.service';
import deepEqual from 'deep-equal';
import { deserializeSessionImage } from 'src/app/helper/sessionFile';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
})
export class SessionsPage {
    constructor(
        public sessionService: SessionService,
        private fileService: FileService,
        private alertController: AlertController,
        private alertService: AlertService,
        public emulationService: EmulationService,
        public emulationState: EmulationStateService,
        private storageService: StorageService,
        private modalController: ModalController,
        private router: Router
    ) {}

    get sessions(): Array<Session> {
        return this.sessionService.getSessions().sort((a, b) => a.name.localeCompare(b.name));
    }

    async deleteSession(session: Session): Promise<void> {
        const alert = await this.alertController.create({
            header: 'Warning',
            message: `Deleting the session '${session.name}' will remove all associated data. This cannot be undone. Are you sure you want to continue?`,
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                { text: 'Delete', handler: () => this.sessionService.deleteSession(session) },
            ],
        });

        await alert.present();
    }

    async editSession(session: Session): Promise<void> {
        const oldSession = { ...session };

        if ((await this.editSettings(session)) && !deepEqual(session, oldSession)) {
            this.sessionService.updateSession(session);
        }
    }

    loadFile(): void {
        this.fileService.openFile(this.processFile.bind(this));
    }

    saveSession(session: Session): void {
        this.fileService.saveSession(session);
    }

    async launchSession(session: Session) {
        this.lastSessionTouched = session.id;
        this.currentSessionOverride = session.id;

        await this.emulationService.switchSession(session.id);

        this.currentSessionOverride = undefined;

        this.router.navigateByUrl('/tab/emulation');
    }

    async resetSession(session: Session) {
        const running = session.id === this.emulationState.getCurrentSession()?.id;

        if (running) await this.emulationService.stop();

        await this.storageService.deleteStateForSession(session);

        if (running) await this.emulationService.switchSession(session.id);

        const alert = await this.alertController.create({
            header: 'Done',
            message: `State for session ${session.name} has been reset.`,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }

    get currentSessionId(): number | undefined {
        return this.currentSessionOverride ?? this.emulationState.getCurrentSession()?.id;
    }

    private async processFile(file: FileDescriptor): Promise<void> {
        if (!/\.(img|rom|bin)$/i.test(file.name)) {
            this.alertService.errorMessage('Unsupported file suffix. Supported suffixes are .bin, .img and .rom.');

            return;
        }

        const sessionImage = deserializeSessionImage(file.content);

        if (sessionImage) {
            const settings: SessionSettings = {
                name: this.disambiguateSessionName(sessionImage.metadata?.name ?? file.name),
                hotsyncName: sessionImage.metadata?.hotsyncName,
            };

            if (await this.editSettings(settings)) {
                const session = await this.sessionService.addSessionFromImage(sessionImage, settings.name, settings);

                this.lastSessionTouched = session.id;
            }
        } else {
            const romInfo = (await this.emulationService.cloudpilot).getRomInfo(file.content);

            if (!romInfo) {
                this.alertService.errorMessage(`Not a valid session file or ROM image.`);
                return;
            }

            if (romInfo.supportedDevices.length === 0) {
                this.alertService.errorMessage(`No supported device for this ROM.`);
                return;
            }

            const settings: SessionSettings = { name: this.disambiguateSessionName(file.name) };

            if (await this.editSettings(settings)) {
                const session = await this.sessionService.addSessionFromRom(
                    file.content,
                    settings.name,
                    romInfo.supportedDevices[0],
                    settings
                );

                this.lastSessionTouched = session.id;
            }
        }
    }

    private editSettings(session: SessionSettings): Promise<boolean> {
        return new Promise((resolve) => {
            let modal: HTMLIonModalElement;

            this.modalController
                .create({
                    component: SessionSettingsComponent,
                    backdropDismiss: false,
                    componentProps: {
                        session,
                        onSave: () => {
                            modal.dismiss();
                            resolve(true);
                        },
                        onCancel: () => {
                            modal.dismiss();
                            resolve(false);
                        },
                    },
                })
                .then((m) => {
                    modal = m;
                    modal.present();
                });
        });
    }

    private disambiguateSessionName(originalName: string) {
        const sessions = this.sessionService.getSessions();
        let name = originalName;

        let i = 1;
        while (sessions.some((s) => s.name === name)) {
            name = `${originalName} (${i++})`;
        }

        return name;
    }

    private currentSessionOverride: number | undefined;
    lastSessionTouched = -1;
}
