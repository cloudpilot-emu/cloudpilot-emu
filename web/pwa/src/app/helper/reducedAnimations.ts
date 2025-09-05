import { Animation, createAnimation } from '@ionic/angular';

import { localStorageGetWithFallback, localStorageSet } from './localStorage';

const KEY = 'reduced-animations';

const enterAnimationActionSheet = (baseEl: HTMLElement): Animation => {
    (baseEl.querySelector('ion-backdrop') as HTMLElement).style.opacity = 'var(--backdrop-opacity)';
    (baseEl.querySelector('.action-sheet-wrapper') as HTMLElement).style.transform = 'translateY(0%)';

    return createAnimation();
};

const enterAnimationLoader = (baseEl: HTMLElement): Animation => {
    (baseEl.querySelector('ion-backdrop') as HTMLElement).style.opacity = 'var(--backdrop-opacity)';

    const wrapper = baseEl.querySelector('.loading-wrapper') as HTMLElement;
    wrapper.style.transform = 'scale(1)';
    wrapper.style.opacity = '1';

    return createAnimation();
};

const enterAnimationAlert = (baseEl: HTMLElement): Animation => {
    (baseEl.querySelector('ion-backdrop') as HTMLElement).style.opacity = 'var(--backdrop-opacity)';

    const wrapper = baseEl.querySelector('.alert-wrapper') as HTMLElement;
    wrapper.style.transform = 'scale(1)';
    wrapper.style.opacity = '1';

    return createAnimation();
};

export function setReducedAnimations(flag: boolean) {
    localStorageSet(KEY, flag ? '1' : '0');
}

export function getReducedAnimations(): boolean {
    const storedValue = localStorageGetWithFallback(KEY);

    return (storedValue ?? '0') === '1';
}

export function ionAnimationConfig() {
    return getReducedAnimations()
        ? {
              actionSheetEnter: enterAnimationActionSheet,
              loadingEnter: enterAnimationLoader,
              alertEnter: enterAnimationAlert,
          }
        : {};
}
