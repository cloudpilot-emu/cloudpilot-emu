import { FileService } from './file.service';

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
import { FsToolsService } from './fstools.service';
import { ZipfileWalkerState } from '@native/cloudpilot_web';
import { AlertService } from './alert.service';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { metadataForSession } from '@pwa/helper/metadata';
import { filenameForSession, filenameForSessions } from '@pwa/helper/filename';

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(
        private cloudpilotService: CloudpilotService,
        private storageService: StorageService,
        private loadingController: LoadingController,
        private fsTools: FsToolsService,
        private alertService: AlertService,
        private fileService: FileService,
    ) {
        void this.updateSessionsFromStorage().then(() => (this.loading = false));

        this.storageService.sessionChangeEvent.addHandler(this.updateSessionsFromStorage.bind(this));
    }

    isLoading(): boolean {
        return this.loading;
    }

    async addSessionFromImage(
        image: SessionImage<SessionMetadata>,
        name: string,
        presets: Partial<Session> = {},
    ): Promise<Session> {
        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        try {
            return this.doAddSessionFromImage(image, name, presets);
        } finally {
            await loader.dismiss();
        }
    }

    async addSessionsFromArchive(zipData: Uint8Array): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        try {
            const cloudpilot = await this.cloudpilotService.cloudpilot;

            const zipfileWalker = await this.fsTools.createZipfileWalker(zipData);
            if (zipfileWalker.GetState() === ZipfileWalkerState.error) {
                await this.alertService.errorMessage('Import failed: unable to read zip file.');
                return;
            }

            const entriesTotal = zipfileWalker.GetTotalEntries();
            const names = new Set(this.sessions.map((session) => session.name));
            let iEntry = 1;

            while (zipfileWalker.GetState() === ZipfileWalkerState.open) {
                loader.message = `Importing ${iEntry++}/${entriesTotal}...`;

                const sessionImage = /\.(bin|img)$/.test(zipfileWalker.GetCurrentEntryName())
                    ? cloudpilot.deserializeSessionImage<SessionMetadata>(zipfileWalker.GetCurrentEntryContent())
                    : undefined;

                if (!sessionImage) {
                    await this.alertService.errorMessage(
                        `${zipfileWalker.GetCurrentEntryName()} is not a valid session.`,
                        'Continue',
                    );

                    zipfileWalker.Next();
                    continue;
                }

                const sessionName = disambiguateName(
                    sessionImage.metadata?.name ?? zipfileWalker.GetCurrentEntryName(),
                    (name) => names.has(name),
                );

                await this.doAddSessionFromImage(sessionImage, sessionName, sessionImage.metadata);

                zipfileWalker.Next();
            }
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
        } catch (e: any) {
            await loader.dismiss();
            await this.alertService.errorMessage(`Failed to import sessions: ${e?.message ?? 'unknown error'}.`);
        } finally {
            void loader.dismiss();
        }
    }

    async addSessionFromRom(
        rom: Uint8Array,
        name: string,
        device: DeviceId,
        presets: Partial<Session> = {},
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

    async saveSession(session: Session): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Exporting...' });
        await loader.present();

        try {
            const image = await this.serializeSession(session);

            if (image) {
                this.fileService.saveFile(filenameForSession(session), image);
            } else {
                await loader.dismiss();
                void this.alertService.errorMessage('Failed to save session.');
            }
        } finally {
            void loader.dismiss();
        }
    }

    async saveSessions(sessions: Array<Session>): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Exporting...' });
        await loader.present();

        const createZipContext = await this.fsTools.createZipContext(0);
        const sessionNames = new Set<string>();

        try {
            let i = 1;

            for (const session of sessions) {
                loader.message = `Exporting ${i++}/${sessions.length}...`;

                const image = await this.serializeSession(session);

                if (image) {
                    const sessionName = disambiguateName(
                        session.name.replace(/[\/\\]/, '_').substring(0, 200),
                        (name) => sessionNames.has(name),
                    );

                    sessionNames.add(sessionName);
                    await createZipContext.addEntry(sessionName + '.img', image);
                } else {
                    await this.alertService.errorMessage(`Failed  to export session '${session.name}'.`, 'Continue');
                }
            }

            this.fileService.saveFile(filenameForSessions(), createZipContext.getContent());
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
        } catch (e: any) {
            void this.alertService.errorMessage(`Failed to export sessions: ${e.message ?? 'unknown error'}`);
        } finally {
            void loader.dismiss();
        }
    }

    async emergencySaveSession(session: Session): Promise<void> {
        const cloudpilot = await this.cloudpilotService.cloudpilot;
        const loader = await this.loadingController.create({ message: 'Exporting...' });

        // This code path is usually triggered from a dialog, so make sure that the loader is on top.
        // This is a hack, but sufficient for this edge case.
        loader.style.zIndex = '10000000';

        await loader.present();

        try {
            const rom = cloudpilot.getRomImage().slice();
            const memory = cloudpilot.getMemory().slice();
            const savestate = cloudpilot.saveState() ? cloudpilot.getSavestate().slice() : undefined;
            const framebufferSize = cloudpilot.framebufferSizeForDevice(session.device);

            if (framebufferSize < 0) {
                throw new Error(`invalid device ID ${session.device}`);
            }

            const sessionImage: Omit<SessionImage<SessionMetadata>, 'version'> = {
                deviceId: session.device,
                metadata: metadataForSession(session),
                rom,
                memory,
                savestate,
            };

            const image = (await this.cloudpilotService.cloudpilot).serializeSessionImage(sessionImage);
            if (image) {
                this.fileService.saveFile(filenameForSession(session), image);
            }
            // Showing an error alert may conflict with the alert that is already visible.
            // However, the error case is only possible if allocations fail in WASM --- an
            // extreme edge case.
        } finally {
            void loader.dismiss();
        }
    }

    private async serializeSession(session: Session): Promise<Uint8Array | undefined> {
        const [rom, memory, savestate] = await this.storageService.loadSession(session, false);

        if (!rom) {
            throw new Error(`invalid ROM ${session.rom}`);
        }

        const sessionImage: Omit<SessionImage<SessionMetadata>, 'version'> = {
            deviceId: session.device,
            metadata: metadataForSession(session),
            rom,
            memory,
            savestate,
        };

        return (await this.cloudpilotService.cloudpilot).serializeSessionImage(sessionImage);
    }

    private async updateSessionsFromStorage(): Promise<void> {
        this.sessions = await this.updateMutex.runExclusive(async () =>
            (await this.storageService.getAllSessions()).sort((x, y) => x.name.localeCompare(y.name)),
        );
    }

    private async doAddSessionFromImage(
        image: SessionImage<SessionMetadata>,
        name: string,
        presets: Partial<Session> = {},
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

        const savedSession = await this.storageService.addSession(session, image.rom, image.memory, image.savestate);

        await this.updateSessionsFromStorage();

        return savedSession;
    }

    private sessions: Array<Session> = [];
    private loading = true;
    private updateMutex = new Mutex();
}
