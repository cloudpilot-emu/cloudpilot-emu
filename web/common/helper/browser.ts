export const isMacOSSafari = 'safari' in window;
export const isIOS =
    !!navigator.platform.match(/iPhone|iPad|iPod/) ||
    (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
export const isIOSSafari = isIOS && !navigator.userAgent.match(/(crios)|(fxios)/i);
export const isIOSNative = !!navigator.userAgent.match(/pwashell/i);

export const isAndroid = !!navigator.userAgent.match(/android/i);

export const isFirefox = navigator.userAgent.toLowerCase().indexOf('firefox') >= 0;
