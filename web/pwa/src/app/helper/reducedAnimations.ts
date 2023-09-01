import { isIOS, isSafari } from '@common/helper/browser';
import { createAnimation, Animation } from '@ionic/angular';

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
    localStorage.setItem(KEY, flag ? '1' : '0');
}

export function getReducedAnimations(): boolean {
    const storedValue = localStorage.getItem(KEY);

    if (storedValue === null) {
        const value = isSafari && !isIOS;
        setReducedAnimations(value);

        return value;
    }

    return storedValue === '1';
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
