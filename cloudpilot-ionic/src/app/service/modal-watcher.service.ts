import { Injectable, NgZone } from '@angular/core';

import { Event } from 'microevent.ts';
import { ModalController } from '@ionic/angular';

@Injectable({
    providedIn: 'root',
})
export class ModalWatcherService {
    constructor(private zone: NgZone) {
        this.update();

        this.zone.runOutsideAngular(() => {
            this.observer = new MutationObserver(this.update.bind(this));

            this.observer.observe(document.body, {
                subtree: true,
                childList: true,
            });
        });
    }

    isModalActive(): boolean {
        return this.modalActive;
    }

    private update(): void {
        const modalActive = document.querySelectorAll('ion-alert, ion-modal').length > 0;

        if (modalActive !== this.modalActive) {
            this.zone.run(() => {
                this.modalActive = modalActive;
                this.modalVisibilityChangeEvent.dispatch(modalActive);
            });
        }
    }

    modalVisibilityChangeEvent = new Event<boolean>();

    private modalActive = false;
    private observer!: MutationObserver;
}
