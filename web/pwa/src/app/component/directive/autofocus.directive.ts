import { Directive, ElementRef, NgZone, OnDestroy } from '@angular/core';

@Directive({ selector: '[appAutofocus]' })
export class AutofocusDirective implements OnDestroy {
    constructor(
        private el: ElementRef,
        private zone: NgZone,
    ) {
        const input = (el.nativeElement as HTMLElement).querySelector('input');
        if (input) {
            input.focus();
            return;
        }

        this.zone.runOutsideAngular(() => {
            this.observer = new MutationObserver(this.update.bind(this));

            this.observer.observe(document.body, {
                subtree: true,
                childList: true,
            });
        });
    }

    ngOnDestroy(): void {
        this.observer?.disconnect();
    }

    private update(): void {
        const input = (this.el.nativeElement as HTMLElement).querySelector('input');
        if (!input) return;

        setTimeout(() => input.focus(), 100);
        this.observer?.disconnect();
    }

    private observer: MutationObserver | undefined;
}
