import { DeviceId } from './../model/DeviceId';
import { EmulationService } from './emulation.service';
import { Injectable } from '@angular/core';
import { Session } from '../model/Session';
import { SessionImage } from './file.service';
import { v4 as uuidv4 } from 'uuid';

const SESSIONS: Array<Session> = [
    {
        id: '1',
        name: 'Palm V, default',
        device: DeviceId.palmV,
        ram: 2,
        osVersion: '3.5',
    },
    {
        id: '2',
        name: 'Palm V, Lemmings',
        device: DeviceId.palmV,
        ram: 2,
        osVersion: '4',
    },
    {
        id: '3',
        name: 'Palm m515',
        device: DeviceId.m515,
        ram: 8,
        osVersion: '4.0',
    },
    {
        id: '4',
        name: 'Palm m515, unbooted',
        device: DeviceId.m515,
        ram: 8,
    },
];

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(private emulationService: EmulationService) {}

    async addSessionFromImage(image: SessionImage, name: string) {
        const session: Session = {
            id: uuidv4(),
            name,
            device: image.deviceId as DeviceId,
            ram: image.memory.length / 1024 / 1024,
        };

        SESSIONS.push(session);
    }

    async addSessionFromRom(rom: Uint8Array, name: string, device: DeviceId) {
        const session: Session = {
            id: uuidv4(),
            name,
            device,
            ram: (await this.emulationService.cloudpilot).minRamForDevice(device) / 1024 / 1024,
        };

        SESSIONS.push(session);
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

    private sessions = SESSIONS;
}
