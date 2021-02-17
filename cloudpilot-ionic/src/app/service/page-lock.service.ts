import { EventEmitter, Injectable } from '@angular/core';

import { v4 as uuid } from 'uuid';

const KEY = 'cloudpilot-lock';

@Injectable({
    providedIn: 'root',
})
export class PageLockService {
    constructor() {
        localStorage.setItem(KEY, this.token);
    }

    lockLost(): boolean {
        if (localStorage.getItem(KEY) !== this.token) {
            if (!this.lockLostFired) this.lockLostDetected.emit();

            this.lockLostFired = true;

            return true;
        }

        return false;
    }

    lockLostDetected = new EventEmitter<void>();

    private token = uuid();
    private lockLostFired = false;
}
