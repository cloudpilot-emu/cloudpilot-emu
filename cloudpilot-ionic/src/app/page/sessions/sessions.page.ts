import { Component, OnInit } from '@angular/core';

import { AlertController } from '@ionic/angular';
import { Session } from 'src/app/model/Session';
import { SessionService } from 'src/app/service/session.service';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
})
export class SessionsPage {
    constructor(public sessionService: SessionService, private alertController: AlertController) {}

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
        const alert = await this.alertController.create({
            header: 'Edit Session',
            message: 'Please enter a new session name',
            inputs: [
                {
                    type: 'textarea',
                    name: 'name',
                    placeholder: 'Session name',
                    value: session.name,
                    label: 'Name',
                },
            ],
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                {
                    text: 'Apply',
                    handler: (data) => this.sessionService.updateSession({ ...session, name: data.name }),
                },
            ],
        });

        await alert.present();
    }
}
