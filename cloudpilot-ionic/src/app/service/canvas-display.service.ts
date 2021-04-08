import { DeviceId } from '../model/DeviceId';
import { EmulationStatistics } from './../model/EmulationStatistics';
import { GRAYSCALE_PALETTE_HEX } from './emulation.service';
import { Injectable } from '@angular/core';
import { PalmButton } from '../helper/Cloudpilot';
import { Session } from '../model/Session';
import { SnapshotStatistics } from './../model/SnapshotStatistics';

const URL_SILKSCREEN_DEFAULT = 'assets/silkscreen-default.svg';
const URL_SILKSCREEN_M515 = 'assets/silkscreen-m515.svg';
const URL_BUTTONS_DEFAULT = 'assets/hard-buttons-default.svg';
const URL_BUTTONS_M515 = 'assets/hard-buttons-m515.svg';

const BACKGROUND_COLOR_SILKSCREEN = GRAYSCALE_PALETTE_HEX[2];
const BACKGROUND_COLOR_DEFAULT = GRAYSCALE_PALETTE_HEX[0];
const BACKGROUND_COLOR_M515 = 'white';
const BACKGROUND_ACTIVE_BUTTON = 'rgba(0,0,0,0.2)';

export const SCALE = 3 * devicePixelRatio;
export const BORDER = Math.round(1 * SCALE);
export const WIDTH = SCALE * 160 + 2 * BORDER;
export const HEIGHT = SCALE * 251 + 2 * BORDER;

function loadImage(url: string): Promise<HTMLImageElement> {
    return new Promise<HTMLImageElement>((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}

async function prerenderButtons(url: string): Promise<HTMLCanvasElement> {
    const canvas = document.createElement('canvas');
    canvas.width = 160 * SCALE;
    canvas.height = 30 * SCALE;

    const ctx = canvas.getContext('2d');
    if (!ctx) throw new Error('get a new browser');

    ctx.drawImage(await loadImage(url), 0, 0, SCALE * 160, SCALE * 30);

    return canvas;
}

const IMAGE_SILKSCREEN_DEFAULT = loadImage(URL_SILKSCREEN_DEFAULT);
const IMAGE_SILKSCREEN_M515 = loadImage(URL_SILKSCREEN_M515);
const IMAGE_BUTTONS_DEFAULT = prerenderButtons(URL_BUTTONS_DEFAULT);
const IMAGE_BUTTONS_M515 = prerenderButtons(URL_BUTTONS_M515);

@Injectable({
    providedIn: 'root',
})
export class CanvasDisplayService {
    constructor() {}

    async initialize(canvas: HTMLCanvasElement, session: Session | undefined): Promise<void> {
        const ctx = canvas.getContext('2d');
        if (!ctx) {
            throw new Error('canvas not supported - get a new browser');
        }

        this.ctx = ctx;

        this.session = session;
        this.fillCanvasRect(0, 0, WIDTH, HEIGHT, this.backgroundColor());

        await this.drawSilkscreen();
        await this.drawButtons();
    }

    async drawSilkscreen(): Promise<void> {
        if (!this.ctx) return;

        const image = await this.silkscreenImage();

        this.fillRect(0, 161, 160, 60, BACKGROUND_COLOR_SILKSCREEN);

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(image, BORDER, BORDER + 161 * SCALE, 160 * SCALE, 60 * SCALE);
    }

    async drawStatistics(
        snapshotStatistics?: SnapshotStatistics,
        emulationStatistics?: EmulationStatistics
    ): Promise<void> {
        if (!this.ctx) return;

        await this.drawSilkscreen();

        this.ctx.beginPath();
        this.fillRect(0, 161, 160, 60, 'rgba(255,255,255,0.6)');

        this.ctx.font = `${SCALE * 6}px monospace`;
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
        ].forEach((line, i) => this.ctx!.fillText(line, BORDER + 1 * SCALE, BORDER + (168 + i * 6) * SCALE));
    }

    async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        if (!this.ctx) return;

        this.ctx.drawImage(await this.buttonsImage(), BORDER, BORDER + 221 * SCALE, 160 * SCALE, 30 * SCALE);

        if (activeButtons.includes(PalmButton.cal)) this.fillRect(0, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.phone)) this.fillRect(30, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.todo)) this.fillRect(100, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.notes)) this.fillRect(130, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.up)) this.fillRect(60, 221, 40, 15, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.down)) this.fillRect(60, 236, 40, 15, BACKGROUND_ACTIVE_BUTTON);
    }

    drawEmulationCanvas(canvas: HTMLCanvasElement): void {
        if (!this.ctx) return;

        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(canvas, BORDER, BORDER, SCALE * 160, SCALE * 160);
    }

    private fillRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(BORDER + SCALE * x, BORDER + SCALE * y, SCALE * width, SCALE * height);
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

            default:
                return IMAGE_SILKSCREEN_DEFAULT;
        }
    }

    private buttonsImage(): Promise<HTMLCanvasElement> {
        switch (this.session?.device) {
            case DeviceId.m515:
                return IMAGE_BUTTONS_M515;

            default:
                return IMAGE_BUTTONS_DEFAULT;
        }
    }

    private backgroundColor(): string {
        switch (this.session?.device) {
            case DeviceId.m515:
                return BACKGROUND_COLOR_M515;

            default:
                return BACKGROUND_COLOR_DEFAULT;
        }
    }

    private ctx: CanvasRenderingContext2D | undefined;
    private session: Session | undefined;
}
