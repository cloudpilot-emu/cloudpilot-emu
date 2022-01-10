import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import Url from 'url-parse';

let initialInstallationUrl: string | undefined;

export function bootstrapLinkApi(): void {
    if (!window.location.hash.startsWith('#/install?')) {
        return;
    }

    const query = window.location.hash.split('?')[1];
    if (!query) {
        return;
    }

    const pairs = query.split('&').map((x) => x.split('='));
    for (const [name, value] of pairs) {
        if (name === 'url' && value) {
            try {
                initialInstallationUrl = decodeURIComponent(value);
                return;
            } catch (e) {
                continue;
            }
        }
    }
}

@Injectable({ providedIn: 'root' })
export class LinkApi {
    constructor() {
        if (initialInstallationUrl) {
            this.dispatchInstallationRequest(initialInstallationUrl);
        }
    }

    dispatchInstallationRequest(url: string) {
        if (!url) {
            return;
        }

        try {
            url = new Url(url).toString();
        } catch (e) {
            console.error(`invalid URL: ${url}`);
            return;
        }

        this.installationUrl = url;
        this.installationRequestEvent.dispatch();
    }

    hasPendingInstallationRequest(): boolean {
        return this.installationUrl !== undefined;
    }

    receivePendingInstallationUrl(): string | undefined {
        const url = this.installationUrl;
        this.installationUrl = undefined;

        return url;
    }

    installationRequestEvent = new Event<void>();

    private installationUrl: string | undefined;
}
