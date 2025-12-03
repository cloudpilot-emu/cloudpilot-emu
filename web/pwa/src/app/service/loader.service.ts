import { Injectable } from '@angular/core';
import { Loader, LoaderAdapter } from '@common/helper/Loader';
import { LoadingController } from '@ionic/angular';

export interface LoaderOptions {
    onTop?: boolean;
}

class Adapter implements LoaderAdapter<LoaderOptions> {
    constructor(private loadingController: LoadingController) {}

    async show(message?: string, { onTop } = {} as LoaderOptions) {
        if (this.loader) {
            this.loader.message = message;
        } else {
            this.loader = await this.loadingController.create({ message });
            await this.loader.present();
        }

        this.loader.style.zIndex = onTop ? '10000000' : '';
    }

    async hide(): Promise<void> {
        if (!this.loader) return;

        await this.loader.dismiss();
        this.loader = undefined;
    }

    private loader: HTMLIonLoadingElement | undefined;
}

@Injectable({ providedIn: 'root' })
export class LoaderService extends Loader<LoaderOptions> {
    constructor(loadingController: LoadingController) {
        super(new Adapter(loadingController), 50);
    }
}
