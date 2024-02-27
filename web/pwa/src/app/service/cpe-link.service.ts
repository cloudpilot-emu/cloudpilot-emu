import { Injectable } from '@angular/core';
import { Event } from 'microevent.ts';

@Injectable({ providedIn: 'root' })
export class CpeLinkService {
    handleClick(e: MouseEvent): void {
        const target = e.target as HTMLAnchorElement;
        if (!target) return;

        const href = target.href;
        if (!href) return;

        if (href.indexOf('cpe://') !== 0) return;

        this.clickEvent.dispatch(href.substring(6));

        e.preventDefault();
    }

    readonly clickEvent = new Event<string>();
}
