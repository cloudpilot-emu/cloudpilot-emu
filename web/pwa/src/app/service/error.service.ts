import { Injectable, NgZone } from '@angular/core';
import { Event } from 'microevent.ts';

import { clearStoredSession } from '@pwa/helper/storedSession';

import { AlertService } from './alert.service';
import { NativeAppService } from './native-app.service';

@Injectable({
    providedIn: 'root',
})
export class ErrorService {
    constructor(
        private alertService: AlertService,
        private nativeAppService: NativeAppService,
        private ngZone: NgZone,
    ) {}

    hasFatalError(): boolean {
        return this.fatalErrorTriggered;
    }

    fatalPageLockLost = () => this.fatalWithMessage('CloudpilotEmu was opened in another tab or window.', false);

    fatalIDBDead = () => {
        if (this.nativeAppService.isResumeFromBackground()) {
            window.location.reload();
        } else {
            this.fatalWithMessage('IndexedDB access failed. This is most likely a browser bug.', false);
        }
    };

    fatalVersionMismatch = () =>
        this.fatalWithMessage(
            `
        Database was written by a newer version of CloudpilotEmu. Please wait for a few seconds before reloading
        while the latest version downloads in the background.
    `,
            false,
        );

    fatalBug = (reason: string) => this.fatalWithMessage(`You encountered a bug in CloudpilotEmu:<br/><br/>${reason}.`);

    fatalInNativeCode = (e: Error) => this.fatalWithMessage(e.message, true, false);

    private fatalWithMessage = (message: string, clearSession = true, emergencySave = true) =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            if (clearSession) clearStoredSession();

            this.fatalErrorEvent.dispatch();

            void this.alertService.fatalError(message, emergencySave);
        });

    fatalErrorEvent = new Event<void>();

    private fatalErrorTriggered = false;
}
