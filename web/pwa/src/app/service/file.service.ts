import { Injectable } from '@angular/core';
import { isIOS, isIOSSafari, isMacOSSafari } from '@common/helper/browser';
import { ActionSheetController, ModalController } from '@ionic/angular';
import Url from 'url-parse';

import { RemoteUrlPromptComponent } from '@pwa/component/remote-url-prompt/remote-url-prompt.component';

import { AlertService } from './alert.service';
import { FetchService } from './fetch.service';
import { KvsService } from './kvs.service';
import { LoaderOptions, LoaderService } from './loader.service';

const CONTENT_LOADER_DELAY = 100;

export interface FileDescriptor {
    name: string;
    getContent: (loaderOptions?: LoaderOptions) => Promise<Uint8Array>;
}

@Injectable({
    providedIn: 'root',
})
export class FileService {
    constructor(
        private actionSheetController: ActionSheetController,
        private kvsService: KvsService,
        private modalController: ModalController,
        private alertService: AlertService,
        private fetchService: FetchService,
        private loaderService: LoaderService,
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

    saveFile(name: string, content: Uint8Array, type = 'application/octet-stream'): void {
        this.saveBlob(name, new Blob([content], { type }));
    }

    saveBlob(name: string, content: Blob): void {
        const url = URL.createObjectURL(content);

        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.click();
    }

    async openUrl(url: string, handler: (file: FileDescriptor) => void): Promise<void> {
        let urlParsed!: Url<unknown>;
        try {
            urlParsed = new Url(url);
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
        } catch (e) {
            await this.alertService.errorMessage(`Invalid URL: ${url}`);

            return;
        }

        let response: Response;

        try {
            response = await this.fetchService.fetch(url, { loaderDelay: -1 });

            if (!response.ok) {
                throw new Error('request failed');
            }
        } catch (e) {
            console.log(e);
            await this.alertService.errorMessage(`Download from ${url} failed.`);

            return;
        }

        const contentPromise = response.arrayBuffer().then((buffer) => new Uint8Array(buffer));

        handler({
            name: urlParsed.pathname.replace(/.*\//, ''),
            getContent: (loaderOptions?: LoaderOptions) =>
                this.loaderService.showWhile(() => contentPromise, 'Loading...', CONTENT_LOADER_DELAY, loaderOptions),
        });

        return;
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
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
        } catch (e) {
            return;
        } finally {
            void this.modalController.dismiss();
        }

        await this.openUrl(url, (file) => handler([file]));
    }

    private openFilesLocal(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): void {
        const append = isMacOSSafari || isIOSSafari;
        if (append && this.input) document.body.removeChild(this.input);

        this.input = document.createElement('input');

        this.input.multiple = multiple;
        this.input.type = 'file';
        if (isIOS) this.input.accept = 'application/octet-stream';

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

        const content = (loaderOptions?: LoaderOptions): Promise<Uint8Array> =>
            this.loaderService.showWhile(
                () => {
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
                },
                'Loading...',
                CONTENT_LOADER_DELAY,
                loaderOptions,
            );

        return { name: file.name, getContent: content };
    }

    private input: HTMLInputElement | undefined;
}
