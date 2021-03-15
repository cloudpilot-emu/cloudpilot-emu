export const isSafari = 'safari' in window;
export const isIOS =
    !!navigator.platform.match(/iPhone|iPad|iPod/) ||
    (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
