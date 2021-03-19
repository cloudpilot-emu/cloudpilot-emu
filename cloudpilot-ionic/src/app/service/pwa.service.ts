import { isAndroid, isIOS, isIOSSafari, isSafari } from './../helper/browser';

import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';
import { InstallationMode } from '../model/InstallationMode';

declare global {
    interface Navigator {
        standalone?: boolean;
    }
}

const MESSAGE_ANDROID = `
    Open your browser's menu and tap the "Add to Home screen" in order to
    add Cloudpilot as an app to your homescreen.
    <br/><br/>
    This will remove the browser UI and give you a native app experience.
`;

const MESSAGE_SAFARI_IOS = `
    Tap the "Share" icon on the bottom of the screen (the little box with the arrow)
    and select "Add to Home Screen" in order to add Cloudpilot as an app to your homescreen.
    <br/><br/>
    This will not only remove the browser UI and give you a native app experience
    but will also ensure that Safari will never remove your sessions.
`;

const MESSAGE_IOS = `
    Open Cloudpilot in Safari, tap the "Share" icon on the bottom of the screen
    (the little box with the arrow) and select "Add to Home Screen" in order to add
    Cloudpilot as a native app to your homescreen.
    <br/><br/>
    This will remove the browser UI and give you a native app experience.
`;

@Injectable({
    providedIn: 'root',
})
export class PwaService {
    constructor(public alertService: AlertService) {}

    promptForInstall(): boolean {
        return (isIOS || isAndroid) && this.installationMode === InstallationMode.web;
    }

    getInstallationMode(): InstallationMode {
        return this.installationMode;
    }

    install(): void {
        this.alertService.message('Install app', this.getInstallMessage());
    }

    private getInstallMessage(): string {
        if (isIOS && isIOSSafari) return MESSAGE_SAFARI_IOS;
        if (isIOS) return MESSAGE_IOS;

        return MESSAGE_ANDROID;
    }

    private determineInstallationMode(): InstallationMode {
        if (window.navigator.standalone) return InstallationMode.pwa;

        if (document.referrer.startsWith('android-app://')) return InstallationMode.twa;

        if (window.matchMedia('(display-mode: standalone)').matches) return InstallationMode.pwa;

        return InstallationMode.web;
    }

    private installationMode: InstallationMode = this.determineInstallationMode();
}
