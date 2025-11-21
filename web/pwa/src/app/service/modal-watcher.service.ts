import { Injectable, NgZone } from '@angular/core';
import { Event } from 'microevent.ts';

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

    enable(): void {
        if (this.enabled) return;

        this.enabled = true;
        if (this.modalActive) this.modalVisibilityChangeEvent.dispatch(true);
    }

    private update(): void {
        const modalActive = document.querySelectorAll('ion-alert, ion-modal').length > 0;

        if (modalActive !== this.modalActive) {
            this.zone.run(() => {
                this.modalActive = modalActive;
                if (this.enabled) this.modalVisibilityChangeEvent.dispatch(modalActive);
            });
        }
    }

    modalVisibilityChangeEvent = new Event<boolean>();

    private modalActive = false;
    private enabled = false;
    private observer!: MutationObserver;
}
