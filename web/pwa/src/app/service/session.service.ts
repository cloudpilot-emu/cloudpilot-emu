import { Injectable, Signal, signal } from '@angular/core';
import { SessionImage } from '@common/bridge/Cloudpilot';
import { engineType, nandSize } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { LoadingController } from '@ionic/angular';
import { ZipfileWalkerState } from '@native/cloudpilot_web';
import { Mutex } from 'async-mutex';

import { disambiguateName } from '@pwa/helper/disambiguate';
import { filenameForSession, filenameForSessions } from '@pwa/helper/filename';
import { metadataForSession } from '@pwa/helper/metadata';
import { Session, SessionSettings, settingsFromMetadata } from '@pwa/model/Session';

import { AlertService } from './alert.service';
import { FileService } from './file.service';
import { FsToolsService } from './fstools.service';
import { NativeSupportService } from './native-support.service';
import { StorageService } from './storage.service';

class ImportError extends Error {
    constructor(message: string) {
        super(message);
    }
}

@Injectable({
    providedIn: 'root',
})
export class SessionService {
    constructor(
        private nativeSupportService: NativeSupportService,
        private storageService: StorageService,
        private loadingController: LoadingController,
        private fsTools: FsToolsService,
        private alertService: AlertService,
        private fileService: FileService,
    ) {
        void this.updateSessionsFromStorage().then(() => this._loading.set(false));

        this.storageService.sessionChangeEvent.addHandler(this.updateSessionsFromStorage.bind(this));
    }

    get loading(): Signal<boolean> {
        return this._loading;
    }

    async addSessionFromImage(image: SessionImage<SessionMetadata>, settings: SessionSettings): Promise<Session> {
        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        try {
            return await this.doAddSessionFromImage(image, settings);
        } finally {
            await loader.dismiss();
        }
    }

