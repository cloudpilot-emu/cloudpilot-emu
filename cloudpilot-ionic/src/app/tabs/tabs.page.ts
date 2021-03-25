import { HEIGHT, SCALE, WIDTH } from './../page/emulation/helper/CanvasHelper';

import { Component } from '@angular/core';
import { Router } from '@angular/router';

function canvasArea(availableHeight: number, availableWidth: number): number {
    availableHeight = Math.min(availableHeight, HEIGHT / devicePixelRatio);
    availableWidth = Math.min(availableWidth, WIDTH / devicePixelRatio);

    let scaledWidth: number;
    let scaledHeight: number;

    if (availableWidth / availableHeight > WIDTH / HEIGHT) {
        scaledHeight = availableHeight;
        scaledWidth = (availableHeight / HEIGHT) * WIDTH;
    } else {
        scaledWidth = availableWidth;
        scaledHeight = (availableWidth / WIDTH) * HEIGHT;
    }

    return scaledWidth * scaledHeight;
}

@Component({
    selector: 'app-tabs',
    templateUrl: 'tabs.page.html',
    styleUrls: ['tabs.page.scss'],
})
export class TabsPage {
    constructor(private router: Router) {
        window.addEventListener('resize', this.updateUseSmallUI);

        this.updateUseSmallUI();
    }

    get smallUI(): boolean {
        return this.router.url === '/tab/emulation' && this.useSmallUI;
    }

    private updateUseSmallUI = (): void => {
        const isMD = !!document.querySelector('html.md');
        const headerHeight = isMD ? 56 : 44;
        const tabbarHeight = isMD ? 57 : 51;
        const canvasPadding = 6;

        this.useSmallUI =
            canvasArea(window.innerHeight - headerHeight - tabbarHeight - canvasPadding, window.innerWidth - 6) <
            canvasArea(window.innerHeight - headerHeight - 25 - canvasPadding, window.innerWidth - 6);
    };

    private useSmallUI = false;
}
