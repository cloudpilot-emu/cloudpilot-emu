export const isSafari = 'safari' in window;
export const isIOSSafari = !navigator.userAgent.match(/(crios)|(fxios)/i);
export const isIOS =
    !!navigator.platform.match(/iPhone|iPad|iPod/) ||
    (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
export const isIOS_174 = !!navigator.userAgent.match(/ 17_4 /);

export const isAndroid = !!navigator.userAgent.match(/android/i);

export const isFirefox = navigator.userAgent.toLowerCase().indexOf('firefox') >= 0;
