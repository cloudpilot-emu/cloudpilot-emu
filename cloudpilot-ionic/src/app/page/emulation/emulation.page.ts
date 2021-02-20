import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { CanvasHelper, SCALE } from './helper/CanvasHelper';

import { EmulationService } from './../../service/emulation.service';
import { EventHandler } from './helper/EventHandler';
import { PalmButton } from '../../../../../src';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit, OnDestroy {
    constructor(private emulationService: EmulationService, private ngZone: NgZone) {}

    ngOnDestroy(): void {
        if (this.eventHandler) this.eventHandler.release();
    }

    ngAfterViewInit(): void {
        const canvasElt = this.canvasRef.nativeElement;
        this.canvasHelper = new CanvasHelper(canvasElt);
        this.eventHandler = new EventHandler(canvasElt, this.emulationService, this.canvasHelper);

        this.canvasHelper.clear();

        this.ngZone.runOutsideAngular(() => this.eventHandler.bind());
    }

    get canvasWidth(): number {
        return SCALE * 160;
    }

    get canvasHeight(): number {
        return SCALE * 250;
    }

    async powerButtonDown(): Promise<void> {
        this.emulationService.handleButtonDown(PalmButton.power);
    }

    async powerButtonUp(): Promise<void> {
        this.emulationService.handleButtonUp(PalmButton.power);
    }

    ionViewDidEnter() {
        this.emulationService.newFrame.addHandler(this.onNewFrame);

        this.emulationService.resume();
    }

    ionViewWillLeave() {
        this.emulationService.pause();

        this.emulationService.newFrame.removeHandler(this.onNewFrame);
    }

    private onNewFrame = (canvas: HTMLCanvasElement): void => {
        this.canvasHelper.drawEmulationCanvas(canvas);
    };

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
    private canvasHelper!: CanvasHelper;
    private eventHandler!: EventHandler;
}
