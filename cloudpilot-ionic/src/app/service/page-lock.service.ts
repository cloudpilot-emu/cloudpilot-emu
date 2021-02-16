import { Injectable } from '@angular/core';
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
        return localStorage.getItem(KEY) === this.token;
    }

    private token = uuid();
}
