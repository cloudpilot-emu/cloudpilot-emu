import { EmulationService, GRAYSCALE_PALETTE_HEX } from './../../../service/emulation.service';

import { DeviceId } from 'src/app/model/DeviceId';
import { PalmButton } from '../../../../../../src';

const URL_SILKSCREEN_DEFAULT = 'assets/silkscreen-default.svg';
const URL_SILKSCREEN_M515 = 'assets/silkscreen-m515.svg';
const BACKGROUND_COLOR_SILKSCREEN = GRAYSCALE_PALETTE_HEX[2];
const BACKGROUND_COLOR_DEFAULT = GRAYSCALE_PALETTE_HEX[0];
const BACKGROUND_COLOR_M515 = 'white';
const BACKGROUND_ACTIVE_BUTTON = GRAYSCALE_PALETTE_HEX[6];

export const SCALE = 3 * devicePixelRatio;
export const BORDER = Math.round(1 * SCALE);
export const WIDTH = SCALE * 160 + 2 * BORDER;
export const HEIGHT = SCALE * 252 + 2 * BORDER;

function loadImage(url: string): Promise<HTMLImageElement> {
    return new Promise<HTMLImageElement>((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}

const IMAGE_SILKSCREEN_DEFAULT = loadImage(URL_SILKSCREEN_DEFAULT);
const IMAGE_SILKSCREEN_M515 = loadImage(URL_SILKSCREEN_M515);

export class CanvasHelper {
    constructor(private canvas: HTMLCanvasElement, private emulationService: EmulationService) {
        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        this.ctx = ctx;
    }

    async clear(): Promise<void> {
        this.fillCanvasRect(0, 0, WIDTH, HEIGHT, this.backgroundColor());

        await this.drawSilkscreen();

        this.drawButtons();
    }

    async drawSilkscreen(): Promise<void> {
        const image = await this.silkscreenImage();

        this.fillRect(0, 161, 160, 60, BACKGROUND_COLOR_SILKSCREEN);

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(image, BORDER, BORDER + 161 * SCALE, 160 * SCALE, 60 * SCALE);
    }

    drawButtons(activeButtons: Array<PalmButton> = []): void {
        this.fillRect(0, 222, 160, 30, this.backgroundColor());

        if (activeButtons.includes(PalmButton.cal)) this.fillRect(0, 222, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.phone)) this.fillRect(30, 222, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.todo)) this.fillRect(100, 222, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.notes)) this.fillRect(130, 222, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.up)) this.fillRect(60, 222, 40, 15, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.down)) this.fillRect(60, 237, 40, 15, BACKGROUND_ACTIVE_BUTTON);

        this.ctx.beginPath();
        this.ctx.lineWidth = Math.round(0.5 * SCALE);
        this.ctx.strokeStyle = BACKGROUND_ACTIVE_BUTTON;
        [
            [30, 222, 30, 252],
            [60, 222, 60, 252],
            [130, 222, 130, 252],
            [100, 222, 100, 252],
            [60, 237, 100, 237],
        ].forEach(([x0, y0, x1, y1]) => {
            this.ctx.moveTo(BORDER + SCALE * x0, BORDER + SCALE * y0);
            this.ctx.lineTo(BORDER + SCALE * x1, BORDER + SCALE * y1);
        });
        this.ctx.stroke();

        this.ctx.font = `${10 * SCALE}px sans`;
        this.ctx.fillStyle = 'black';

        this.textCenteredAt(15, 237, 'D');
        this.textCenteredAt(45, 237, 'P');
        this.textCenteredAt(115, 237, 'T');
        this.textCenteredAt(145, 237, 'N');
    }

    drawEmulationCanvas(canvas: HTMLCanvasElement): void {
        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(canvas, BORDER, BORDER, SCALE * 160, SCALE * 160);
    }

    private fillRect(x: number, y: number, width: number, height: number, style: string): void {
        this.ctx.beginPath();
        this.ctx.rect(BORDER + SCALE * x, BORDER + SCALE * y, SCALE * width, SCALE * height);
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    private fillCanvasRect(x: number, y: number, width: number, height: number, style: string): void {
        this.ctx.beginPath();
        this.ctx.rect(x, y, width, height);
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    private textCenteredAt(x: number, y: number, text: string): void {
        const metrics = this.ctx.measureText(text);

        this.ctx.textBaseline = 'middle';
        this.ctx.fillText(text, BORDER + SCALE * x - metrics.width / 2, BORDER + SCALE * y);
    }

    private silkscreenImage(): Promise<HTMLImageElement> {
        switch (this.emulationService.getCurrentSession()?.device) {
            case DeviceId.m515:
                return IMAGE_SILKSCREEN_M515;

            default:
                return IMAGE_SILKSCREEN_DEFAULT;
        }
    }

    private backgroundColor(): string {
        switch (this.emulationService.getCurrentSession()?.device) {
            case DeviceId.m515:
                return BACKGROUND_COLOR_M515;

            default:
                return BACKGROUND_COLOR_DEFAULT;
        }
    }

    private ctx: CanvasRenderingContext2D;
}
