import { AlertService } from './alert.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { pageLockIntact } from '../helper/pagelock';

@Injectable({
    providedIn: 'root',
})
export class PageLockService {
    constructor(private alertService: AlertService) {}

    lockLost(): boolean {
        if (this.wasLockLost) return true;

        this.wasLockLost = !pageLockIntact();

        if (this.wasLockLost) {
            this.alertService.fatalError('Cloudpilot was opened in another tab or windoow.');
            this.lockLostEvent.dispatch();
        }

        return this.wasLockLost;
    }

    lockLostEvent = new Event<void>();

    private wasLockLost = false;
}
