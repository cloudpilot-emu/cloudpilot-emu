import {} from './file.service';

import { CloudpilotService } from './cloudpilot.service';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';
import { Session } from '@pwa/model/Session';
import { SessionImage } from '@common/bridge/Cloudpilot';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { StorageService } from './storage.service';

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(
        private cloudpilotService: CloudpilotService,
        private storageService: StorageService,
        private loadingController: LoadingController
    ) {
        this.updateSessionsFromStorage().then(() => (this.loading = false));

        this.storageService.sessionChangeEvent.addHandler(this.updateSessionsFromStorage.bind(this));
    }

    isLoading(): boolean {
        return this.loading;
    }

    async addSessionFromImage(
        image: SessionImage<SessionMetadata>,
        name: string,
        presets: Partial<Session> = {}
    ): Promise<Session> {
        const session: Session = {
            hotsyncName: image.metadata?.hotsyncName,
            dontManageHotsyncName: false,
            speed: 1,
            deviceOrientation: DeviceOrientation.portrait,
            ...presets,
            id: -1,
            name,
            device: image.deviceId,
            ram: (await this.cloudpilotService.cloudpilot).minRamForDevice(image.deviceId) / 1024 / 1024,
            rom: '',
            osVersion: image?.metadata?.osVersion,
        };

        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        const savedSession = await this.storageService.addSession(session, image.rom, image.memory, image.savestate);

        await this.updateSessionsFromStorage();
        await loader.dismiss();

        return savedSession;
    }

    async addSessionFromRom(
        rom: Uint8Array,
        name: string,
        device: DeviceId,
        presets: Partial<Session> = {}
    ): Promise<Session> {
        const session: Session = {
            hotsyncName: '',
            dontManageHotsyncName: false,
            speed: 1,
            deviceOrientation: DeviceOrientation.portrait,
            ...presets,
            id: -1,
            name,
            device,
            ram: (await this.cloudpilotService.cloudpilot).minRamForDevice(device) / 1024 / 1024,
            rom: '',
        };

        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        const savedSession = await this.storageService.addSession(session, rom);

        await this.updateSessionsFromStorage();
        await loader.dismiss();

        return savedSession;
    }

    getSessions(): Array<Session> {
        return this.sessions;
    }

    async deleteSession(session: Session): Promise<void> {
        await this.storageService.deleteSession(session);
    }

    async updateSession(session: Session): Promise<void> {
        await this.storageService.updateSession(session);
    }

    private async updateSessionsFromStorage(): Promise<void> {
        this.sessions = await this.updateMutex.runExclusive(async () =>
            (await this.storageService.getAllSessions()).sort((x, y) => x.name.localeCompare(y.name))
        );
    }

    private sessions: Array<Session> = [];
    private loading = true;
    private updateMutex = new Mutex();
}
