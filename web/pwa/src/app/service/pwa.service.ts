import { isAndroid, isIOS, isIOSSafari } from '@common/helper/browser';

import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';
import { InstallationMode } from '@pwa/model/InstallationMode';
import { KvsService } from './kvs.service';

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
    Tap the "Share" icon on the bottom of the screen (the little box with the arrow)
    and select "Add to Home Screen" in order to install CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and makes sure that Safari never deletes your sessions.
`;

const INSTRUCTIONS_IOS = `
    Open CloudpilotEmu in Safari, tap the "Share" icon on the bottom of the screen
    (the little box with the arrow) and select "Add to Home Screen" in order to install
    CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INVITATION_ANDROID = `
    Howdy! It seems that you are using CloudpilotEmu on an Android device. You can
    install CloudpilotEmu as an app by opening your browser's menu
    and tapping "Add to Home screen".
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INVITATION_SAFARI_IOS = `
    Howdy! It seems that you are using CloudpilotEmu on an iPhone or iPad. You can
    install CloudpilotEmu as an app by tapping the "Share" icon
    at the bottom of the screen (the little box with the arrow) and selecting
    "Add to Home Screen".
    <br/><br/>
    This removes the browser UI and makes sure that Safari never deletes your sessions.
`;

const INVITATION_IOS = `
    Howdy! It seems that you are using CloudpilotEmu on an iPhone or iPad. You can
    install CloudpilotEmu as an app by opening it in Safari,
    tapping the "Share" icon at the bottom of the screen (the little box with the arrow)
    and selecting "Add to Home Screen"
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

@Injectable({
    providedIn: 'root',
})
export class PwaService {
    constructor(private kvsService: KvsService, private alertService: AlertService) {
        this.serviceWorkerKeepalive();
    }

    promptForInstall(): boolean {
        return (isIOS || isAndroid) && this.installationMode === InstallationMode.web;
    }

    getInstallationMode(): InstallationMode {
        return this.installationMode;
    }

    install(): void {
        this.alertService.message('Install app', this.getInstallMessage());
    }

    invite(): void {
        if (!this.promptForInstall()) return;

        if (this.kvsService.kvs.didShowInvitation) return;
        this.kvsService.kvs.didShowInvitation = true;

        if (!window.hasOwnProperty('onbeforeinstallprompt')) {
            this.alertService.message('App available', this.getInviteMessage());
        }
    }

    private getInstallMessage(): string {
        if (isIOS && isIOSSafari) return INSTRUCTIONS_SAFARI_IOS;
        if (isIOS) return INSTRUCTIONS_IOS;

        return INSTRUCTIONS_ANDROID;
    }

    private getInviteMessage(): string {
        if (isIOS && isIOSSafari) return INVITATION_SAFARI_IOS;
        if (isIOS) return INVITATION_IOS;

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

    private installationMode: InstallationMode = this.determineInstallationMode();
}
