import { isIOS, isIOSSafari } from '@common/helper/browser';

export const enum IndicatorFixMode {
    none = 'none',
    portrait = 'portrait',
    landscape = 'landscape',
    all = 'all',
}

const CLASS_ENABLE_PORTRAIT = 'cp-indicator-fix-portrait';
const CLASS_ENABLE_LANDSCAPE = 'cp-indicator-fix-landscape';
const KEY = 'cloudpilot-indicator-fix-mode';

let injectedStyle: HTMLStyleElement | undefined;

export function getIndicatorFixMode(): IndicatorFixMode {
    return (localStorage.getItem(KEY) as IndicatorFixMode | null) ?? IndicatorFixMode.none;
}

export function isIndicatorFixApplicable(): boolean {
    return isIOS && isIOSSafari && !navigator?.standalone;
}

export function applyHomeIndicatorFix(mode?: IndicatorFixMode) {
    if (!isIndicatorFixApplicable()) return;

    if (!injectedStyle) {
        injectedStyle = document.createElement('style');
        injectedStyle.textContent = `
           @media (orientation: portrait) and (min-height: ${window.screen.height - 110}px) {
                html.${CLASS_ENABLE_PORTRAIT} {
                    --ion-safe-area-bottom: max(env(safe-area-inset-bottom, 0), 20px);
                }
            }

            @media (orientation: landscape) {
                html.${CLASS_ENABLE_LANDSCAPE} {
                    --ion-safe-area-bottom: max(env(safe-area-inset-bottom, 0), 20px);
                }
            }
        `;

        document.head.appendChild(injectedStyle);
    }

    if (mode !== undefined) {
        localStorage.setItem(KEY, mode);
    } else {
        mode = getIndicatorFixMode();
    }

    const classList = document.documentElement.classList;

    classList.toggle(CLASS_ENABLE_PORTRAIT, mode === IndicatorFixMode.all || mode === IndicatorFixMode.portrait);
    classList.toggle(CLASS_ENABLE_LANDSCAPE, mode === IndicatorFixMode.all || mode === IndicatorFixMode.landscape);
}
