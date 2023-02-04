export const enum ScreenSize {
    screen160x160,
    screen320x320,
    screen240x320,
    screen320x480,
}

export interface Dimensions {
    readonly screenSize: ScreenSize;
    readonly width: number;
    readonly height: number;
    readonly silkscreenHeight: number;
    readonly silkscreenOvershoot?: number;
    readonly siklscreenShift?: number;
}
