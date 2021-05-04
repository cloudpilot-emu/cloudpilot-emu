import { DeviceId, isColor } from '../model/DeviceId';

import { EmulationStatistics } from './../model/EmulationStatistics';
import { GRAYSCALE_PALETTE_HEX } from './emulation.service';
import { Injectable } from '@angular/core';
import { PalmButton } from '../helper/Cloudpilot';
import { ScreenSize } from '../model/Dimensions';
import { Session } from '../model/Session';
import { SnapshotStatistics } from './../model/SnapshotStatistics';
import { deviceDimensions } from '../helper/deviceProperties';

const BACKGROUND_COLOR_SILKSCREEN = GRAYSCALE_PALETTE_HEX[2];
const BACKGROUND_COLOR_GRAYSCALE_DEVICE = GRAYSCALE_PALETTE_HEX[0];
const BACKGROUND_COLOR_COLOR_DEVICE = 'white';
const BACKGROUND_ACTIVE_BUTTON = 'rgba(0,0,0,0.2)';

function loadImage(url: string): Promise<HTMLImageElement> {
    return new Promise<HTMLImageElement>((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}

async function prerenderedButton(url: string, screenSize: ScreenSize): Promise<HTMLCanvasElement> {
    const canvas = document.createElement('canvas');
    canvas.width = (screenSize === ScreenSize.screen320x320 ? 640 : 480) * devicePixelRatio;
    canvas.height = (screenSize === ScreenSize.screen320x320 ? 120 : 90) * devicePixelRatio;

    const ctx = canvas.getContext('2d');
    if (!ctx) throw new Error('get a new browser');

    ctx.drawImage(await loadImage(url), 0, 0, canvas.width, canvas.height);

    return canvas;
}

function prerenderedButtonFactor(url: string) {
    const sizes = new Map<ScreenSize, Promise<HTMLCanvasElement>>();

    return (screenSize: ScreenSize): Promise<HTMLCanvasElement> => {
        if (!sizes.has(screenSize)) sizes.set(screenSize, prerenderedButton(url, screenSize));

        return sizes.get(screenSize)!;
    };
}

const IMAGE_SILKSCREEN_V = loadImage('assets/skin/silkscreen/v.svg');
const IMAGE_SILKSCREEN_M515 = loadImage('assets/skin/silkscreen/m515.svg');
const IMAGE_SILKSCREEN_IIIC = loadImage('assets/skin/silkscreen/iiic.svg');
const IMAGE_SILKSCREEN_M130 = loadImage('assets/skin/silkscreen/m130.svg');

const IMAGE_BUTTONS_V = prerenderedButtonFactor('assets/skin/hard-buttons/v.svg');
const IMAGE_BUTTONS_M515 = prerenderedButtonFactor('assets/skin/hard-buttons/m515.svg');
const IMAGE_BUTTONS_IIIC = prerenderedButtonFactor('assets/skin/hard-buttons/iiic.svg');
const IMAGE_BUTTONS_M130 = prerenderedButtonFactor('assets/skin/hard-buttons/m130.svg');

const DEFAULT_DIMENSIONS = deviceDimensions(DeviceId.m515);

[IMAGE_BUTTONS_IIIC, IMAGE_BUTTONS_V, IMAGE_BUTTONS_M130, IMAGE_BUTTONS_M515].forEach((prerender) => {
    prerender(ScreenSize.screen160x160);
    prerender(ScreenSize.screen320x320);
});

@Injectable({
    providedIn: 'root',
})
export class CanvasDisplayService {
    constructor() {}

    get scale(): number {
        return (this.dimensions.screenSize === ScreenSize.screen320x320 ? 2 : 3) * devicePixelRatio;
    }

    get border(): number {
        return 1 * this.scale;
    }

    get width(): number {
        return this.scale * this.dimensions.width + 2 * this.border;
    }

    get separator(): number {
        return this.dimensions.screenSize === ScreenSize.screen320x320 ? 2 : 1;
    }

    get buttonsHeight(): number {
        return this.dimensions.screenSize === ScreenSize.screen320x320 ? 60 : 30;
    }

    get height(): number {
        return (
            this.scale *
                (this.dimensions.height + this.dimensions.silkscreenHeight + this.separator + this.buttonsHeight) +
            2 * this.border
        );
    }

    async initialize(canvas: HTMLCanvasElement, session: Session | undefined): Promise<void> {
        this.dimensions = session ? deviceDimensions(session.device) : DEFAULT_DIMENSIONS;

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

        const image = await this.silkscreenImage();

        this.fillRect(
            0,
            this.dimensions.height + this.separator,
            this.dimensions.width,
            this.dimensions.silkscreenHeight,
            BACKGROUND_COLOR_SILKSCREEN
        );

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(
            image,
            this.border,
            this.border + (this.dimensions.height + this.separator) * this.scale,
            this.dimensions.width * this.scale,
            this.dimensions.silkscreenHeight * this.scale
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
            0,
            this.dimensions.height + this.separator,
            this.dimensions.width,
            this.dimensions.silkscreenHeight,
            'rgba(255,255,255,0.6)'
        );

        const fontFactor = this.dimensions.screenSize === ScreenSize.screen320x320 ? 2 : 1;

        this.ctx.font = `${this.scale * 6 * fontFactor}px monospace`;
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
                this.border + fontFactor * this.scale,
                this.border + (this.dimensions.height + fontFactor * (8 + i * 6)) * this.scale
            )
        );
    }

    async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        if (!this.ctx) return;

        const ystart = this.dimensions.height + this.dimensions.silkscreenHeight + this.separator;
        const buttonWidth = this.dimensions.screenSize === ScreenSize.screen320x320 ? 60 : 30;

        this.ctx.drawImage(
            await this.buttonsImage(),
            this.border,
            this.border + this.scale * ystart,
            this.dimensions.width * this.scale,
            this.buttonsHeight * this.scale
        );

        if (activeButtons.includes(PalmButton.cal)) {
            this.fillRect(0, ystart, buttonWidth, this.buttonsHeight, BACKGROUND_ACTIVE_BUTTON);
        }
        if (activeButtons.includes(PalmButton.phone)) {
            this.fillRect(buttonWidth, ystart, buttonWidth, this.buttonsHeight, BACKGROUND_ACTIVE_BUTTON);
        }
        if (activeButtons.includes(PalmButton.todo)) {
            this.fillRect(
                this.dimensions.width - 2 * buttonWidth,
                ystart,
                buttonWidth,
                this.buttonsHeight,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.notes)) {
            this.fillRect(
                this.dimensions.width - buttonWidth,
                ystart,
                buttonWidth,
                this.buttonsHeight,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.up)) {
            this.fillRect(
                2 * buttonWidth,
                ystart,
                this.dimensions.width - 4 * buttonWidth,
                this.buttonsHeight / 2,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
        if (activeButtons.includes(PalmButton.down)) {
            this.fillRect(
                2 * buttonWidth,
                ystart + this.buttonsHeight / 2,
                this.dimensions.width - 4 * buttonWidth,
                this.buttonsHeight / 2,
                BACKGROUND_ACTIVE_BUTTON
            );
        }
    }

    drawEmulationCanvas(canvas: HTMLCanvasElement): void {
        if (!this.ctx) return;

        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(
            canvas,
            this.border,
            this.border,
            this.scale * this.dimensions.width,
            this.scale * this.dimensions.height
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
            Math.floor((((e.clientX - contentX) / contentWidth) * this.width) / this.scale) - this.border / this.scale;
        let y =
            Math.floor((((e.clientY - contentY) / contentHeight) * this.height) / this.scale) -
            this.border / this.scale;

        // The canvas layout inside the border is as follows:
        //
        // * 0 .. 159   : LCD
        // * 160        : separator
        // * 161 .. 220 : silkscreen
        // * 221 .. 250 : buttons
        //
        // we map this to 160x250 lines by mapping the separator to the silkscreen

        if (y >= this.dimensions.height + this.separator) y -= 1;

        const totalHeight = this.dimensions.height + this.dimensions.silkscreenHeight + this.buttonsHeight;

        if (clip) {
            if (x < 0) x = 0;
            if (x >= this.dimensions.width) x = this.dimensions.width - 1;
            if (y < 0) y = 0;
            if (y >= totalHeight) {
                y = this.dimensions.height + this.dimensions.silkscreenHeight - 1;
            }
        } else {
            if (x < 0 || x >= this.dimensions.width || y < 0 || y >= totalHeight) {
                return undefined;
            }
        }

        return [x, y];
    }

    isButtons([, y]: [number, number]): boolean {
        return y >= this.dimensions.height + this.dimensions.silkscreenHeight;
    }

    determineButton([x, y]: [number, number]): PalmButton {
        const buttonWidth = this.dimensions.screenSize === ScreenSize.screen320x320 ? 60 : 30;

        if (x >= this.dimensions.width - buttonWidth) return PalmButton.notes;
        if (x >= this.dimensions.width - 2 * buttonWidth) return PalmButton.todo;
        if (x >= 2 * buttonWidth) {
            return y >=
                this.dimensions.height + this.dimensions.silkscreenHeight + this.separator + this.buttonsHeight / 2
                ? PalmButton.down
                : PalmButton.up;
        }
        if (x >= buttonWidth) return PalmButton.phone;

        return PalmButton.cal;
    }

    private fillRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(
            this.border + this.scale * x,
            this.border + this.scale * y,
            this.scale * width,
            this.scale * height
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

    private silkscreenImage(): Promise<HTMLImageElement> {
        switch (this.session?.device) {
            case DeviceId.m515:
                return IMAGE_SILKSCREEN_M515;

            case DeviceId.iiic:
                return IMAGE_SILKSCREEN_IIIC;

            case DeviceId.i710:
            case DeviceId.m130:
                return IMAGE_SILKSCREEN_M130;

            case DeviceId.palmV:
            default:
                return IMAGE_SILKSCREEN_V;
        }
    }

    private buttonsImage(): Promise<HTMLCanvasElement> {
        switch (this.session?.device) {
            case DeviceId.m515:
            case DeviceId.i710:
                return IMAGE_BUTTONS_M515(this.dimensions.screenSize);

            case DeviceId.iiic:
                return IMAGE_BUTTONS_IIIC(this.dimensions.screenSize);

            case DeviceId.m130:
                return IMAGE_BUTTONS_M130(this.dimensions.screenSize);

            case DeviceId.palmV:
            default:
                return IMAGE_BUTTONS_V(this.dimensions.screenSize);
        }
    }

    private backgroundColor(): string {
        return isColor(this.session?.device) ? BACKGROUND_COLOR_COLOR_DEVICE : BACKGROUND_COLOR_GRAYSCALE_DEVICE;
    }

    private ctx: CanvasRenderingContext2D | undefined;
    private session: Session | undefined;

    private dimensions = DEFAULT_DIMENSIONS;
}
