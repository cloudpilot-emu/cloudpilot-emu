import { lazyLoadImage, prerender } from '@common/helper/image';

import { AbstractCanvasDisplayService } from '@common/service/AbstractCanvasDisplayService';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { PalmButton } from '@native/cloudpilot_web';
import { svgToUrl } from './../../../common/helper/image';

const GAME_ICON_SVG =
    '<svg xmlns="http://www.w3.org/2000/svg" class="ionicon" viewBox="0 0 512 512"><title>Game Controller</title><path d="M467.51 248.83c-18.4-83.18-45.69-136.24-89.43-149.17A91.5 91.5 0 00352 96c-26.89 0-48.11 16-96 16s-69.15-16-96-16a99.09 99.09 0 00-27.2 3.66C89 112.59 61.94 165.7 43.33 248.83c-19 84.91-15.56 152 21.58 164.88 26 9 49.25-9.61 71.27-37 25-31.2 55.79-40.8 119.82-40.8s93.62 9.6 118.66 40.8c22 27.41 46.11 45.79 71.42 37.16 41.02-14.01 40.44-79.13 21.43-165.04z" fill="none" stroke="currentColor" stroke-miterlimit="10" stroke-width="32"/><circle cx="292" cy="224" r="20"/><path d="M336 288a20 20 0 1120-19.95A20 20 0 01336 288z"/><circle cx="336" cy="180" r="20"/><circle cx="380" cy="224" r="20"/><path fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="32" d="M160 176v96M208 224h-96"/></svg>';

const GAME_ICON_PRERENDERED = prerender(lazyLoadImage(svgToUrl(GAME_ICON_SVG)), GAME_ICON_SVG);

export class EmbeddedCanvasDisplayService extends AbstractCanvasDisplayService {
    async initialize(canvas?: HTMLCanvasElement, deviceId = this.deviceId, orientation = this.orientation) {
        this.deviceId = deviceId;
        this.orientation = orientation;

        const theCanvas = canvas || this.ctx?.canvas;
        if (theCanvas) {
            theCanvas.width = this.width;
            theCanvas.height = this.height;

            theCanvas.style.objectFit = 'contain';
        }

        await this.initWithCanvas(canvas);
    }

    updateOrientation(orientation: DeviceOrientation) {
        this.orientation = orientation;
        this.initialize();
    }

    toggleGameModeIndicator(gameModeActive: boolean): void {
        this.gameModeActive = gameModeActive;

        this.drawButtons(this.lastActiveButtons);
    }

    setGameModeIndicatorEnabled(gameModeIndicatorEnabled: boolean) {
        this.gameModeIndicatorEnabled = gameModeIndicatorEnabled;

        this.drawButtons(this.lastActiveButtons);
    }

    isGameModeIndicatorEnabled(): boolean {
        return this.gameModeIndicatorEnabled;
    }

    getCanvas(): HTMLCanvasElement | undefined {
        return this.ctx?.canvas;
    }

    override async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        this.lastActiveButtons = activeButtons;

        await super.drawButtons(activeButtons);

        if (this.gameModeActive && this.gameModeIndicatorEnabled) {
            await this.drawGameModeIndicator();
        }
    }

    protected getDeviceId(): DeviceId {
        return this.deviceId;
    }

    protected getOrientation(): DeviceOrientation {
        return this.orientation;
    }

    private async drawGameModeIndicator(): Promise<void> {
        if (!this.ctx) return;

        const indicatorSize = Math.floor(this.layout.screenWidth.frameCanvas * 0.1);
        const iconSize = Math.floor(indicatorSize * 0.8);
        const padding = Math.floor(indicatorSize * 0.1);
        const iconPadding = Math.floor(indicatorSize * 0.11);

        this.fillCanvasRect(
            this.layout.screenLeft.frameCanvas - padding + this.layout.screenWidth.frameCanvas - indicatorSize,
            this.layout.buttonTop.frameCanvas + this.layout.buttonHeight.frameCanvas - indicatorSize - padding,
            indicatorSize,
            indicatorSize,
            'rgba(200, 200, 200, 0.7)'
        );

        const icon = await GAME_ICON_PRERENDERED(indicatorSize, indicatorSize);

        this.ctx.drawImage(
            await GAME_ICON_PRERENDERED(indicatorSize, indicatorSize),
            this.layout.screenLeft.frameCanvas +
                this.layout.screenWidth.frameCanvas -
                padding -
                indicatorSize +
                iconPadding,
            this.layout.buttonTop.frameCanvas +
                this.layout.buttonHeight.frameCanvas -
                indicatorSize -
                padding +
                iconPadding,
            iconSize,
            iconSize
        );
    }

    private deviceId: DeviceId = DeviceId.m515;
    private orientation: DeviceOrientation = DeviceOrientation.portrait;

    private gameModeIndicatorEnabled = true;
    private gameModeActive = false;
    private lastActiveButtons: Array<PalmButton> | undefined = undefined;
}
