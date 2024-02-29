export const isSafari = 'safari' in window;
export const isIOSSafari = !navigator.userAgent.match(/(crios)|(fxios)/i);
export const isIOS =
    !!navigator.platform.match(/iPhone|iPad|iPod/) ||
    (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
export const iosVersion = getIosVersion();
export const isIOSNative = !!navigator.userAgent.match(/ pwashell$/i);

export const isAndroid = !!navigator.userAgent.match(/android/i);

export const isFirefox = navigator.userAgent.toLowerCase().indexOf('firefox') >= 0;

function getIosVersion(): number {
    const match = navigator.userAgent.match(/\WVersion\/(\d+)\.(\d+)(?:\.(\d+))?\W/);
    if (!match) return 0;

    const major = parseInt(match[1], 10);
    const minor = parseInt(match[2], 10);
    const patch = parseInt(match[3], 10);

    if (isNaN(major) || isNaN(minor) || major > 255 || minor > 255) return 0;
    if (!isNaN(patch) && patch > 255) return 0;

    return (major << 16) | (minor << 8) | (isNaN(patch) ? 0 : patch);
}

export function version(major: number, minor: number, patch: number) {
    return ((major & 0xff) << 16) | ((minor & 0xff) << 8) | (patch & 0xff);
}
