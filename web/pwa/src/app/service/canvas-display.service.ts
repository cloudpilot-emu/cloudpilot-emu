import { AbstractCanvasDisplayService, DEFAULT_DEVICE } from '@common/service/AbstractCanvasDisplayService';

import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { Injectable } from '@angular/core';
import { ScreenSize } from '@common/model/Dimensions';
import { Session } from '@pwa/model/Session';
import { SkinLoader } from '@common/service/SkinLoader';
import { SnapshotStatistics } from '@pwa/model/SnapshotStatistics';
import { deviceDimensions } from '@common/helper/deviceProperties';

function fontScaleForScreenSize(screenSize: ScreenSize) {
    switch (screenSize) {
        case ScreenSize.screen320x320:
        case ScreenSize.screen320x480:
            return 2;

        case ScreenSize.screen240x320:
            return 1.5;

        default:
            return 1;
    }
}

@Injectable({ providedIn: 'root' })
export class CanvasDisplayService extends AbstractCanvasDisplayService {
    constructor(private cloudpilotService: CloudpilotService) {
        super(new SkinLoader(cloudpilotService.cloudpilot));
    }

    async initialize(canvas?: HTMLCanvasElement, session: Session | undefined = this.session): Promise<void> {
        this.session = session;

        await this.initWithCanvas(canvas);
    }

    async clearStatistics(): Promise<void> {
        this.statisticsVisible = false;

        if (this.layout.silkscreenHeight.frameCanvas > 0) {
            await this.drawSilkscreen();
        } else {
            await this.drawEmulationCanvas();
        }
    }

    async updateStatistics(
        snapshotStatistics?: SnapshotStatistics,
        emulationStatistics?: EmulationStatistics
    ): Promise<void> {
        await this.clearStatistics();

        this.drawStatistics(snapshotStatistics, emulationStatistics);

        this.lastEmulationStatistics = emulationStatistics;
        this.lastSnapshotStatistics = snapshotStatistics;
        this.statisticsVisible = true;
    }

    protected getDeviceId(): DeviceId {
        return this.session?.device ?? DEFAULT_DEVICE;
    }

    protected getOrientation(): DeviceOrientation {
        return this.session?.deviceOrientation ?? DeviceOrientation.portrait;
    }

    protected override onAfterUpdateCanvas(): void {
        if (this.statisticsVisible && this.layout.silkscreenHeight.frameCanvas === 0) {
            this.drawStatistics(this.lastSnapshotStatistics, this.lastEmulationStatistics);
        }
    }

    private drawStatistics(snapshotStatistics?: SnapshotStatistics, emulationStatistics?: EmulationStatistics): void {
        if (!this.ctx) return;
        this.setupTransformation();

        const height =
            this.layout.silkscreenHeight.frameDevice > 0
                ? this.layout.silkscreenHeight
                : this.layout.softSilkscreenHeight;

        const top =
            this.layout.silkscreenHeight.frameCanvas > 0
                ? this.layout.silkscreenTop
                : {
                      frameCanvas: this.layout.screenBottom.frameCanvas - height.frameCanvas,
                      frameDevice: this.layout.screenBottom.frameDevice - height.frameDevice,
                  };

        this.ctx.beginPath();
        this.fillRect(
            this.layout.screenLeft.frameDevice,
            top.frameDevice,
            this.layout.screenWidth.frameDevice,
            height.frameDevice,
            'rgba(255,255,255,0.6)'
        );

        const screenSize = deviceDimensions(this.session?.device ?? DEFAULT_DEVICE).screenSize;
        const fontScale = fontScaleForScreenSize(screenSize);

        this.ctx.font = `${this.layout.scale * 6 * fontScale}px monospace`;
        this.ctx.fillStyle = 'black';
        [
            ...(!snapshotStatistics && !emulationStatistics ? ['collecting statistics...'] : []),
            ...(snapshotStatistics
                ? [
                      `last snapshot          : ${new Date(snapshotStatistics.timestamp).toLocaleTimeString()}`,
                      `snapshot pages         : ${snapshotStatistics.pages}`,
                      `snapshot time total    : ${snapshotStatistics.timeTotal.toFixed(2)} msec`,
                      `snapshot time blocking : ${snapshotStatistics.timeBlocking.toFixed(2)} msec`,
                  ]
                : []),
            ...(emulationStatistics
                ? [
                      `host speed             : ${emulationStatistics.hostSpeed.toFixed(2)}x`,
                      `emulation speed        : ${emulationStatistics.emulationSpeed.toFixed(2)}x`,
                      `average FPS            : ${emulationStatistics.averageFps.toFixed(2)}`,
                  ]
                : []),
        ].forEach((line, i) =>
            this.ctx!.fillText(
                line,
                this.layout.screenLeft.frameCanvas + fontScale * this.layout.scale,
                top.frameCanvas + fontScale * this.layout.scale * (8 + i * 6)
            )
        );
    }

    private session: Session | undefined;
    lastSnapshotStatistics?: SnapshotStatistics;
    lastEmulationStatistics?: EmulationStatistics;
    statisticsVisible = false;
}
