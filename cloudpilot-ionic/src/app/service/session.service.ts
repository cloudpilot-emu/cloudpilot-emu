import { Cloudpilot } from './Cloudpilot';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor() {}

    newSession(): void {
        this.input = document.createElement('input');

        this.input.type = 'file';
        this.input.accept = '.bin, .rom, .img';

        this.input.click();
    }

    private input: HTMLInputElement = null;
    private cloudpilotInstance: Promise<Cloudpilot> = Cloudpilot.create();
}
