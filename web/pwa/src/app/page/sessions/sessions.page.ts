import { AlertController, ModalController } from '@ionic/angular';
import { FileDescriptor, FileService } from './../../service/file.service';
import { SessionSettings, SessionSettingsComponent } from '../../component/session-settings/session-settings.component';

import { AlertService } from './../../service/alert.service';
import { CloudpilotService } from './../../service/cloudpilot.service';
import { Component } from '@angular/core';
import { EmulationService } from './../../service/emulation.service';
import { EmulationStateService } from './../../service/emulation-state.service';
import { HelpComponent } from '../../component/help/help.component';
import { LinkApi } from './../../service/link-api.service';
import { Router } from '@angular/router';
import { Session } from './../../model/Session';
import { SessionMetadata } from '../../model/SessionMetadata';
import { SessionService } from '../../service/session.service';
import { StorageService } from './../../service/storage.service';
import deepEqual from 'deep-equal';

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
        private linkApi: LinkApi,
        private router: Router,
        private cloudpilotService: CloudpilotService
    ) {}

    ionViewDidEnter(): void {
        this.linkApi.import.requestEvent.addHandler(this.handleLinkApiImportRequest);
        this.handleLinkApiImportRequest();
    }

    ionViewWillLeave(): void {
        this.linkApi.import.requestEvent.removeHandler(this.handleLinkApiImportRequest);
    }

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

    importSession(): void {
        this.fileService.openFile(this.processFile.bind(this));
    }

    saveSession(session: Session): void {
        this.fileService.saveSession(session);
    }

    trackSessionBy(index: number, session: Session) {
        return session.id;
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

        session.wasResetForcefully = true;
        await this.storageService.updateSession(session);

        if (running) await this.emulationService.switchSession(session.id);

        const alert = await this.alertController.create({
            header: 'Done',
            message: `Session ${session.name} has been reset.`,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }

    get currentSessionId(): number | undefined {
        return this.currentSessionOverride ?? this.emulationState.getCurrentSession()?.id;
    }

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/sessions.md',
            },
        });
        await modal.present();
    }

    private async processFile(file: FileDescriptor): Promise<void> {
        if (!/\.(img|rom|bin)$/i.test(file.name)) {
            this.alertService.errorMessage('Unsupported file suffix. Supported suffixes are .bin, .img and .rom.');

            return;
        }

        const sessionImage = (await this.cloudpilotService.cloudpilot).deserializeSessionImage<SessionMetadata>(
            file.content
        );

        if (sessionImage) {
            const settings: SessionSettings = {
                name: this.disambiguateSessionName(sessionImage.metadata?.name ?? file.name),
                hotsyncName: sessionImage.metadata?.hotsyncName,
                device: sessionImage.deviceId,
                dontManageHotsyncName: sessionImage.metadata?.dontManageHotsyncName,
                speed: sessionImage.metadata?.speed,
                deviceOrientation: sessionImage.metadata?.deviceOrientation,
            };

            if (await this.editSettings(settings)) {
                const session = await this.sessionService.addSessionFromImage(sessionImage, settings.name, settings);

                this.lastSessionTouched = session.id;
            }
        } else {
            const romInfo = (await this.cloudpilotService.cloudpilot).getRomInfo(file.content);

            if (!romInfo) {
                this.alertService.errorMessage(`Not a valid session file or ROM image.`);
                return;
            }

            if (romInfo.supportedDevices.length === 0) {
                this.alertService.errorMessage(`No supported device for this ROM.`);
                return;
            }

            const settings: SessionSettings = {
                name: this.disambiguateSessionName(file.name),
                hotsyncName: '',
                device: romInfo.supportedDevices[0],
            };

            if (await this.editSettings(settings, romInfo.supportedDevices)) {
                const session = await this.sessionService.addSessionFromRom(
                    file.content,
                    settings.name,
                    settings.device,
                    settings
                );

                this.lastSessionTouched = session.id;
            }
        }
    }

    private editSettings(session: SessionSettings, availableDevices = [session.device]): Promise<boolean> {
        return new Promise((resolve) => {
            let modal: HTMLIonModalElement;

            this.modalController
                .create({
                    component: SessionSettingsComponent,
                    backdropDismiss: false,
                    componentProps: {
                        session,
                        availableDevices,
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

    private handleLinkApiImportRequest = (): void => {
        if (!this.linkApi.import.hasPendingRequest()) {
            return;
        }

        const url = this.linkApi.import.receivePendingUrl();
        if (!url) {
            return;
        }

        this.alertService.message(
            'Import request',
            `Do you want to import<br>${url} ?`,
            {
                OK: () => this.fileService.openUrl(url, this.processFile.bind(this)),
            },
            'Cancel'
        );
    };

    private currentSessionOverride: number | undefined;
    lastSessionTouched = -1;
}
