import { Injectable } from '@angular/core';
import { Loader, LoaderAdapter } from '@common/helper/Loader';
import { LoadingController } from '@ionic/angular';

class Adapter implements LoaderAdapter {
    constructor(loadingController: LoadingController) {
        this.loader = loadingController.create();
    }

    async show(message: '') {
        const loader = await this.loader;
        loader.message = message;

        await loader.present();
    }

    async hide(): Promise<void> {
        const loader = await this.loader;

        await loader.dismiss();
    }

    private loader: Promise<HTMLIonLoadingElement>;
}

@Injectable({ providedIn: 'root' })
export class LoaderService extends Loader {
    constructor(loadingController: LoadingController) {
        super(new Adapter(loadingController), 50);
    }
}
