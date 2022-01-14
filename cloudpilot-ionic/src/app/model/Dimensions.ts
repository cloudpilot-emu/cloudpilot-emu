export const enum ScreenSize {
    screen160x160,
    screen320x320,
    screen240x320,
}

export interface Dimensions {
    readonly screenSize: ScreenSize;
    readonly width: number;
    readonly height: number;
    readonly silkscreenHeight: number;
}
