import { localStorageDelete, localStorageGet, localStorageSet } from './localStorage';

const KEY = 'cloudpilot-session';

export function hasStoredSession(): boolean {
    return !!localStorageGet(KEY);
}

export function getStoredSession(): number | undefined {
    const storedSession = localStorageGet(KEY);

    return storedSession ? parseInt(storedSession, 10) : undefined;
}

export function setStoredSession(session: number): void {
    localStorageSet(KEY, session + '');
}

export function clearStoredSession(): void {
    localStorageDelete(KEY);
}
