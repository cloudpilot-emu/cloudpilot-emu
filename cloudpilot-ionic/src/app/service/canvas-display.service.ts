import { DeviceId, isColor } from '../model/DeviceId';

import { EmulationStatistics } from './../model/EmulationStatistics';
import { GRAYSCALE_PALETTE_HEX } from './emulation.service';
import { Injectable } from '@angular/core';
import { PalmButton } from '../helper/Cloudpilot';
import { ScreenSize } from '../model/Dimensions';
import { Session } from '../model/Session';
import { SnapshotStatistics } from './../model/SnapshotStatistics';
import { deviceDimensions } from '../helper/deviceProperties';

const DEFAULT_DEVICE = DeviceId.m515;
const BACKGROUND_COLOR_SILKSCREEN = GRAYSCALE_PALETTE_HEX[2];
const BACKGROUND_COLOR_GRAYSCALE_DEVICE = GRAYSCALE_PALETTE_HEX[0];
const BACKGROUND_COLOR_COLOR_DEVICE = 'white';
const BACKGROUND_ACTIVE_BUTTON = 'rgba(0,0,0,0.2)';

interface FrameDependent {
    frameDevice: number;
    frameCanvas: number;
}

interface Layout {
    scale: number;
    borderWidth: FrameDependent;
    width: FrameDependent;
    height: FrameDependent;
    screenHeight: FrameDependent;
    screenWidth: FrameDependent;
    screenLeft: FrameDependent;
    screenTop: FrameDependent;
    screenBottom: FrameDependent;
    separatorHeight: FrameDependent;
    silkscreenHeight: FrameDependent;
    silkscreenTop: FrameDependent;
    silkscreenBottom: FrameDependent;
    buttonHeight: FrameDependent;
    buttonWidth: FrameDependent;
    buttonTop: FrameDependent;
    buttonBottom: FrameDependent;
}

function calculateLayout(device: DeviceId): Layout {
    const dimensions = deviceDimensions(device);
    const scale = (dimensions.screenSize === ScreenSize.screen320x320 ? 2 : 3) * devicePixelRatio;
    const borderWidth: FrameDependent = { frameDevice: 1, frameCanvas: scale };

    const dist = (x: number): FrameDependent => ({ frameDevice: x, frameCanvas: x * scale });
    const coord = (x: number): FrameDependent => ({ frameDevice: x, frameCanvas: borderWidth.frameCanvas + x * scale });

    const separatorHeight = dist(1);
    const buttonHeight = dist(dimensions.screenSize === ScreenSize.screen320x320 ? 60 : 30);

    return {
        scale,
        borderWidth,
        height: dist(
            2 * borderWidth.frameDevice +
                dimensions.height +
                separatorHeight.frameDevice +
                dimensions.silkscreenHeight +
                buttonHeight.frameDevice
        ),
        width: dist(2 * borderWidth.frameDevice + dimensions.width),
        screenHeight: dist(dimensions.height),
        screenWidth: dist(dimensions.width),
        screenLeft: coord(0),
        screenTop: coord(0),
        screenBottom: coord(dimensions.height),
        separatorHeight,
        silkscreenHeight: dist(dimensions.silkscreenHeight),
        silkscreenTop: coord(dimensions.height + separatorHeight.frameDevice),
        silkscreenBottom: coord(dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight),
        buttonHeight,
        buttonWidth: buttonHeight,
        buttonTop: coord(dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight),
        buttonBottom: coord(
            dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight + buttonHeight.frameDevice
        ),
    };
}

type PrerenderedImage = (width: number, height: number) => Promise<HTMLCanvasElement>;

function prerender(image: Promise<HTMLImageElement>): PrerenderedImage {
    let cachedWidth: number;
    let cachedHeight: number;
    const canvas = document.createElement('canvas');

    return async (width: number, height: number) => {
        if (cachedWidth === width && cachedHeight === height) {
            return canvas;
        }

        canvas.width = width;
        canvas.height = height;

        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        ctx.drawImage(await image, 0, 0, canvas.width, canvas.height);

        cachedWidth = width;
        cachedHeight = height;

        return canvas;
    };
}

