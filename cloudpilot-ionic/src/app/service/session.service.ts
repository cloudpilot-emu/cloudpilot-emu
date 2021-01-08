import { Observable, of } from 'rxjs';

import { Cloudpilot } from './Cloudpilot';
import { Device } from '../model/Device';
import { Injectable } from '@angular/core';
import { Session } from '../model/Session';

const SESSIONS: Array<Session> = [
    {
        name: 'Palm V, default',
        device: Device.palmV,
        ram: 2,
        osVersion: '3.5',
    },
    {
        name: 'Palm V, Lemmings',
        device: Device.palmV,
        ram: 2,
        osVersion: '4',
        description: 'Palm V with OS4 for better compatibility with Lemmings',
    },
    {
        name: 'Palm m515',
        device: Device.m515,
        ram: 8,
        osVersion: '4.0',
    },
    {
        name: 'Palm m515, unbooted',
        device: Device.m515,
        ram: 8,
    },
];

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

    getSessions$(): Observable<Array<Session>> {
        return of(SESSIONS);
    }

    private input: HTMLInputElement = null;
    private cloudpilotInstance: Promise<Cloudpilot> = Cloudpilot.create();
}
