import { Injectable } from '@angular/core';
import { AbstractAudioService } from '@common/service/AbstractAudioService';

import { EmulationService } from './emulation.service';
import { FeatureService } from './feature.service';
import { KvsService } from './kvs.service';
import { ModalWatcherService } from './modal-watcher.service';

@Injectable({ providedIn: 'root' })
export class AudioService extends AbstractAudioService {
    constructor(
        emulationService: EmulationService,
        private kvs: KvsService,
        private modalWatcher: ModalWatcherService,
        private featureService: FeatureService,
    ) {
        super(emulationService);

        this.modalWatcher.modalVisibilityChangeEvent.addHandler(() => this.updateState());
    }

    protected getVolume(): number {
        return this.kvs.kvs.volume;
    }

    protected runHidden(): boolean {
        return this.kvs.kvs.runHidden && this.featureService.runHidden;
    }

    protected override shouldRun(): boolean {
        if (this.modalWatcher.isModalActive()) return false;

        return super.shouldRun();
    }
}
