import { AudioService } from './audio.service';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';

const EVENTS = ['touch', 'click', 'keydown'];

/*
 * This really should be an initializer, but cannot as it causes a DI
 * cycle.
 */

@Injectable({ providedIn: 'root' })
export class AutoEnableAudioService {
    constructor(
        private kvsService: KvsService,
        private audioService: AudioService,
    ) {
        void this.initialize();
    }

    private async initialize() {
        await this.kvsService.initialize();
        if (!this.kvsService.kvs.enableAudioOnFirstInteraction) return;

        const handler = () => {
            EVENTS.forEach((evt) => window.removeEventListener(evt, handler, true));
            void this.audioService.initialize();
        };

        EVENTS.forEach((evt) => window.addEventListener(evt, handler, true));
    }
}
