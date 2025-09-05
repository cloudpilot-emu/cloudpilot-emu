import { Injectable } from '@angular/core';
import { isAndroid, isIOS, isMacOSSafari } from '@common/helper/browser';

import { InstallationMode } from '@pwa/model/InstallationMode';

import { AlertService } from './alert.service';
import { KvsService } from './kvs.service';

const SERVICE_WORKER_KEEPALIVE_INTERVAL = 15 * 60 * 1000;

declare global {
    interface Navigator {
        standalone?: boolean;
    }
}

const APPSTORE_LINK =
    '<a href="https://apps.apple.com/us/app/cloudpilotemu/id6478502699" target="_blank">App Store</a>';

const INSTRUCTIONS_ANDROID = `
    Open your browser's menu and tap "Add to Home screen" or "Install App"
    in order to install CloudpilotEmu as an app.
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INSTRUCTIONS_IOS = `
    You can install CloudpilotEmu as an app either from the
    ${APPSTORE_LINK} or by selecting "Share" and "Add to Home Screen"
    in Safari.
    <br/><br/>
    If you continue to run CloudpilotEmu in the browser iOS will remove your data after seven days
    of inactivity.
`;

const INVITATION_ANDROID = `
    Howdy! It seems that you are using an Android device. You can
    install CloudpilotEmu as an app by opening your browser's menu
    and tapping "Add to Home screen".
    <br/><br/>
    This removes the browser UI and gives you a native app experience.
`;

const INVITATION_IOS = `
    Howdy! It seems that you are using an iPhone or iPad. You can
    install CloudpilotEmu as an app either from the
    ${APPSTORE_LINK} or by selecting "Share"
    and "Add to Home Screen" in Safari.
    <br/><br/>
    If you continue to run CloudpilotEmu in the browser iOS will remove your data after seven days
    of inactivity.
`;

const INVITATION_SAFARI_MACOS = `
    Howdy! It seems that you are running CloudpilotEmu on a Mac. Safari will delete all
    data after seven days of inactivity. However, you can click the "Share"
    icon in the browser toolbar and select "Add to dock" to install
    CloudpilotEmu as an app.
    <br><br>
    This will keep Safari from deleting your
    data.
`;

@Injectable({
    providedIn: 'root',
})
export class PwaService {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
    ) {
        void this.serviceWorkerKeepalive();
    }

    promptForInstall(): boolean {
        return (isIOS || isAndroid) && this.installationMode === InstallationMode.web;
    }

    getInstallationMode(): InstallationMode {
        return this.installationMode;
    }

    install(): void {
        void this.alertService.message('Install app', this.alertService.verbatim(this.getInstallMessage()));
    }

    invite(): void {
        if (!((isIOS || isAndroid || isMacOSSafari) && this.installationMode === InstallationMode.web)) return;

        if (this.kvsService.kvs.didShowInvitation) return;
        this.kvsService.kvs.didShowInvitation = true;

        if (!Object.prototype.hasOwnProperty.call(window, 'onbeforeinstallprompt')) {
            void this.alertService.message('App available', this.alertService.verbatim(this.getInviteMessage()));
        }
    }

    private getInstallMessage(): string {
        if (isIOS) return INSTRUCTIONS_IOS;

        return INSTRUCTIONS_ANDROID;
    }

    private getInviteMessage(): string {
        if (isIOS) return INVITATION_IOS;

        if (isMacOSSafari) return INVITATION_SAFARI_MACOS;

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
            } catch (e) {
                console.error(e);
            }
        }

        setTimeout(this.serviceWorkerKeepalive, SERVICE_WORKER_KEEPALIVE_INTERVAL);
    };

    private installationMode: InstallationMode = this.determineInstallationMode();
}
