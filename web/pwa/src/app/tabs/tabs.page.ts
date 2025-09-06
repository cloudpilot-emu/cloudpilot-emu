import { Component } from '@angular/core';
import { Router } from '@angular/router';

import { CanvasDisplayService } from '@pwa/service/canvas-display.service';
import { EmulationStateService } from '@pwa/service/emulation-state.service';

function getHeight(selector: string): number | undefined {
    const elts = document.querySelectorAll(selector);

    for (let i = 0; i < elts.length; i++) {
        const bb = elts[i].getBoundingClientRect();

        if (bb.height > 0) return bb.height;
    }

    return 0;
}

@Component({
    selector: 'app-tabs',
    templateUrl: 'tabs.page.html',
    styleUrls: ['tabs.page.scss'],
    standalone: false,
})
export class TabsPage {
    constructor(
        private router: Router,
        private canvasDisplayService: CanvasDisplayService,
        private emulationStateService: EmulationStateService,
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
            this.emulationStateService.currentSession() !== undefined
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
        const headerHeight = getHeight('ion-header');
        const tabbarHeight = isMD ? 56 : 50;
        const canvasPadding = 6;

        this.useSmallUI =
            headerHeight !== undefined &&
            this.canvasArea(
                window.innerHeight - headerHeight - tabbarHeight - canvasPadding,
                window.innerWidth - canvasPadding,
            ) <
                this.canvasArea(
                    window.innerHeight - headerHeight - 25 - canvasPadding,
                    window.innerWidth - canvasPadding,
                );
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
