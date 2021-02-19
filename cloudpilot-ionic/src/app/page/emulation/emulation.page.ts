import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, OnInit, ViewChild } from '@angular/core';

import { EmulationService } from './../../service/emulation.service';

const SCALE = 3 * devicePixelRatio;
const SILKSCREEN_URL = 'assets/silkscreen.png';

function textCenteredAt(ctx: CanvasRenderingContext2D, x: number, y: number, text: string): void {
    const metrics = ctx.measureText(text);

    ctx.textBaseline = 'middle';
    ctx.fillText(text, x - metrics.width / 2, y);
}

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor(private emulationService: EmulationService, private ngZone: NgZone) {}

    ngAfterViewInit(): void {
        const canvasElt = this.canvasRef.nativeElement;
        // tslint:disable-next-line: no-non-null-assertion
        const ctx = canvasElt.getContext('2d')!;

        ctx.imageSmoothingEnabled = false;

        ctx.beginPath();
        ctx.rect(0, 0, 160 * SCALE, 160 * SCALE);
        ctx.fillStyle = '#ccc';
        ctx.fill();

        this.drawSilkscreen();
        this.drawButtons();

        this.ngZone.runOutsideAngular(() => {
            canvasElt.addEventListener('mousedown', this.handeMouseEvent);
            canvasElt.addEventListener('mouseup', this.handeMouseEvent);
            canvasElt.addEventListener('mousemove', this.handeMouseEvent);
            canvasElt.addEventListener('touchstart', this.handleTouchEvent);
            canvasElt.addEventListener('touchmove', this.handleTouchEvent);
            canvasElt.addEventListener('touchend', this.handleTouchEvent);
            canvasElt.addEventListener('touchcancel', this.handleTouchEvent);
        });
    }

    get canvasWidth(): number {
        return SCALE * 160;
    }

    get canvasHeight(): number {
        return SCALE * 250;
    }

    handeMouseEvent = (e: MouseEvent): void => {
        const coords = this.eventToPalmCoordinates(e);

        // tslint:disable-next-line: no-bitwise
        if (e.buttons & 0x01) {
            if (coords) this.emulationService.handlePointerMove(...coords);
        } else {
            this.emulationService.handlePointerUp();
        }
    };

    handleTouchEvent = (e: TouchEvent): void => {
        if (e.touches.length > 0) {
            // tslint:disable-next-line: no-non-null-assertion
            const coords = this.eventToPalmCoordinates(e.touches.item(0)!);

            if (coords) this.emulationService.handlePointerMove(...coords);
        } else {
            this.emulationService.handlePointerUp();
        }

        e.preventDefault();
    };

    private ionViewDidEnter() {
        this.emulationService.newFrame.addHandler(this.onNewFrame);

        this.emulationService.resume();
    }

    private ionViewWillLeave() {
        this.emulationService.pause();

        this.emulationService.newFrame.removeHandler(this.onNewFrame);
    }

    private onNewFrame = (canvas: HTMLCanvasElement): void => {
        // tslint:disable-next-line: no-non-null-assertion
        this.canvasRef.nativeElement.getContext('2d')!.drawImage(canvas, 0, 0, SCALE * 160, SCALE * 160);
    };

    private async drawSilkscreen(): Promise<void> {
        const image = new Image();

        await new Promise<void>((resolve, reject) => {
            image.onload = () => resolve();
            image.onerror = () => reject();

            image.src = SILKSCREEN_URL;
        });

        // tslint:disable-next-line: no-non-null-assertion
        const ctx = this.canvasRef.nativeElement.getContext('2d')!;

        ctx.beginPath();
        ctx.rect(0, 160 * SCALE, 160 * SCALE, 60 * SCALE);
        ctx.fillStyle = '#eee';
        ctx.fill();

        ctx.imageSmoothingEnabled = true;
        ctx.imageSmoothingQuality = 'high';

        ctx.drawImage(image, 0, 160 * SCALE, 160 * SCALE, 60 * SCALE);

        ctx.imageSmoothingEnabled = false;
    }

    private drawButtons(): void {
        // tslint:disable-next-line: no-non-null-assertion
        const ctx = this.canvasRef.nativeElement.getContext('2d')!;

        ctx.beginPath();
        ctx.rect(0, 220 * SCALE, 160 * SCALE, 30 * SCALE);
        ctx.fillStyle = 'white';
        ctx.fill();

        ctx.beginPath();
        ctx.lineWidth = 3;
        ctx.strokeStyle = 'black';
        ctx.moveTo(0, 220 * SCALE);
        ctx.lineTo(160 * SCALE, 220 * SCALE);
        ctx.moveTo(30 * SCALE, 220 * SCALE);
        ctx.lineTo(30 * SCALE, 250 * SCALE);
        ctx.moveTo(60 * SCALE, 220 * SCALE);
        ctx.lineTo(60 * SCALE, 250 * SCALE);
        ctx.moveTo(130 * SCALE, 220 * SCALE);
        ctx.lineTo(130 * SCALE, 250 * SCALE);
        ctx.moveTo(100 * SCALE, 220 * SCALE);
        ctx.lineTo(100 * SCALE, 250 * SCALE);
        ctx.moveTo(60 * SCALE, 235 * SCALE);
        ctx.lineTo(100 * SCALE, 235 * SCALE);
        ctx.stroke();

        ctx.font = `${10 * SCALE}px sans`;
        ctx.fillStyle = 'black';
        textCenteredAt(ctx, 15 * SCALE, 235 * SCALE, 'D');
        textCenteredAt(ctx, 45 * SCALE, 235 * SCALE, 'P');
        textCenteredAt(ctx, 115 * SCALE, 235 * SCALE, 'T');
        textCenteredAt(ctx, 145 * SCALE, 235 * SCALE, 'N');
    }

    private eventToPalmCoordinates(e: MouseEvent | Touch): [number, number] | undefined {
        const canvasElt = this.canvasRef.nativeElement;
        const boundingBox = canvasElt.getBoundingClientRect();

        const x = Math.floor(((e.clientX - boundingBox.left - 5) / (boundingBox.width - 10)) * 160);
        const y = Math.floor(((e.clientY - boundingBox.top - 5) / (boundingBox.height - 10)) * 250);

        if (x < 0 || x >= 160 || y < 0 || y >= 220) return undefined;

        return [x, y];
    }

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
}
