import { AfterViewInit, Component, ElementRef, OnInit, ViewChild } from '@angular/core';

import { EmulationService } from './../../service/emulation.service';

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

        ctx.fillStyle = 'green';
        ctx.rect(0, 0, this.canvasWidth, this.canvasHeight);
        ctx.fill();
    }

    get canvasWidth(): number {
        return 3 * 160;
    }

    get canvasHeight(): number {
        return 3 * 250;
    }

    private ionViewDidEnter() {
        this.emulationService.resume();
    }

    private ionViewWillLeave() {
        this.emulationService.pause();
    }

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
}
