import { environment } from './../../environments/environment';
import { v4 as uuid } from 'uuid';

export const PAGELOCK_KEY =
    'cloudpilot-lock' + (environment.localStorageSuffix ? `-${environment.localStorageSuffix}` : '');
export const PAGELOCK_TOKEN = uuid();

localStorage.setItem(PAGELOCK_KEY, PAGELOCK_TOKEN);

export function pageLockIntact() {
    return localStorage.getItem(PAGELOCK_KEY) === PAGELOCK_TOKEN;
}
