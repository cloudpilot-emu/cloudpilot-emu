import { Inject, Injectable, signal } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { deviceDimensions, hasDPad } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { ScreenSize } from '@common/model/Dimensions';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { AbstractCanvasDisplayService, DEFAULT_DEVICE } from '@common/service/AbstractCanvasDisplayService';
import { SkinLoader } from '@common/service/SkinLoader';
import { Mutex } from 'async-mutex';

import { Session } from '@pwa/model/Session';

import { KvsService } from './kvs.service';
import { TOKEN_CLOUDPILOT_INSTANCE } from './token';

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

class ScreenshotDisplayService extends AbstractCanvasDisplayService {
    constructor(
        private deviceId: DeviceId,
        private orientation: DeviceOrientation,
        private _pixelRatio: number,
        skinLoader: SkinLoader,
    ) {
        super(skinLoader);
    }

    setCanvas(canvas: HTMLCanvasElement): Promise<void> {
        return this.initWithCanvas(false, canvas);
    }

    protected override getDeviceId(): DeviceId {
        return this.deviceId;
    }

    protected override getOrientation(): DeviceOrientation {
        return this.orientation;
    }

    protected override pixelRatio(): number {
        return this._pixelRatio;
    }
}

@Injectable({ providedIn: 'root' })
export class CanvasDisplayService extends AbstractCanvasDisplayService {
    constructor(
        @Inject(TOKEN_CLOUDPILOT_INSTANCE) cloudpilotInstance: Promise<Cloudpilot>,
        private kvsService: KvsService,
    ) {
        super(new SkinLoader(cloudpilotInstance));
    }

    initialize = (canvas?: HTMLCanvasElement, session: Session | undefined = this.session): Promise<void> =>
        this.mutex.runExclusive(async () => {
            this.initializedSession.set(undefined);

            this.session = session;
            if (session?.device !== this.session?.device) this.lastEmulationCanvas = undefined;

            await this.initWithCanvas(
                this.session !== undefined
                    ? hasDPad(this.session.device) && !this.kvsService.kvs.dontEmulateDPad
                    : false,
                canvas,
            );

            this.initializedSession.set(this.session);
        });

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
        emulationStatistics?: EmulationStatistics,
    ): Promise<void> {
        await this.clearStatistics();

        this.drawStatistics(snapshotStatistics, emulationStatistics);

        this.lastEmulationStatistics = emulationStatistics;
        this.lastSnapshotStatistics = snapshotStatistics;
        this.statisticsVisible = true;
    }

    async screenshot(pixelRatio: number): Promise<Blob | undefined> {
        if (!this.ctx) return undefined;

        const screenshotDisplayService = new ScreenshotDisplayService(
            this.getDeviceId(),
            this.getOrientation(),
            pixelRatio,
            this.skinLoader,
        );

        const canvas = document.createElement('canvas');
        canvas.width = screenshotDisplayService.width;
        canvas.height = screenshotDisplayService.height;

        await screenshotDisplayService.setCanvas(canvas);
        screenshotDisplayService.updateEmulationCanvas(this.lastEmulationCanvas);

        return new Promise((resolve, reject) => {
            canvas.toBlob((blob) => (blob ? resolve(blob) : reject(new Error('unable to create screenshot'))));
        });
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
            'rgba(255,255,255,0.6)',
        );

        const screenSize = deviceDimensions(this.session?.device ?? DEFAULT_DEVICE).screenSize;
        const fontScale = fontScaleForScreenSize(screenSize);

        this.ctx.font = `${this.layout.scale * 6 * fontScale}px monospace`;
        this.ctx.fillStyle = 'black';
        [
            ...(!snapshotStatistics && !emulationStatistics ? ['collecting statistics...'] : []),
            ...(snapshotStatistics
                ? [
                      `last snapshot ${snapshotStatistics.success ? '        ' : '[failed]'} : ${new Date(snapshotStatistics.timestamp).toLocaleTimeString()}`,
                      `snapshot size          : ${snapshotStatistics.size >>> 10} kB`,
                      `snapshot time total    : ${snapshotStatistics.timeTotal.toFixed(2)} msec`,
                      `snapshot time blocking : ${snapshotStatistics.timeBlocking.toFixed(2)} msec`,
                  ]
                : []),
            ...(emulationStatistics?.type === 'cloudpilot'
                ? [
                      `host speed             : ${emulationStatistics.hostSpeed.toFixed(2)}x`,
                      `emulation speed        : ${emulationStatistics.emulationSpeed.toFixed(2)}x`,
                      `average FPS            : ${emulationStatistics.averageFps.toFixed(2)}`,
                  ]
                : []),
            ...(emulationStatistics?.type === 'uarm'
                ? [
                      `current MIPS           : ${emulationStatistics.currentSpeedMips.toFixed(2)}`,
                      `current MIPS limit     : ${emulationStatistics.currentMaxSpeedMips.toFixed(2)}`,
                      `current load           : ${((emulationStatistics.currentSpeedMips / emulationStatistics.currentMaxSpeedMips) * 100).toFixed(2)}%`,
                  ]
                : []),
        ].forEach((line, i) =>
            this.ctx!.fillText(
                line,
                this.layout.screenLeft.frameCanvas + fontScale * this.layout.scale,
                top.frameCanvas + fontScale * this.layout.scale * (8 + i * 6),
            ),
        );
    }

    initializedSession = signal<Session | undefined>(undefined);

    lastSnapshotStatistics?: SnapshotStatistics;
    lastEmulationStatistics?: EmulationStatistics;
    statisticsVisible = false;

    private session: Session | undefined;
    private initialized = false;

    private mutex = new Mutex();
}
