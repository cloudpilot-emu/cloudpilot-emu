import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import Url from 'url-parse';

let initialInstallationUrl: string | undefined;
let initialImportUrl: string | undefined;

function boostrapOperation(fragment: string): string | undefined {
    if (!window.location.hash.startsWith(fragment)) {
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
                return decodeURIComponent(value);
                // eslint-disable-next-line @typescript-eslint/no-unused-vars
            } catch (e) {
                continue;
            }
        }
    }
}

export function bootstrapLinkApi(): void {
    initialInstallationUrl = boostrapOperation('#/install?');
    initialImportUrl = boostrapOperation('#/import?');
}

export function hasInitialImportRequest(): boolean {
    return initialImportUrl !== undefined;
}

export function hasInitialInstallRequest(): boolean {
    return initialInstallationUrl !== undefined;
}

class Operation {
    dispatchRequest(url: string) {
        if (!url) {
            return;
        }

        try {
            url = new Url(url).toString();
        } catch (e) {
            console.error(`invalid URL: ${url}`, e);
            return;
        }

        this.url = url;
        this.requestEvent.dispatch();
    }

    hasPendingRequest(): boolean {
        return this.url !== undefined;
    }

    receivePendingUrl(): string | undefined {
        const url = this.url;
        this.url = undefined;

        return url;
    }

    requestEvent = new Event<void>();
    private url: string | undefined;
}

@Injectable({ providedIn: 'root' })
export class LinkApi {
    constructor() {
        if (initialInstallationUrl) {
            this.installation.dispatchRequest(initialInstallationUrl);
        }

        if (initialImportUrl) {
            this.import.dispatchRequest(initialImportUrl);
        }
    }

    readonly installation = new Operation();
    readonly import = new Operation();
}
