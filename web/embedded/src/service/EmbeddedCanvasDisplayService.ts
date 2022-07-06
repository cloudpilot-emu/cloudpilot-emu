import { AbstractCanvasDisplayService } from '@common/service/AbstractCanvasDisplayService';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { PalmButton } from '@native/cloudpilot_web';

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

    override async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        this.lastActiveButtons = activeButtons;

        await super.drawButtons(activeButtons);

        console.log(this.gameModeActive, this.gameModeIndicatorEnabled);
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

    private drawGameModeIndicator(): void {
        if (!this.ctx) return;

        const indicatorSize = Math.floor(this.layout.screenWidth.frameCanvas * 0.1);
        const padding = Math.floor(indicatorSize * 0.1);

        // this.ctx.shadowBlur = indicatorSize * 2;
        // this.ctx.shadowColor = 'white';

        this.fillCanvasRect(
            this.layout.screenLeft.frameCanvas + padding,
            this.layout.buttonTop.frameCanvas + this.layout.buttonHeight.frameCanvas - indicatorSize - padding,
            indicatorSize,
            indicatorSize,
            'rgba(255, 255, 255, 0.8)'
        );
    }

    private deviceId: DeviceId = DeviceId.m515;
    private orientation: DeviceOrientation = DeviceOrientation.portrait;

    private gameModeIndicatorEnabled = true;
    private gameModeActive = false;
    private lastActiveButtons: Array<PalmButton> | undefined = undefined;
}
