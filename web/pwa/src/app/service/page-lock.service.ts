import { ErrorService } from './error.service';
import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { pageLockIntact } from '../helper/pagelock';

@Injectable({
    providedIn: 'root',
})
export class PageLockService {
    constructor(private errorService: ErrorService) {}

    lockLost(): boolean {
        if (this.wasLockLost) return true;

        this.wasLockLost = !pageLockIntact();

        if (this.wasLockLost) {
            this.errorService.fatalPageLockLost();
        }

        return this.wasLockLost;
    }

    private wasLockLost = false;
}
