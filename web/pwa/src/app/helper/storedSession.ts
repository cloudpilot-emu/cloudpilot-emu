import { environment } from '../../environments/environment';

const KEY = 'cloudpilot-session' + (environment.localStorageSuffix ? `-${environment.localStorageSuffix}` : '');

export function hasStoredSession(): boolean {
    return !!localStorage.getItem(KEY);
}

export function getStoredSession(): number | undefined {
    const storedSession = localStorage.getItem(KEY);

    return storedSession ? parseInt(storedSession, 10) : undefined;
}

export function setStoredSession(session: number): void {
    localStorage.setItem(KEY, session + '');
}

export function clearStoredSession(): void {
    localStorage.removeItem(KEY);
}
