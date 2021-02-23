import { PalmButton } from '../../../../../../src';

const SILKSCREEN_URL = 'assets/silkscreen.svg';
const BACKGROUND_COLOR_SILKSCREEN = '#bbb';
const BACKGROUND_COLOR = '#d2d2d2';
const BACKGROUND_ACTIVE_BUTTON = '#777';

export const SCALE = 3 * devicePixelRatio;

const IMAGE_SILKSCREEN = new Promise<HTMLImageElement>((resolve, reject) => {
    const image = new Image();

    image.onload = () => resolve(image);
    image.onerror = () => reject();

    image.src = SILKSCREEN_URL;
});

export class CanvasHelper {
    constructor(private canvas: HTMLCanvasElement) {
        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        this.ctx = ctx;
    }

    async clear(): Promise<void> {
        this.fillRect(0, 0, 160, 250, BACKGROUND_COLOR);

        await this.drawSilkscreen();

        this.drawButtons();
    }

    async drawSilkscreen(): Promise<void> {
        const image = await IMAGE_SILKSCREEN;

        this.fillRect(0, 160, 160, 60, BACKGROUND_COLOR_SILKSCREEN);

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(image, 0, 160 * SCALE, 160 * SCALE, 60 * SCALE);
    }

    drawButtons(activeButtons: Array<PalmButton> = []): void {
        this.fillRect(0, 220, 160, 30, BACKGROUND_COLOR);

        if (activeButtons.includes(PalmButton.cal)) this.fillRect(0, 220, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.phone)) this.fillRect(30, 220, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.todo)) this.fillRect(100, 220, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.notes)) this.fillRect(130, 220, 30, 30, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.up)) this.fillRect(60, 220, 40, 15, BACKGROUND_ACTIVE_BUTTON);
        if (activeButtons.includes(PalmButton.down)) this.fillRect(60, 235, 40, 15, BACKGROUND_ACTIVE_BUTTON);

        this.ctx.beginPath();
        this.ctx.lineWidth = Math.round(0.5 * SCALE);
        this.ctx.strokeStyle = 'black';
        [
            [0, 220, 160, 220],
            [30, 220, 30, 250],
            [60, 220, 60, 250],
            [130, 220, 130, 250],
            [100, 220, 100, 250],
            [60, 235, 100, 235],
        ].forEach(([x0, y0, x1, y1]) => {
            this.ctx.moveTo(SCALE * x0, SCALE * y0);
            this.ctx.lineTo(SCALE * x1, SCALE * y1);
        });
        this.ctx.stroke();

        this.ctx.font = `${10 * SCALE}px sans`;
        this.ctx.fillStyle = 'black';

        this.textCenteredAt(15 * SCALE, 235 * SCALE, 'D');
        this.textCenteredAt(45 * SCALE, 235 * SCALE, 'P');
        this.textCenteredAt(115 * SCALE, 235 * SCALE, 'T');
        this.textCenteredAt(145 * SCALE, 235 * SCALE, 'N');
    }

    drawEmulationCanvas(canvas: HTMLCanvasElement): void {
        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(canvas, 0, 0, SCALE * 160, SCALE * 160);
    }

    private fillRect(x: number, y: number, width: number, height: number, style: string): void {
        this.ctx.beginPath();
        this.ctx.rect(SCALE * x, SCALE * y, SCALE * width, SCALE * height);
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    private textCenteredAt(x: number, y: number, text: string): void {
        const metrics = this.ctx.measureText(text);

        this.ctx.textBaseline = 'middle';
        this.ctx.fillText(text, x - metrics.width / 2, y);
    }

    private ctx: CanvasRenderingContext2D;
}
