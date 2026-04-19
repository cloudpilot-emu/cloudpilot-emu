import { Injectable } from '@angular/core';

import { InstallationMode } from '@pwa/model/InstallationMode';

import { AudioService } from './audio.service';
import { KvsService } from './kvs.service';
import { PwaService } from './pwa.service';

const EVENTS = ['touchstart', 'click', 'keydown'];

/*
 * This really should be an initializer, but cannot as it causes a DI
 * cycle.
 */

@Injectable({ providedIn: 'root' })
export class AutoEnableAudioService {
    constructor(
        private kvsService: KvsService,
        private audioService: AudioService,
        private pwaService: PwaService,
    ) {
        void this.initialize();
    }

    private async initialize() {
        await this.kvsService.initialize();
        if (!this.kvsService.kvs.enableAudioOnFirstInteraction) return;

        if (this.pwaService.determineInstallationMode() === InstallationMode.app) {
            void this.audioService.initialize();
            return;
        }

        const handler = () => {
            EVENTS.forEach((evt) => window.removeEventListener(evt, handler, true));
            void this.audioService.initialize();
        };

        EVENTS.forEach((evt) => window.addEventListener(evt, handler, true));
    }
}
