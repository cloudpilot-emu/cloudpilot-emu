import { AnimationFrameScheduler, Scheduler } from '@common/helper/scheduler';
import { Cloudpilot, PwmUpdate } from '@common/Cloudpilot';

import { Average } from '@common/helper/Average';
import { Event } from 'microevent.ts';
import { Fifo } from '@common/helper/Fifo';
import { GRAYSCALE_PALETTE_RGBA } from '@common/helper/palette';

const PEN_MOVE_THROTTLE = 25;
const SNAPSHOT_INTERVAL = 1000;
const PWM_FIFO_SIZE = 10;
const SPEED_AVERAGE_N = 20;
const TIME_PER_FRAME_AVERAGE_N = 60;
const MIN_FPS = 30;
const DUMMY_SPEED = 1000;

export class EmulationService {
    private onSchedule(): void {}

    private updateScreen(): void {
        const frame = this.cloudpilotInstance.getFrame();

        if (this.cloudpilotInstance.isPowerOff()) {
            this.clearCanvas();

            this.newFrameEvent.dispatch(this.canvas);

            return;
        }

        if (!frame.hasChanges) {
            return;
        }

        if (frame.lines * frame.scaleY === this.canvas.height && frame.lineWidth * frame.scaleX === this.canvas.width) {
            if (!this.imageData || this.imageData.width !== frame.lineWidth || this.imageData.height !== frame.lines) {
                this.imageData = new ImageData(frame.lineWidth, frame.lines);
            }

            const imageData32 = new Uint32Array(this.imageData.data.buffer);

            switch (frame.bpp) {
                case 1: {
                    const fg = GRAYSCALE_PALETTE_RGBA[15];
                    const bg = GRAYSCALE_PALETTE_RGBA[0];
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                frame.buffer[offsetSrc + (i >>> 3)] & (0x80 >>> (i & 0x07)) ? fg : bg;
                        }
                    }

                    break;
                }

                case 2: {
                    const mapping = this.cloudpilotInstance.getPalette2bitMapping();
                    const palette = [
                        GRAYSCALE_PALETTE_RGBA[mapping & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 4) & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 8) & 0x000f],
                        GRAYSCALE_PALETTE_RGBA[(mapping >>> 12) & 0x000f],
                    ];
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                palette[(frame.buffer[offsetSrc + (i >>> 2)] >> (6 - 2 * (i & 0x03))) & 0x03];
                        }
                    }

                    break;
                }

                case 4: {
                    let iDest = 0;

                    for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        const offsetSrc = y * frame.bytesPerLine;

                        for (let i = frame.margin; i < frame.lineWidth + frame.margin; i++) {
                            imageData32[iDest++] =
                                GRAYSCALE_PALETTE_RGBA[
                                    (frame.buffer[offsetSrc + (i >>> 1)] >>> (4 - 4 * (i & 0x01))) & 0x0f
                                ];
                        }
                    }

                    break;
                }

                case 24:
                    {
                        const buffer32 = new Uint32Array(
                            frame.buffer.buffer,
                            frame.buffer.byteOffset,
                            frame.buffer.byteLength >>> 2
                        );

                        if (frame.margin === 0) {
                            imageData32
                                .subarray(0, frame.lineWidth * (frame.lastDirtyLine - frame.firstDirtyLine + 1))
                                .set(
                                    buffer32.subarray(
                                        frame.firstDirtyLine * frame.lineWidth,
                                        frame.lineWidth * (frame.lastDirtyLine + 1)
                                    )
                                );
                        } else {
                            let iDest = 0;

                            for (let y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                                let j = y * (frame.bytesPerLine >>> 2) + frame.margin;

                                for (let x = 0; x < frame.lineWidth; x++) {
                                    imageData32[iDest++] = buffer32[j++];
                                }
                            }
                        }
                    }

                    break;
            }
        }

        if (!this.imageData) return;

        const scaling = frame.scaleX !== 1 || frame.scaleY !== 1;

        (scaling ? this.contextTmp : this.context).putImageData(
            this.imageData,
            0,
            frame.firstDirtyLine,
            0,
            0,
            frame.lineWidth,
            frame.lastDirtyLine - frame.firstDirtyLine + 1
        );

        if (scaling) {
            this.context.imageSmoothingEnabled = false;
            this.context.drawImage(
                this.canvasTmp,
                0,
                frame.firstDirtyLine,
                frame.lineWidth,
                frame.lastDirtyLine - frame.firstDirtyLine + 1,
                0,
                frame.firstDirtyLine * frame.scaleY,
                frame.lineWidth * frame.scaleX,
                (frame.lastDirtyLine - frame.firstDirtyLine + 1) * frame.scaleY
            );
        }

        this.newFrameEvent.dispatch(this.canvas);
    }

    newFrameEvent = new Event<HTMLCanvasElement>();
    pwmUpdateEvent = new Event<PwmUpdate>();
    emulationStateChangeEvent = new Event<boolean>();
    powerOffChangeEvent = new Event<boolean>();

    private cloudpilotInstance: Cloudpilot;

    private clockEmulator = 0;
    private scheduler: Scheduler = new AnimationFrameScheduler(this.onSchedule);

    private canvas: HTMLCanvasElement = document.createElement('canvas');
    private canvasTmp: HTMLCanvasElement = document.createElement('canvas');
    private context!: CanvasRenderingContext2D;
    private contextTmp!: CanvasRenderingContext2D;
    private imageData: ImageData | undefined;

    private lastPenUpdate = 0;
    private penDown = false;

    private running = false;
    private powerOff = false;
    private uiInitialized = false;

    private lastSnapshotAt = 0;
    private deviceHotsyncName: string | undefined;

    private pendingPwmUpdates = new Fifo<PwmUpdate>(PWM_FIFO_SIZE);

    private advanceEmulationHandle: number | undefined;

    private emulationSpeed = 1;
    private speedAverage = new Average(SPEED_AVERAGE_N);

    private timePerFrameAverage = new Average(TIME_PER_FRAME_AVERAGE_N);
    private timestampLastFrame = 0;

    private frameCounter = 0;
}
