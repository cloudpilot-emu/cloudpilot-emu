import { Injectable, NgZone } from '@angular/core';
import { EmulatorEventTarget, GenericEventHandlingService } from '@common/service/GenericEventHandlingService';
import { EventHandler } from '@common/service/GenericEventHandlingService';

import { CanvasDisplayService } from './canvas-display.service';
import { EmulationService } from './emulation.service';
import { ModalWatcherService } from './modal-watcher.service';

@Injectable({ providedIn: 'root' })
export class EventHandlingService extends GenericEventHandlingService {
    constructor(
        emulationService: EmulationService,
        canvasDisplayService: CanvasDisplayService,
        private modalWatcher: ModalWatcherService,
        private ngZone: NgZone,
    ) {
        super(emulationService, canvasDisplayService);
    }

    protected override enableKeyboardEvents(): boolean {
        return !this.modalWatcher.isModalActive();
    }

    protected override onToggleGameMode(): void {
        this.ngZone.run(() => undefined);
    }

    protected override addEventListener<K extends keyof HTMLElementEventMap, E extends EmulatorEventTarget>(
        elt: E,
        type: K,
        handler: EventHandler<K>,
        caputure?: boolean,
    ) {
        this.ngZone.runOutsideAngular(() => elt.addEventListener(type, handler, caputure));
    }
}
