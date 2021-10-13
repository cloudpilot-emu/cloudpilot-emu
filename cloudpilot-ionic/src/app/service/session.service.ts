import {} from './file.service';

import { DeviceId } from './../model/DeviceId';
import { EmulationService } from './emulation.service';
import { Injectable } from '@angular/core';
import { LoadingController } from '@ionic/angular';
import { Session } from '../model/Session';
import { SessionImage } from '../model/SessionImage';
import { StorageService } from './storage.service';

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(
        private emulationService: EmulationService,
        private storageService: StorageService,
        private loadingController: LoadingController
    ) {
        this.updateSessionsFromStorage().then(() => (this.loading = false));

        this.storageService.sessionChangeEvent.addHandler(this.updateSessionsFromStorage.bind(this));
    }

    isLoading(): boolean {
        return this.loading;
    }

    async addSessionFromImage(image: SessionImage, name: string, presets: Partial<Session> = {}): Promise<Session> {
        const framebufferSizeForDevice = (await this.emulationService.cloudpilot).framebufferSizeForDevice(
            image.deviceId
        );
        if (framebufferSizeForDevice < 0) {
            throw new Error(`invalid device ID ${image.deviceId}`);
        }

        const ram =
            (image.version >= 2 ? image.memory.length - framebufferSizeForDevice : image.memory.length) / 1024 / 1024;
        const totalMemory = image.version >= 2 ? image.memory.length : image.memory.length + framebufferSizeForDevice;

        const session: Session = {
            hotsyncName: image.metadata?.hotsyncName,
            dontManageHotsyncName: false,
            ...presets,
            id: -1,
            name,
            device: image.deviceId,
            ram,
            totalMemory,
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
            ...presets,
            id: -1,
            name,
            device,
            ram: (await this.emulationService.cloudpilot).minRamForDevice(device) / 1024 / 1024,
            totalMemory: (await this.emulationService.cloudpilot).totalMemorySizeForDevice(device),
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
        this.sessions = (await this.storageService.getAllSessions()).sort((x, y) => x.name.localeCompare(y.name));
    }

    private sessions: Array<Session> = [];
    private loading = true;
}
