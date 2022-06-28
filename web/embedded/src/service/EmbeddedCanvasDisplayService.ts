import { AbstractCanvasDisplayService } from '@common/service/AbstractCanvasDisplayService';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';

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

    protected getDeviceId(): DeviceId {
        return this.deviceId;
    }

    protected getOrientation(): DeviceOrientation {
        return this.orientation;
    }

    private deviceId: DeviceId = DeviceId.m515;
    private orientation: DeviceOrientation = DeviceOrientation.portrait;
}
