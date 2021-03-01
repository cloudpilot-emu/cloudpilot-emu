import { DeviceId } from 'src/app/model/DeviceId';
import { EmulationStateService } from 'src/app/service/emulation-state.service';
import { GRAYSCALE_PALETTE_HEX } from './../../../service/emulation.service';
import { PalmButton } from '../../../../../../src';
import { Session } from './../../../model/Session';

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

export class CanvasHelper {
    constructor(private canvas: HTMLCanvasElement) {
        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        this.ctx = ctx;
    }

    async clear(session: Session | undefined): Promise<void> {
        this.session = session;
        this.fillCanvasRect(0, 0, WIDTH, HEIGHT, this.backgroundColor());

        await this.drawSilkscreen();
        await this.drawButtons();
    }

    async drawSilkscreen(): Promise<void> {
        const image = await this.silkscreenImage();

        this.fillRect(0, 161, 160, 60, BACKGROUND_COLOR_SILKSCREEN);

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(image, BORDER, BORDER + 161 * SCALE, 160 * SCALE, 60 * SCALE);
    }

    async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        this.ctx.drawImage(await this.buttonsImage(), BORDER, BORDER + 221 * SCALE, 160 * SCALE, 30 * SCALE);

        if (activeButtons.includes(PalmButton.cal)) this.fillRect(0, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.phone)) this.fillRect(30, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.todo)) this.fillRect(100, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.notes)) this.fillRect(130, 221, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.up)) this.fillRect(60, 221, 40, 15, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.down)) this.fillRect(60, 236, 40, 15, BACKGROUND_ACTIVE_BUTTON);
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

    private ctx: CanvasRenderingContext2D;
    private session: Session | undefined;
}
