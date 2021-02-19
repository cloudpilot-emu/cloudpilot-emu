import { AfterViewInit, Component, ElementRef, OnInit, ViewChild } from '@angular/core';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor() {}

    ngAfterViewInit(): void {
        // tslint:disable-next-line: no-non-null-assertion
        const ctx = this.canvasRef.nativeElement.getContext('2d')!;

        ctx.fillStyle = 'green';
        ctx.rect(0, 0, this.canvasWidth, this.canvasHeight);
        ctx.fill();
    }

    private get canvasWidth(): number {
        return 3 * 160;
    }

    private get canvasHeight(): number {
        return 3 * 250;
    }

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
}
