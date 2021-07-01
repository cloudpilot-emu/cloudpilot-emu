import { AbstractControl, ValidationErrors } from '@angular/forms';
const DEFAULT_PORT = 8666;

const REGEX_HOST = /^[\da-z\-\.]+$/i;
const REGEXT_HOST_PORT = /^[\da-z\-\.]+:\d+$/i;

export function normalizeProxyAddress(address: string | undefined): string | undefined {
    if (!address) return undefined;

    if (address.match(REGEX_HOST)) return `ws://${address}:${DEFAULT_PORT}`;

    if (address.match(REGEXT_HOST_PORT)) return `ws://${address}`;

    try {
        const url = new URL(address);

        switch (url.protocol) {
            case 'http:':
                url.protocol = 'ws:';
                return url.toString();

            case 'https:':
                url.protocol = 'wss:';
                return url.toString();

            default:
                return undefined;
        }
    } catch (e) {
        return undefined;
    }
}

export function validateProxyAddress(control: AbstractControl): ValidationErrors | null {
    return normalizeProxyAddress(control.value) !== undefined ? null : { proxyAddress: 'invalid' };
}