function loadImage(url: string): Promise<HTMLImageElement> {
    return new Promise<HTMLImageElement>((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}

const IMAGE_SILKSCREEN_V = prerender(loadImage('assets/skin/silkscreen/v.svg'));
const IMAGE_SILKSCREEN_M515 = prerender(loadImage('assets/skin/silkscreen/m515.svg'));
const IMAGE_SILKSCREEN_IIIC = prerender(loadImage('assets/skin/silkscreen/iiic.svg'));
const IMAGE_SILKSCREEN_M130 = prerender(loadImage('assets/skin/silkscreen/m130.svg'));
const IMAGE_SILKSCREEN_TUNGSTENW = prerender(loadImage('assets/skin/silkscreen/tungstenw.svg'));

const IMAGE_BUTTONS_V = prerender(loadImage('assets/skin/hard-buttons/v.svg'));
const IMAGE_BUTTONS_M515 = prerender(loadImage('assets/skin/hard-buttons/m515.svg'));
const IMAGE_BUTTONS_IIIC = prerender(loadImage('assets/skin/hard-buttons/iiic.svg'));
const IMAGE_BUTTONS_M130 = prerender(loadImage('assets/skin/hard-buttons/m130.svg'));
const IMAGE_BUTTONS_TUNGSTENW = prerender(loadImage('assets/skin/hard-buttons/tungstenw.svg'));

@Injectable({
    providedIn: 'root',
})
export class CanvasDisplayService {
    constructor() {}

    get width(): number {
        return this.layout.width.frameCanvas;
    }

    get height(): number {
        return this.layout.height.frameCanvas;
    }

    async initialize(canvas: HTMLCanvasElement, session: Session | undefined): Promise<void> {
        this.layout = calculateLayout(session?.device ?? DeviceId.m515);

        canvas.width = this.width;
        canvas.height = this.height;

        const ctx = canvas.getContext('2d');
        if (!ctx) {
            throw new Error('canvas not supported - get a new browser');
        }

        this.ctx = ctx;

        this.session = session;
        this.fillCanvasRect(0, 0, this.width, this.height, this.backgroundColor());

        await this.drawSilkscreen();
        await this.drawButtons();
    }

    async drawSilkscreen(): Promise<void> {
        if (!this.ctx) return;

        this.fillRect(
            this.layout.screenLeft.frameDevice,
            this.layout.silkscreenTop.frameDevice,
            this.layout.screenWidth.frameDevice,
            this.layout.silkscreenHeight.frameDevice,
            BACKGROUND_COLOR_SILKSCREEN
        );

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(
            await this.silkscreenImage()(this.layout.screenWidth.frameCanvas, this.layout.silkscreenHeight.frameCanvas),
            this.layout.borderWidth.frameCanvas,
            this.layout.silkscreenTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.silkscreenHeight.frameCanvas
        );
    }

    async drawStatistics(
        snapshotStatistics?: SnapshotStatistics,
        emulationStatistics?: EmulationStatistics
    ): Promise<void> {
        if (!this.ctx) return;

        await this.drawSilkscreen();

        this.ctx.beginPath();
        this.fillRect(
            this.layout.screenLeft.frameDevice,
            this.layout.silkscreenTop.frameDevice,
            this.layout.screenWidth.frameDevice,
            this.layout.silkscreenHeight.frameDevice,
            'rgba(255,255,255,0.6)'
        );

        const screenSize = deviceDimensions(this.session?.device ?? DEFAULT_DEVICE).screenSize;
        const fontScale = screenSize === ScreenSize.screen320x320 ? 2 : 1;

        this.ctx.font = `${this.layout.scale * 6 * fontScale}px monospace`;
        this.ctx.fillStyle = 'black';
        [
            ...(!snapshotStatistics && !emulationStatistics ? ['collecting statistics...'] : []),
            ...(snapshotStatistics
                ? [
                      `last snapshot          : ${new Date(snapshotStatistics.timestamp).toLocaleTimeString()}`,
                      `snapshot pages         : ${snapshotStatistics.pages}`,
                      `snapshot time total    : ${snapshotStatistics.timeTotal.toFixed(2)} msec`,
                      `snapshot time blocking : ${snapshotStatistics.timeBlocking.toFixed(2)} msec`,
                  ]
                : []),
            ...(emulationStatistics
                ? [
                      `host speed             : ${emulationStatistics.hostSpeed.toFixed(2)}x`,
                      `emulation speed        : ${emulationStatistics.emulationSpeed.toFixed(2)}x`,
                      `average FPS            : ${emulationStatistics.averageFPS.toFixed(2)}`,
                  ]
                : []),
        ].forEach((line, i) =>
            this.ctx!.fillText(
                line,
                this.layout.screenLeft.frameCanvas + fontScale * this.layout.scale,
                this.layout.silkscreenTop.frameCanvas + fontScale * this.layout.scale * (8 + i * 6)
            )
        );
    }

    async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        if (!this.ctx) return;

        this.ctx.drawImage(
            await this.buttonsImage()(this.layout.screenWidth.frameCanvas, this.layout.buttonHeight.frameCanvas),
            this.layout.screenLeft.frameCanvas,
            this.layout.buttonTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.buttonHeight.frameCanvas
        );

        if (activeButtons.includes(PalmButton.cal)) {
            this.fillRect(
                0,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.phone)) {
            this.fillRect(
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.todo)) {
            this.fillRect(
                this.layout.screenWidth.frameDevice - 2 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.notes)) {
            this.fillRect(
                this.layout.screenWidth.frameDevice - this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.up)) {
            this.fillRect(
                2 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.screenWidth.frameDevice - 4 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice / 2,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.down)) {
            this.fillRect(
                2 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice + this.layout.buttonHeight.frameDevice / 2,
                this.layout.screenWidth.frameDevice - 4 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice / 2,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
    }

    drawEmulationCanvas(canvas: HTMLCanvasElement): void {
        if (!this.ctx) return;

        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(
            canvas,
            this.layout.screenLeft.frameCanvas,
            this.layout.screenTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.screenHeight.frameCanvas
        );
    }

    eventToPalmCoordinates(e: MouseEvent | Touch, clip = false): [number, number] | undefined {
        if (!this.ctx) return;

        const bb = this.ctx.canvas.getBoundingClientRect();

        let contentX: number;
        let contentY: number;
        let contentWidth: number;
        let contentHeight: number;

        // CSS object-fit keeps the aspect ratio of the canvas content, but the canvas itself
        // looses aspect and fills the container -> manually calculate the metrics for the content
        if (bb.width / bb.height > this.width / this.height) {
            contentHeight = bb.height;
            contentWidth = (this.width / this.height) * bb.height;
            contentY = bb.top;
            contentX = bb.left + (bb.width - contentWidth) / 2;
        } else {
            contentWidth = bb.width;
            contentHeight = (this.height / this.width) * bb.width;
            contentX = bb.left;
            contentY = bb.top + (bb.height - contentHeight) / 2;
        }

        // Compensate for the border
        let x =
            Math.floor((((e.clientX - contentX) / contentWidth) * this.width) / this.layout.scale) -
            this.layout.borderWidth.frameDevice;
        let y =
            Math.floor((((e.clientY - contentY) / contentHeight) * this.height) / this.layout.scale) -
            this.layout.borderWidth.frameDevice;

        // The canvas layout inside the border is as follows:
        //
        // * 0 .. 159   : LCD
        // * 160        : separator
        // * 161 .. 220 : silkscreen
        // * 221 .. 250 : buttons
        //
        // we map this to 160x250 lines by mapping the separator to the silkscreen

        if (y >= this.layout.screenHeight.frameDevice) {
            if (y <= this.layout.screenHeight.frameDevice + this.layout.separatorHeight.frameDevice) {
                y = this.layout.screenHeight.frameDevice;
            } else {
                y -= this.layout.separatorHeight.frameDevice;
            }
        }

        const totalHeight =
            this.layout.screenHeight.frameDevice +
            this.layout.silkscreenHeight.frameDevice +
            this.layout.buttonHeight.frameDevice;

        if (clip) {
            if (x < 0) x = 0;
            if (x >= this.layout.screenWidth.frameDevice) x = this.layout.screenWidth.frameDevice - 1;
            if (y < 0) y = 0;
            if (y >= totalHeight) y = totalHeight - 1;
        } else {
            if (x < 0 || x >= this.layout.screenWidth.frameDevice || y < 0 || y >= totalHeight) {
                return undefined;
            }
        }

        return [x, y];
    }

    isButtons([, y]: [number, number]): boolean {
        return y >= this.layout.screenHeight.frameDevice + this.layout.silkscreenHeight.frameDevice;
    }

    determineButton([x, y]: [number, number]): PalmButton {
        if (x >= this.layout.screenWidth.frameDevice - this.layout.buttonWidth.frameDevice) return PalmButton.notes;
        if (x >= this.layout.screenWidth.frameDevice - 2 * this.layout.buttonWidth.frameDevice) return PalmButton.todo;
        if (x >= 2 * this.layout.buttonWidth.frameDevice) {
            return y >=
                this.layout.screenHeight.frameDevice +
                    this.layout.silkscreenHeight.frameDevice +
                    this.layout.buttonHeight.frameDevice / 2
                ? PalmButton.down
                : PalmButton.up;
        }
        if (x >= this.layout.buttonWidth.frameDevice) return PalmButton.phone;

        return PalmButton.cal;
    }

    private fillRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(
            this.layout.borderWidth.frameCanvas + this.layout.scale * x,
            this.layout.borderWidth.frameCanvas + this.layout.scale * y,
            this.layout.scale * width,
            this.layout.scale * height
        );
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    private fillCanvasRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(x, y, width, height);
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    private silkscreenImage(): PrerenderedImage {
        switch (this.session?.device) {
            case DeviceId.m515:
                return IMAGE_SILKSCREEN_M515;

            case DeviceId.iiic:
                return IMAGE_SILKSCREEN_IIIC;

            case DeviceId.m130:
                return IMAGE_SILKSCREEN_M130;

            case DeviceId.i710:
                return IMAGE_SILKSCREEN_TUNGSTENW;

            case DeviceId.palmV:
            default:
                return IMAGE_SILKSCREEN_V;
        }
    }

    private buttonsImage(): PrerenderedImage {
        switch (this.session?.device) {
            case DeviceId.m515:
                return IMAGE_BUTTONS_M515;

            case DeviceId.iiic:
                return IMAGE_BUTTONS_IIIC;

            case DeviceId.m130:
                return IMAGE_BUTTONS_M130;

            case DeviceId.i710:
                return IMAGE_BUTTONS_TUNGSTENW;

            case DeviceId.palmV:
            default:
                return IMAGE_BUTTONS_V;
        }
    }

    private backgroundColor(): string {
        return isColor(this.session?.device) ? BACKGROUND_COLOR_COLOR_DEVICE : BACKGROUND_COLOR_GRAYSCALE_DEVICE;
    }

    private ctx: CanvasRenderingContext2D | undefined;
    private session: Session | undefined;
    private layout = calculateLayout(DEFAULT_DEVICE);

    // private dimensions = DEFAULT_DIMENSIONS;
}
