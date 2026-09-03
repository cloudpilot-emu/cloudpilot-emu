export const enum ScreenSize {
    screen160x160 = '160x160',
    screen160x220 = '160x220',
    screen240x240 = '240x240',
    screen240x320 = '240x320',
    screen320x320 = '320x320',
    screen320x480 = '320x480',
    screen480x480 = '480x480',
    screen480x720 = '480x720',
}

export interface Dimensions {
    readonly screenSize: ScreenSize;
    readonly width: number;
    readonly height: number;
    readonly silkscreenHeight: number;
    readonly silkscreenOvershoot?: number;
    readonly siklscreenShift?: number;
}

export function densityForScreenSize(screenSize: ScreenSize): number {
    switch (screenSize) {
        case ScreenSize.screen160x160:
        case ScreenSize.screen160x220:
            return 1;

        case ScreenSize.screen240x240:
        case ScreenSize.screen240x320:
            return 1.5;

        case ScreenSize.screen320x320:
        case ScreenSize.screen320x480:
            return 2;

        case ScreenSize.screen480x480:
        case ScreenSize.screen480x720:
            return 3;

        default:
            screenSize satisfies never;
            throw new Error('unreachable');
    }
}
