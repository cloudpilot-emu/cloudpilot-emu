import { ActionSheetController, LoadingController, ModalController } from '@ionic/angular';
import { metadataForSession, serializeSessionImage } from '../helper/sessionFile';

import { AlertService } from 'src/app/service/alert.service';
import { Cloudpilot } from '../helper/Cloudpilot';
import { FetchService } from './fetch.service';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { RemoteUrlPromptComponent } from './../component/remote-url-prompt/remote-url-prompt.component';
import { Session } from './../model/Session';
import { SessionImage } from './../model/SessionImage';
import { StorageService } from './storage.service';
import Url from 'url-parse';
import { filenameForSession } from '../helper/filename';

// tslint:disable: no-bitwise

export interface FileDescriptor {
    name: string;
    content: Uint8Array;
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
        private fetchService: FetchService
    ) {}

    openFile(handler: (file: FileDescriptor) => void): void {
        this.openFilesImpl(false, (files) => {
            if (files.length > 0) handler(files[0]);
        });
    }

    openFiles(handler: (files: Array<FileDescriptor>) => void): void {
        this.openFilesImpl(true, handler);
    }

    async saveSession(session: Session): Promise<void> {
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            const [rom, memory, savestate] = await this.storageService.loadSession(session);

            if (!rom) {
                throw new Error(`invalid ROM ${session.rom}`);
            }

            const sessionImage: Omit<SessionImage, 'version'> = {
                deviceId: session.device,
                metadata: metadataForSession(session),
                rom,
                memory,
                savestate,
                framebufferSize: session.totalMemory - session.ram * 1024 * 1024,
            };

            this.saveFile(filenameForSession(session), serializeSessionImage(sessionImage));
        } finally {
            loader.dismiss();
        }
    }

    emergencySaveSession(session: Session, cloudpilot: Cloudpilot): void {
        const rom = cloudpilot.getRomImage();
        const memory = cloudpilot.getMemory();
        const savestate = cloudpilot.saveState() ? cloudpilot.getSavestate() : undefined;
        const framebufferSize = cloudpilot.framebufferSizeForDevice(session.device);

        if (framebufferSize < 0) {
            throw new Error(`invalid device ID ${session.device}`);
        }

        const sessionImage: Omit<SessionImage, 'version'> = {
            deviceId: session.device,
            metadata: metadataForSession(session),
            rom,
            memory,
            savestate,
            framebufferSize,
        };

        this.saveFile(filenameForSession(session), serializeSessionImage(sessionImage));
    }

    saveFile(name: string, content: Uint8Array): void {
        const file = new Blob([content], { type: 'application/octet-stream' });
        const url = URL.createObjectURL(file);

        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.click();
    }

    async openUrl(url: string, handler: (files: Array<FileDescriptor>) => void): Promise<void> {
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

            handler([
                {
                    name: urlParsed.pathname.replace(/.*\//, ''),
                    content: new Uint8Array(await response.arrayBuffer()),
                },
            ]);

            return;
        } catch (e) {
            await this.alertService.errorMessage(`Download from ${url} failed.`);
        }
    }

    private async openFilesImpl(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): Promise<void> {
        if (this.kvsService.kvs.enableRemoteInstall) {
            const sheet = await this.actionSheetController.create({
                header: 'From where do you want to install?',
                buttons: [
                    {
                        text: 'Local files',
                        handler: () => this.openFilesLocal(multiple, handler),
                    },
                    {
                        text: 'Remote server',
                        handler: () => this.queryAndOpenFileRemote(handler),
                    },
                ],
            });

            sheet.present();
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
                    .then((modal) => modal.present())
            );
        } catch (e) {
            return;
        } finally {
            this.modalController.dismiss();
        }

        await this.openUrl(url, handler);
    }

    private openFilesLocal(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): void {
        if (this.input) {
            document.body.removeChild(this.input);
        }

        this.input = document.createElement('input');

        this.input.style.display = 'none';
        this.input.multiple = multiple;
        this.input.type = 'file';

        this.input.addEventListener('change', async (e) => {
            const target = e.target as HTMLInputElement;

            if (!target?.files?.length) return [];

            const result: Array<Promise<FileDescriptor>> = [];

            for (let i = 0; i < target.files?.length; i++) {
                const file = target.files.item(i);

                if (!file) continue;

                const content = new Promise<FileDescriptor>((resolve, reject) => {
                    const reader = new FileReader();

                    reader.onload = () =>
                        resolve({ content: new Uint8Array(reader.result as ArrayBuffer), name: file.name });
                    reader.onerror = () => reject(reader.error);

                    reader.readAsArrayBuffer(file);
                });

                result.push(content);
            }

            handler(await Promise.all(result));
        });

        document.body.appendChild(this.input);

        this.input.click();
    }

    private input: HTMLInputElement | undefined;
}
