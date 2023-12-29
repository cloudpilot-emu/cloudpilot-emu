import { ActionSheetController, AlertController, LoadingController, ModalController } from '@ionic/angular';
import { SessionImage } from '@common/bridge/Cloudpilot';

import { AlertService } from './alert.service';
import { CloudpilotService } from './cloudpilot.service';
import { FetchService } from './fetch.service';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { RemoteUrlPromptComponent } from '@pwa/component/remote-url-prompt/remote-url-prompt.component';
import { Session } from '@pwa/model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { StorageService } from './storage.service';
import Url from 'url-parse';
import { filenameForSession, filenameForSessions } from '@pwa/helper/filename';
import { metadataForSession } from '@pwa/helper/metadata';
import { isIOS, isIOSSafari, isSafari } from '@common/helper/browser';
import { FsToolsService } from './fstools.service';

/* eslint-disable no-bitwise */

export interface FileDescriptor {
    name: string;
    getContent: () => Promise<Uint8Array>;
}

@Injectable({
    providedIn: 'root',
})
export class FileService {
    constructor(
        private storageService: StorageService,
        private loadingController: LoadingController,
        private actionSheetController: ActionSheetController,
        private kvsService: KvsService,
        private modalController: ModalController,
        private alertService: AlertService,
        private fetchService: FetchService,
        private cloudpilotService: CloudpilotService,
        private fstoolsService: FsToolsService,
        private alertController: AlertController,
    ) {}

    openFile(handler: (file: FileDescriptor) => void): void {
        void this.openFilesImpl(false, (files) => {
            if (files.length > 0) handler(files[0]);
        });
    }

    openFiles(handler: (files: Array<FileDescriptor>) => void): void {
        void this.openFilesImpl(true, handler);
    }

    openFromDrop(e: DragEvent, handler: (files: Array<FileDescriptor>) => void): void {
        if (e.type !== 'drop' || !e.dataTransfer?.files) return;

        handler(Array.from(e.dataTransfer.files).map(this.readFile.bind(this)));
    }

    async saveSession(session: Session): Promise<void> {
        const loader = await this.loadingController.create({ message: 'Exporting...' });
        await loader.present();

        try {
            const image = await this.serializeSession(session);

            if (image) {
                this.saveFile(filenameForSession(session), image);
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

        const createZipContext = await this.fstoolsService.createZipContext(0);
        const sessionNames = new Set<string>();

        try {
            let i = 1;

            for (const session of sessions) {
                loader.message = `Exporting ${i++}/${sessions.length}...`;

                const image = await this.serializeSession(session);

                if (image) {
                    const sessionNameBase = session.name.replace(/[\/\\]/, '_').substring(0, 200);

                    let sessionName = sessionNameBase;
                    for (let i = 0; sessionNames.has(sessionName); sessionName = `${sessionNameBase} (${i})`) {}

                    sessionNames.add(sessionName);
                    await createZipContext.addEntry(sessionName + '.img', image);
                } else {
                    const alert = await this.alertController.create({
                        header: 'Error',
                        backdropDismiss: false,
                        message: `Failed  to export session '${session.name}'.`,
                        buttons: [{ text: 'Continue', role: 'cancel' }],
                        cssClass: 'alert-error',
                    });

                    await alert.present();
                    await alert.onDidDismiss();
                }
            }

            this.saveFile(filenameForSessions(), createZipContext.getContent());
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
                this.saveFile(filenameForSession(session), image);
            }
            // Showing an error alert may conflict with the alert that is already visible.
            // However, the error case is only possible if allocations fail in WASM --- an
            // extreme edge case.
        } finally {
            void loader.dismiss();
        }
    }

    saveFile(name: string, content: Uint8Array): void {
        const file = new Blob([content], { type: 'application/octet-stream' });
        const url = URL.createObjectURL(file);

        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.click();
    }

    async openUrl(url: string, handler: (file: FileDescriptor) => void): Promise<void> {
        let urlParsed!: Url<unknown>;
        try {
            urlParsed = new Url(url);
        } catch (e) {
            await this.alertService.errorMessage(`Invalid URL: ${url}`);

            return;
        }

        try {
            const response = await this.fetchService.fetch(url);

            if (!response.ok) {
                throw new Error('request failed');
            }

            const loader = await this.loadingController.create({ message: 'Loading...' });
            void loader.present();

            const contentPromise = response.arrayBuffer().then((buffer) => new Uint8Array(buffer));
            void contentPromise.finally(() => loader.dismiss());

            handler({
                name: urlParsed.pathname.replace(/.*\//, ''),
                getContent: () => contentPromise,
            });

            return;
        } catch (e) {
            await this.alertService.errorMessage(`Download from ${url} failed.`);
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

    private async openFilesImpl(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): Promise<void> {
        if (this.kvsService.kvs.enableRemoteInstall) {
            const sheet = await this.actionSheetController.create({
                header: 'What source do you want to open?',
                buttons: [
                    {
                        text: 'Local files',
                        handler: () => this.openFilesLocal(multiple, handler),
                    },
                    {
                        text: 'File on remote server',
                        handler: () => this.queryAndOpenFileRemote(handler),
                    },
                    { text: 'Cancel' },
                ],
            });

            void sheet.present();
        } else {
            this.openFilesLocal(multiple, handler);
        }
    }

    private async queryAndOpenFileRemote(handler: (files: Array<FileDescriptor>) => void): Promise<void> {
        let url!: string;

        try {
            url = await new Promise<string>((resolve, reject) =>
                this.modalController
                    .create({
                        component: RemoteUrlPromptComponent,
                        backdropDismiss: false,
                        componentProps: {
                            onContinue: resolve,
                            onCancel: reject,
                        },
                    })
                    .then((modal) => modal.present()),
            );
        } catch (e) {
            return;
        } finally {
            void this.modalController.dismiss();
        }

        await this.openUrl(url, (file) => handler([file]));
    }

    private openFilesLocal(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): void {
        const append = isSafari || (isIOS && isIOSSafari);
        if (append && this.input) document.body.removeChild(this.input);

        this.input = document.createElement('input');

        this.input.multiple = multiple;
        this.input.type = 'file';

        this.input.addEventListener('change', async (e) => {
            const target = e.target as HTMLInputElement;

            if (!target?.files) return;
            handler(Array.from(target.files).map(this.readFile.bind(this)));
        });

        if (append) {
            this.input.style.display = 'none';
            document.body.appendChild(this.input);
        }

        this.input.click();
    }

    private readFile(file: File): FileDescriptor {
        let contentPromise: Promise<Uint8Array> | undefined;
        const content = async (): Promise<Uint8Array> => {
            if (contentPromise) return contentPromise;

            contentPromise = new Promise((resolve, reject) => {
                const reader = new FileReader();

                reader.onload = () => resolve(new Uint8Array(reader.result as ArrayBuffer));
                reader.onerror = () => {
                    console.warn(reader.error);
                    reject(reader.error);
                };

                reader.readAsArrayBuffer(file);
            });

            return contentPromise;
        };

        return { name: file.name, getContent: content };
    }

    private input: HTMLInputElement | undefined;
}
