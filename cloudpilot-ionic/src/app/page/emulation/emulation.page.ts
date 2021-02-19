import { AfterViewInit, Component, ElementRef, OnInit, ViewChild } from '@angular/core';

import { EmulationService } from './../../service/emulation.service';

const SCALE = 3;
@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor(private emulationService: EmulationService) {}

    ngAfterViewInit(): void {
        // tslint:disable-next-line: no-non-null-assertion
        const ctx = this.canvasRef.nativeElement.getContext('2d')!;
        ctx.imageSmoothingEnabled = false;

        ctx.fillStyle = '#aaa';
        ctx.rect(0, 0, this.canvasWidth, this.canvasHeight);
        ctx.fill();
    }

    get canvasWidth(): number {
        return SCALE * 160;
    }

    get canvasHeight(): number {
        return SCALE * 250;
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

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
}
