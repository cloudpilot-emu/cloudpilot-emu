import { Cloudpilot } from '../helper/Cloudpilot';
import { Device } from '../model/Device';
import { FileService } from './file.service';
import { Injectable } from '@angular/core';
import { Session } from '../model/Session';

const SESSIONS: Array<Session> = [
    {
        id: '1',
        name: 'Palm V, default',
        device: Device.palmV,
        ram: 2,
        osVersion: '3.5',
    },
    {
        id: '2',
        name: 'Palm V, Lemmings',
        device: Device.palmV,
        ram: 2,
        osVersion: '4',
    },
    {
        id: '3',
        name: 'Palm m515',
        device: Device.m515,
        ram: 8,
        osVersion: '4.0',
    },
    {
        id: '4',
        name: 'Palm m515, unbooted',
        device: Device.m515,
        ram: 8,
    },
];

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(private fileService: FileService) {}

    newSession(): void {
        this.fileService.openFile('.bin, .img, .rom', (f) =>
            console.log(this.fileService.parseSessionImage(f.content))
        );
    }

    getSessions(): Array<Session> {
        return this.sessions;
    }

    deleteSession(session: Session): void {
        this.sessions = this.sessions.filter((s) => s.id !== session.id);
    }

    updateSession(session: Session): void {
        this.sessions = this.sessions.map((s) => (s.id == session.id ? session : s));
    }

    private cloudpilotInstance: Promise<Cloudpilot> = Cloudpilot.create();

    private sessions = SESSIONS;
}
