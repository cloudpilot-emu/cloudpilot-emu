import { Injectable } from '@angular/core';
import { isIOS } from '@common/helper/browser';
import { AbstractAudioService } from '@common/service/AbstractAudioService';

import { EmulationService } from './emulation.service';
import { FeatureService } from './feature.service';
import { KvsService } from './kvs.service';

@Injectable({ providedIn: 'root' })
export class AudioService extends AbstractAudioService {
    constructor(
        emulationService: EmulationService,
        private kvs: KvsService,
        private featureService: FeatureService,
    ) {
        super(emulationService);
    }

    activate(): void {
        this.activated = true;
        this.updateState();
    }

    suspend(): void {
        this.activated = false;
        this.updateState();
    }

    protected override shouldRun(): boolean {
        return super.shouldRun() && (this.activated || isIOS);
    }

    protected getVolume(): number {
        return this.kvs.kvs.volume;
    }

    protected runHidden(): boolean {
        return this.kvs.kvs.runHidden && this.featureService.runHidden;
    }

    private activated = false;
}
