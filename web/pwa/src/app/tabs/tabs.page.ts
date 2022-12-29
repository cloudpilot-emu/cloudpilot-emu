import { CanvasDisplayService } from '@pwa/service/canvas-display.service';
import { Component } from '@angular/core';
import { EmulationStateService } from '@pwa/service/emulation-state.service';
import { Router } from '@angular/router';

@Component({
    selector: 'app-tabs',
    templateUrl: 'tabs.page.html',
    styleUrls: ['tabs.page.scss'],
})
export class TabsPage {
    constructor(
        private router: Router,
        private canvasDisplayService: CanvasDisplayService,
        private emulationStateService: EmulationStateService
    ) {
        window.addEventListener('resize', this.updateUseSmallUI);
        window.addEventListener('orientationchange', this.updateUseSmallUI);
        canvasDisplayService.onResize.addHandler(() => this.updateUseSmallUI());

        this.updateUseSmallUI();
    }
    get smallUI(): boolean {
        return (
            this.router.url === '/tab/emulation' &&
            this.useSmallUI &&
            this.emulationStateService.getCurrentSession() !== undefined
        );
    }

    lock(): void {
        this.isLockedFlag = true;
    }

    unlock(): void {
        this.isLockedFlag = false;
    }

    isLocked(): boolean {
        return this.isLockedFlag;
    }

    private updateUseSmallUI = (): void => {
        const isMD = !!document.querySelector('html.md');
        const headerHeight = isMD ? 56 : 44;
        const tabbarHeight = isMD ? 57 : 51;
        const canvasPadding = 6;

        this.useSmallUI =
            this.canvasArea(
                window.innerHeight - headerHeight - tabbarHeight - canvasPadding,
                window.innerWidth - canvasPadding
            ) <
            this.canvasArea(window.innerHeight - headerHeight - 25 - canvasPadding, window.innerWidth - canvasPadding);
    };

    private canvasArea(availableHeight: number, availableWidth: number): number {
        availableHeight = Math.min(availableHeight, this.canvasDisplayService.height / devicePixelRatio);
        availableWidth = Math.min(availableWidth, this.canvasDisplayService.width / devicePixelRatio);

        let scaledWidth: number;
        let scaledHeight: number;

        if (availableWidth / availableHeight > this.canvasDisplayService.width / this.canvasDisplayService.height) {
            scaledHeight = availableHeight;
            scaledWidth = (availableHeight / this.canvasDisplayService.height) * this.canvasDisplayService.width;
        } else {
            scaledWidth = availableWidth;
            scaledHeight = (availableWidth / this.canvasDisplayService.width) * this.canvasDisplayService.height;
        }

        return scaledWidth * scaledHeight;
    }

    private useSmallUI = false;
    private isLockedFlag = false;
}
