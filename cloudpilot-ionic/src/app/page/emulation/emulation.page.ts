import { AfterViewInit, Component, ElementRef, OnInit, ViewChild } from '@angular/core';

import { EmulationService } from './../../service/emulation.service';

const SCALE = 3;
const SILKSCREEN_URL = 'assets/silkscreen.png';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor(private emulationService: EmulationService) {}

    ngAfterViewInit(): void {
        const canvasElt = this.canvasRef.nativeElement;
        // tslint:disable-next-line: no-non-null-assertion
        const ctx = canvasElt.getContext('2d')!;

        ctx.fillStyle = '#aaa';
        ctx.rect(0, 0, this.canvasWidth, this.canvasHeight);
        ctx.fill();

        ctx.imageSmoothingEnabled = false;

        this.drawSilkscreen();
    }

    get canvasWidth(): number {
        return SCALE * 160;
    }

    get canvasHeight(): number {
        return SCALE * 250;
    }

    handeMouseEvent(e: MouseEvent): void {
        const coords = this.eventToPalmCoordinates(e);

        // tslint:disable-next-line: no-bitwise
        if (e.buttons & 0x01) {
            if (coords) this.emulationService.handlePointerMove(...coords);
        } else {
            this.emulationService.handlePointerUp();
        }
    }

    handleTouchEvent(e: TouchEvent): void {
        if (e.touches.length > 0) {
            // tslint:disable-next-line: no-non-null-assertion
            const coords = this.eventToPalmCoordinates(e.touches.item(0)!);

            if (coords) this.emulationService.handlePointerMove(...coords);
        } else {
            this.emulationService.handlePointerUp();
        }

        e.preventDefault();
    }

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

        ctx.imageSmoothingEnabled = true;
        ctx.imageSmoothingQuality = 'high';

        ctx.drawImage(image, 0, 160 * SCALE, 160 * SCALE, 60 * SCALE);

        ctx.imageSmoothingEnabled = false;
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
