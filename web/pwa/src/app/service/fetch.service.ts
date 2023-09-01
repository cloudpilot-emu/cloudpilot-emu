import { LoadingController, ModalController } from '@ionic/angular';

import { CredentialsPromptComponent } from '@pwa/component/credentials-prompt/credentials-prompt.component';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import Url from 'url-parse';

const DEFAULT_TIMEOUT = 5000;

interface ExtendedRequestInit extends RequestInit {
    timeout?: number;
    loaderDelay?: number;
}

@Injectable({ providedIn: 'root' })
export class FetchService {
    constructor(
        private loadingController: LoadingController,
        private kvsService: KvsService,
        private modalController: ModalController,
    ) {}

    async fetch(input: RequestInfo, init?: ExtendedRequestInit): Promise<Response> {
        let loaderDelay = init?.loaderDelay;
        const timeout = init?.timeout === undefined ? DEFAULT_TIMEOUT : init?.timeout;
        const urlParsed = new Url(typeof input === 'string' ? input : input.url);

        let loader: HTMLIonLoadingElement | undefined;
        let loaderTimeout: number | undefined;
        let authorizationRequired = false;

        while (true) {
            try {
                if (!loader) loader = await this.loadingController.create({ message: 'Loading...' });

                if (loaderDelay !== undefined && loaderDelay > 0 && loaderTimeout === undefined) {
                    loaderTimeout = window.setTimeout(() => loader?.present(), loaderDelay);
                } else {
                    void loader.present();
                }

                const auth = this.kvsService.kvs.credentials[urlParsed.origin];

                const abortContrtoller = new AbortController();
                const fetchTimeout = window.setTimeout(() => abortContrtoller.abort(), timeout);

                const response = await fetch(input, {
                    ...this.requestInit(init),
                    signal: abortContrtoller.signal,
                    headers: {
                        ...(init?.headers || {}),
                        ...(authorizationRequired && auth
                            ? { Authorization: 'Basic ' + btoa(`${auth.username}:${auth.password}`) }
                            : {}),
                    },
                });

                clearTimeout(fetchTimeout);

                if (response.status === 401) {
                    if (!authorizationRequired && auth) {
                        authorizationRequired = true;
                        continue;
                    }

                    void loader?.dismiss();
                    clearTimeout(loaderTimeout);

                    loaderDelay = 0;
                    loader = undefined;
                    loaderDelay = undefined;

                    authorizationRequired = true;

                    if (await this.authenticate(urlParsed.origin)) continue;
                }

                return response;
            } finally {
                if (loader) await loader.dismiss();
                clearTimeout(loaderTimeout);
            }
        }
    }

    private requestInit(init?: ExtendedRequestInit): RequestInit | undefined {
        if (!init) return undefined;

        const { timeout, loaderDelay, ...rest } = init;

        return rest;
    }

    private async authenticate(origin: string): Promise<boolean> {
        const confirmed = await new Promise<boolean>((resolve) =>
            this.modalController
                .create({
                    component: CredentialsPromptComponent,
                    backdropDismiss: false,
                    componentProps: {
                        origin,
                        onContinue: () => resolve(true),
                        onCancel: () => resolve(false),
                    },
                })
                .then((modal) => modal.present()),
        );

        void this.modalController.dismiss();

        return confirmed;
    }
}
