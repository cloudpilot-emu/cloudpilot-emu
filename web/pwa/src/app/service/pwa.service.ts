import { isAndroid, isIOS, isIOSSafari, isIOS_174 } from '@common/helper/browser';
import { v4 as uuid } from 'uuid';

import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';
import { InstallationMode } from '@pwa/model/InstallationMode';
import { KvsService } from './kvs.service';
import { CpeLinkService } from './cpe-link.service';
import { FileService } from './file.service';

const SERVICE_WORKER_KEEPALIVE_INTERVAL = 15 * 60 * 1000;

declare global {
    interface Navigator {
        standalone?: boolean;
    }
}

const INSTRUCTIONS_ANDROID = `
    Open your browser's menu and tap "Add to Home screen" or "Install App"
    in order to install CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INSTRUCTIONS_SAFARI_IOS = `
    Tap the "Share" icon in the browser toolbar
    and select "Add to Home Screen" in order to install CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and makes sure that Safari never deletes your sessions.
`;

const INSTRUCTIONS_IOS = `
    Open CloudpilotEmu in Safari, tap the "Share" icon in the browser toolbar
    and select "Add to Home Screen" in order to install
    CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INVITATION_ANDROID = `
    Howdy! It seems that you are using an Android device. You can
    install CloudpilotEmu as an app by opening your browser's menu
    and tapping "Add to Home screen".
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INVITATION_SAFARI_IOS = `
    Howdy! It seems that you are using an iPhone or iPad. You can
    install CloudpilotEmu as an app by tapping the "Share" icon
    in the browser toolbar and selecting
    "Add to Home Screen".
    <br/><br/>
    This removes the browser UI and makes sure that Safari never deletes your sessions.
`;

const INVITATION_IOS = `
    Howdy! It seems that you are using an iPhone or iPad. You can
    install CloudpilotEmu as an app by opening it in Safari,
    tapping the "Share" icon in the browser toolbar
    and selecting "Add to Home Screen"
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const WARNING_IOS_174 = `
    <br/><br/>
    WARNING: This may be currently broken within the European Union. Please check the "About"
    for details.
`;

@Injectable({
    providedIn: 'root',
})
export class PwaService {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private fileService: FileService,
        cpeLinkService: CpeLinkService,
    ) {
        void this.serviceWorkerKeepalive();

        cpeLinkService.clickEvent.addHandler((key) => key === 'itp-reminder' && this.downloadItpReminder());
    }

    promptForInstall(): boolean {
        return (isIOS || isAndroid) && this.installationMode === InstallationMode.web;
    }

    getInstallationMode(): InstallationMode {
        return this.installationMode;
    }

    install(): void {
        void this.alertService.message('Install app', this.getInstallMessage());
    }

    invite(): void {
        if (!this.promptForInstall()) return;

        if (this.kvsService.kvs.didShowInvitation) return;
        this.kvsService.kvs.didShowInvitation = true;

        if (!window.hasOwnProperty('onbeforeinstallprompt')) {
            void this.alertService.message('App available', this.getInviteMessage());
        }
    }

    private getInstallMessage(): string {
        if (isIOS && isIOSSafari) return INSTRUCTIONS_SAFARI_IOS + (isIOS_174 ? WARNING_IOS_174 : '');
        if (isIOS) return INSTRUCTIONS_IOS + (isIOS_174 ? WARNING_IOS_174 : '');

        return INSTRUCTIONS_ANDROID;
    }

    private getInviteMessage(): string {
        if (isIOS && isIOSSafari) return INVITATION_SAFARI_IOS + (isIOS_174 ? WARNING_IOS_174 : '');
        if (isIOS) return INVITATION_IOS + (isIOS_174 ? WARNING_IOS_174 : '');

        return INVITATION_ANDROID;
    }

    private determineInstallationMode(): InstallationMode {
        if (window.navigator.standalone) return InstallationMode.pwa;

        if (document.referrer.startsWith('android-app://')) return InstallationMode.twa;

        if (window.matchMedia('(display-mode: standalone)').matches) return InstallationMode.pwa;

        return InstallationMode.web;
    }

    private serviceWorkerKeepalive = async () => {
        if (navigator.serviceWorker) {
            try {
                const registration = await navigator.serviceWorker.getRegistration();

                if (registration?.active?.postMessage) {
                    registration.active.postMessage({});
                }
            } catch (e) {}
        }

        setTimeout(this.serviceWorkerKeepalive, SERVICE_WORKER_KEEPALIVE_INTERVAL);
    };

    private downloadItpReminder(): void {
        const stampNow = new Date().toISOString().replace(/[:\-\.]/g, '');
        const dtStart = new Date(Date.now() + 48 * 3600 * 1000).toISOString().replace(/\-/g, '').substring(0, 8);
        const dtEnd = new Date(Date.now() + 72 * 3600 * 1000).toISOString().replace(/\-/g, '').substring(0, 8);
        const uuidVEvent = uuid().toUpperCase();
        const uuidVAlarm = uuid().toUpperCase();
        const url = location.origin + location.pathname.replace('index.html', '');

        const ics = `
BEGIN:VCALENDAR
CALSCALE:GREGORIAN
VERSION:2.0
X-WR-CALNAME:Open Cloudpilot
METHOD:PUBLISH
PRODID:CloudpilotEmu
BEGIN:VEVENT
TRANSP:TRANSPARENT
DTEND;VALUE=DATE:${dtEnd}
UID:${uuidVEvent}
DTSTAMP:${stampNow}
URL;VALUE=URI:${url}
SEQUENCE:3
SUMMARY:Open CloudpilotEmu
DTSTART;VALUE=DATE:${dtStart}
LAST-MODIFIED:${stampNow}
CREATED:${stampNow}
RRULE:FREQ=DAILY;INTERVAL=6
BEGIN:VALARM
UID:${uuidVAlarm}
X-WR-ALARMUID:${uuidVAlarm}
DESCRIPTION:Reminder
TRIGGER:-PT15H
ACTION:DISPLAY
END:VALARM
BEGIN:VALARM
UID:${uuidVAlarm}
TRIGGER:-PT15H
DESCRIPTION:Reminder
X-WR-ALARMUID:${uuidVAlarm}
ACTION:DISPLAY
END:VALARM
END:VEVENT
END:VCALENDAR`;

        this.fileService.saveFile('reminder.ics', new TextEncoder().encode(ics), 'text/calendar');
    }

    private installationMode: InstallationMode = this.determineInstallationMode();
}
