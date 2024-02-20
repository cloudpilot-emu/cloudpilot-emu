import { environment } from 'pwa/src/environments/environment';

function namespaceKey(key: string): string {
    return key + (environment.localStorageSuffix ? `-${environment.localStorageSuffix}` : '');
}

export function localStorageGet(key: string): string | null {
    return localStorage.getItem(namespaceKey(key));
}

export function localStorageGetWithFallback(key: string): string | null {
    return localStorage.getItem(namespaceKey(key)) ?? localStorage.getItem(key);
}

export function localStorageSet(key: string, value: string): void {
    localStorage.setItem(namespaceKey(key), value);
}

export function localStorageDelete(key: string): void {
    localStorage.removeItem(namespaceKey(key));
}
