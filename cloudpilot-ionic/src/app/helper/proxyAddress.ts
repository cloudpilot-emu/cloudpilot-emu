import { AbstractControl, ValidationErrors } from '@angular/forms';

const DEFAULT_PORT = window.location.protocol === 'https:' ? 8667 : 8666;
const REGEX_HOST = /^[\da-z\-\.]+$/i;
const REGEXT_HOST_PORT = /^[\da-z\-\.]+:\d+$/i;

export function normalizeProxyAddress(address: string | undefined): string | undefined {
    if (!address) return undefined;

    const host = window.location.hostname;
    const protocol = host === 'localhost' ? 'http' : window.location.protocol;

    if (address.match(REGEX_HOST)) return `${protocol}//${address}:${DEFAULT_PORT}`;

    if (address.match(REGEXT_HOST_PORT)) return `${protocol}//${address}`;

    try {
        const url = new URL(address);

        if (url.protocol === 'https' || url.protocol === 'http') {
            return url.toString().replace(/\/+$/, '');
        }
    } catch (e) {}

    return undefined;
}

export function validateProxyAddress(control: AbstractControl): ValidationErrors | null {
    return normalizeProxyAddress(control.value) !== undefined ? null : { proxyAddress: 'invalid' };
}
