import { Injectable, NgZone } from '@angular/core';

import { AlertService } from 'src/app/service/alert.service';
import { Event } from 'microevent.ts';
import { clearStoredSession } from '../helper/storedSession';

@Injectable({
    providedIn: 'root',
})
export class ErrorService {
    constructor(private alertService: AlertService, private ngZone: NgZone) {}

    hasFatalError(): boolean {
        return this.fatalErrorTriggered;
    }

    fatalPageLockLost = () =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            this.fatalErrorEvent.dispatch();

            this.alertService.fatalError('Cloudpilot was opened in another tab or windoow.');
        });

    fatalIDBDead = () =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            this.fatalErrorEvent.dispatch();

            this.alertService.fatalError('IndexedDB access lost. This is most likely a browser bug.');
        });

    fatalBug = (reason: string) =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            clearStoredSession();

            this.fatalErrorEvent.dispatch();

            this.alertService.fatalError(`You encountered a bug in cloudpilot:<br/><br/>${reason}.`);
        });

    fatalInNativeCode = (e: Error) =>
        this.ngZone.run(() => {
            if (this.fatalErrorTriggered) return;
            this.fatalErrorTriggered = true;

            clearStoredSession();

            this.fatalErrorEvent.dispatch();

            this.alertService.errorInNativeCode(e.message);
        });

    fatalErrorEvent = new Event<void>();

    private fatalErrorTriggered = false;
}
