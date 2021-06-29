import { AbstractControl, ValidationErrors } from '@angular/forms';
const DEFAULT_PORT = 8666;

const REGEX_HOST = /^[\da-z\-\.]+$/i;
const REGEXT_HOST_PORT = /^[\da-z\-\.]+:\d+$/i;

export function normalizeProxyAddress(address: string): string | undefined {
    if (!address) return undefined;

    if (address.match(REGEX_HOST)) return `http://${address}:${DEFAULT_PORT}`;

    if (address.match(REGEXT_HOST_PORT)) return `http://${address}`;

    try {
        const url = new URL(address);

        return ['http:', 'https:'].includes(url.protocol) ? url.toString() : undefined;
    } catch (e) {
        return undefined;
    }
}

export function validateProxyAddress(control: AbstractControl): ValidationErrors | null {
    return normalizeProxyAddress(control.value) !== undefined ? null : { proxyAddress: 'invalid' };
}