    async addSessionsFromArchive(zipData: Uint8Array): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        try {
            const zipfileWalker = await this.fsTools.createZipfileWalker(zipData);
            if (zipfileWalker.GetState() === ZipfileWalkerState.error) {
                await this.alertService.errorMessage('Import failed: unable to read zip file.');
                return;
            }

            const entriesTotal = zipfileWalker.GetTotalEntries();
            const names = new Set(this._sessions().map((session) => session.name));
            let iEntry = 1;
            let skipErrors = false;
            const failures: Array<string> = [];

            while (zipfileWalker.GetState() === ZipfileWalkerState.open) {
                try {
                    loader.message = `Importing ${iEntry++}/${entriesTotal}...`;

                    const sessionImage = /\.(bin|img)$/i.test(zipfileWalker.GetCurrentEntryName())
                        ? await this.nativeSupportService.deserializeSessionImage<SessionMetadata>(
                              zipfileWalker.GetCurrentEntryContent(),
                          )
                        : undefined;

                    if (!sessionImage) {
                        throw new ImportError(`${zipfileWalker.GetCurrentEntryName()} is not a valid session.`);
                    }

                    const sessionName = disambiguateName(
                        sessionImage.metadata?.name ?? zipfileWalker.GetCurrentEntryName(),
                        (name) => names.has(name),
                    );

                    await this.doAddSessionFromImage(sessionImage, {
                        ...settingsFromMetadata(sessionImage.engine, sessionImage.metadata),
                        name: sessionName,
                    });
                } catch (e) {
                    if (!(e instanceof ImportError)) throw e;

                    failures.push(zipfileWalker.GetCurrentEntryName());
                    if (!skipErrors) {
                        skipErrors = await this.alertService.messageWithChoice(
                            'Error',
                            e.message,
                            'Skip further errors',
                            skipErrors,
                            {},
                            'Continue',
                        );
                    }
                } finally {
                    zipfileWalker.Next();
                }
            }

            if (failures.length <= 3 && failures.length > 0) {
                await this.alertService.errorMessage(
                    `The following files could not be imported: <br><br>${failures.join('<br>')}`,
                );
            } else if (failures.length > 3) {
                await this.alertService.errorMessage(
                    `
                        The following files could not be imported: <br><br>${failures.slice(0, 3).join('<br>')}
                        <br><br>
                        and ${failures.length - 3} more files.
                    `,
                );
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
        device: DeviceId,
        settings: SessionSettings,
        nand?: Uint8Array,
    ): Promise<Session> {
        const eng = engineType(device);

        if (eng !== settings.engine) throw new Error('settings do not match engine');
        if (nand && nand?.length !== nandSize(device)) throw new Error('NAND size mismatch');

        const session: Session = {
            ...settings,
            id: -1,
            device,
            ram: (await this.nativeSupportService.ramSizeForDevice(device, rom)) >>> 20,
            rom: '',
            wasResetForcefully: false,
            nand: nandSize(device),
        };

        const loader = await this.loadingController.create({ message: 'Importing...' });
        await loader.present();

        try {
            const savedSession = await this.storageService.addSession(session, { rom, nand });
            await this.updateSessionsFromStorage();

            return savedSession;
        } finally {
            void loader.dismiss();
        }
    }

    get sessions(): Signal<Array<Session>> {
        return this._sessions;
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
                    const sessionName = disambiguateName(session.name.replace(/[/\\]/, '_').substring(0, 200), (name) =>
                        sessionNames.has(name),
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

    async emergencySaveSession(
        session: Session,
        rom: Uint8Array,
        memory: Uint8Array,
        nand?: Uint8Array,
        savestate?: Uint8Array,
    ): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Exporting...' });

        // This code path is usually triggered from a dialog, so make sure that the loader is on top.
        // This is a hack, but sufficient for this edge case.
        loader.style.zIndex = '10000000';

        await loader.present();

        try {
            const sessionImage: Omit<SessionImage<SessionMetadata>, 'version'> = {
                engine: session.engine,
                deviceId: session.device,
                metadata: metadataForSession(session),
                rom,
                memory,
                nand,
                savestate,
            };

            const image = await this.nativeSupportService.serializeSessionImage(sessionImage);
            if (image) {
                this.fileService.saveFile(filenameForSession(session), image);
            }
        } finally {
            void loader.dismiss();
        }
    }

    private async serializeSession(session: Session): Promise<Uint8Array | undefined> {
        const { rom, memory, savestate, nand } = await this.storageService.loadSession(session, false);

        if (!rom) {
            throw new Error(`invalid ROM ${session.rom}`);
        }

        const sessionImage: Omit<SessionImage<SessionMetadata>, 'version'> = {
            engine: session.engine,
            deviceId: session.device,
            metadata: metadataForSession(session),
            rom,
            memory,
            savestate,
            nand,
        };

        return this.nativeSupportService.serializeSessionImage(sessionImage);
    }

    private async updateSessionsFromStorage(): Promise<void> {
        this._sessions.set(
            await this.updateMutex.runExclusive(async () =>
                (await this.storageService.getAllSessions()).sort((x, y) => x.name.localeCompare(y.name)),
            ),
        );
    }

    private async doAddSessionFromImage(
        image: SessionImage<SessionMetadata>,
        settings: SessionSettings,
    ): Promise<Session> {
        if (settings.engine !== image.engine) throw new Error('settings do not match session type');

        const session: Session = {
            ...settings,
            id: -1,
            device: image.deviceId,
            ram: await this.getRamSizeForSession(image),
            rom: '',
            osVersion: image?.metadata?.osVersion,
            wasResetForcefully: false,
        };

        const savedSession = await this.storageService.addSession(session, {
            rom: image.rom,
            memory: image.memory,
            state: image.savestate,
            nand: image.nand,
        });

        await this.updateSessionsFromStorage();

        return savedSession;
    }

    private async getRamSizeForSession(image: SessionImage<unknown>): Promise<number> {
        if (image.engine === 'uarm' && image.memory) {
            return image.memory?.length >= 32 << 20 ? 32 : 16;
        }

        return (await this.nativeSupportService.ramSizeForDevice(image.deviceId, image.rom)) >>> 20;
    }

    readonly _sessions = signal<Array<Session>>([]);

    private _loading = signal(true);
    private updateMutex = new Mutex();
}
