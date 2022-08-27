import { Injectable, NgZone } from '@angular/core';

import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { clearStoredSession } from '@pwa/helper/storedSession';

@Injectable({
    providedIn: 'root',
})
export class ErrorService {
    constructor(private alertService: AlertService, private ngZone: NgZone) {}

    hasFatalError(): boolean {
        return this.fatalErrorTriggered;
    }

    fatalPageLockLost = () => this.fatalWithMessage('Cloudpilot was opened in another tab or window.', false);

    fatalIDBDead = () => this.fatalWithMessage('IndexedDB access failed. This is most likely a browser bug.', false);

    fatalVersionMismatch = () =>
        this.fatalWithMessage(
            `
        Database was written by a newer version of Cloudpilot. Please wait for a few seconds before reloading
        while the latest version downloads in the background.
    `,
            false
        );

    fatalBug = (reason: string) => this.fatalWithMessage(`You encountered a bug in Cloudpilot:<br/><br/>${reason}.`);

    fatalInNativeCode = (e: Error) => this.fatalWithMessage(e.message, true, false);

    private fatalWithMessage = (message: string, clearSession = true, emergencySave = true) =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            if (clearSession) clearStoredSession();

            this.fatalErrorEvent.dispatch();

            this.alertService.fatalError(message, emergencySave);
        });

    fatalErrorEvent = new Event<void>();

    private fatalErrorTriggered = false;
}
