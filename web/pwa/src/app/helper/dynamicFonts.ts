import { isAndroid, isIOS } from '@common/helper/browser';

import { localStorageGet, localStorageSet } from './localStorage';

const KEY = 'DYNAMIC_FONTS';

export function initializeDynamicFOnts(): void {
    if (!dynamicFontsSupport()) return;

    updateDynamicFontSupport();
}

export function dynamicFontsEnabled(): boolean {
    if (!dynamicFontsSupport()) return false;

    return (localStorageGet(KEY) ?? '1') === '1';
}

export function setDynamicFontsEnabled(enabled: boolean): void {
    if (!dynamicFontsSupport()) return;

    localStorageSet(KEY, enabled ? '1' : '0');
    updateDynamicFontSupport();
}

export function dynamicFontsSupport(): boolean {
    return isIOS;
}

function updateDynamicFontSupport(): void {
    if (!dynamicFontsSupport()) return;

    if (dynamicFontsEnabled()) {
        document.documentElement.style.removeProperty('--ion-dynamic-font');
    } else {
        document.documentElement.style.setProperty('--ion-dynamic-font', 'initial');
    }
}
