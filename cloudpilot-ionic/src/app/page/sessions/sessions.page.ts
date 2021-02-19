import { FileDescriptor, FileService } from './../../service/file.service';

import { AlertController } from '@ionic/angular';
import { Component } from '@angular/core';
import { EmulationService } from './../../service/emulation.service';
import { Router } from '@angular/router';
import { Session } from 'src/app/model/Session';
import { SessionService } from 'src/app/service/session.service';

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
        private emulationService: EmulationService,
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
        const newName = await this.queryName(session.name);

        if (newName !== undefined) {
            this.sessionService.updateSession({ ...session, name: newName });
        }
    }

    loadFile(): void {
        this.fileService.openFile('.img, .rom, .bin', this.processFile.bind(this));
    }

    launchSession(session: Session) {
        this.emulationService.switchSession(session.id);
        this.router.navigateByUrl('/tab/emulation');
    }

    private async processFile(file: FileDescriptor): Promise<void> {
        const sessionImage = this.fileService.parseSessionImage(file.content);

        if (sessionImage) {
            const name = await this.queryName(file.name);

            if (name !== undefined) this.sessionService.addSessionFromImage(sessionImage, name);
        } else {
            const romInfo = (await this.emulationService.cloudpilot).getRomInfo(file.content);

            if (!romInfo) {
                this.errorMessage(`Not a valid session file or ROM image.`);
                return;
            }

            if (romInfo.supportedDevices.length === 0) {
                this.errorMessage(`No supported device for this ROM.`);
                return;
            }

            const name = await this.queryName(file.name);

            if (name !== undefined) {
                this.sessionService.addSessionFromRom(file.content, name, romInfo.supportedDevices[0]);
            }
        }
    }

    private queryName(current?: string): Promise<string | undefined> {
        return new Promise(async (resolve) => {
            const alert = await this.alertController.create({
                header: 'Session Name',
                inputs: [
                    {
                        type: 'textarea',
                        name: 'name',
                        placeholder: 'Session name',
                        value: current,
                        label: 'Name',
                    },
                ],
                buttons: [
                    { text: 'Cancel', role: 'cancel', handler: () => resolve(undefined) },
                    {
                        text: 'Continue',
                        handler: (data) => resolve(data.name),
                    },
                ],
            });

            alert.present();
        });
    }

    private async errorMessage(message: string) {
        const alert = await this.alertController.create({
            header: 'Error',
            message,
            buttons: [{ text: 'Close', role: 'cancel' }],
        });

        await alert.present();
    }
}
