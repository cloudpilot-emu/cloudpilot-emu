import { PalmButton } from '@common/bridge/Cloudpilot';
import { deviceDimensions, isColor } from '@common/helper/deviceProperties';
import { PrerenderedImage } from '@common/helper/image';
import { GRAYSCALE_PALETTE_HEX } from '@common/helper/palette';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { ScreenSize } from '@common/model/Dimensions';
import { Event } from 'microevent.ts';

import { SkinElement, SkinLoader } from './SkinLoader';

export const DEFAULT_DEVICE = DeviceId.m515;
const BACKGROUND_COLOR_SILKSCREEN = GRAYSCALE_PALETTE_HEX[2];
const BACKGROUND_COLOR_GRAYSCALE_DEVICE = GRAYSCALE_PALETTE_HEX[0];
const BACKGROUND_COLOR_COLOR_DEVICE = 'white';
const FRAME_COLOR_COLOR_DEVICE = 'white';
const FRAME_COLOR_GRAYSCALE_DEVICE = '#ccc';
const BACKGROUND_ACTIVE_BUTTON = 'rgba(0,0,0,0.2)';

interface FrameDependent {
    frameDevice: number;
    frameCanvas: number;
}

interface Layout {
    scale: number;
    borderWidth: FrameDependent;
    width: FrameDependent;
    height: FrameDependent;
    screenHeight: FrameDependent;
    screenWidth: FrameDependent;
    screenLeft: FrameDependent;
    screenTop: FrameDependent;
    screenBottom: FrameDependent;
    separatorHeight: FrameDependent;
    silkscreenHeight: FrameDependent;
    silkscreenTop: FrameDependent;
    silkscreenBottom: FrameDependent;
    softSilkscreenHeight: FrameDependent;
    buttonHeight: FrameDependent;
    buttonWidth: FrameDependent;
    buttonTop: FrameDependent;
    buttonBottom: FrameDependent;
    radiusDpadSelect: FrameDependent;
    angleDpad: number;
}

function buttonHeightForScreenSize(screenSize: ScreenSize) {
    switch (screenSize) {
        case ScreenSize.screen320x480:
        case ScreenSize.screen320x320:
            return 60;

        case ScreenSize.screen240x320:
            return 45;

        default:
            return 30;
    }
}

function calculateLayout(device: DeviceId): Layout {
    const dimensions = deviceDimensions(device);
    const scale = (dimensions.screenSize === ScreenSize.screen160x160 ? 3 : 2) * devicePixelRatio;
    const borderWidth: FrameDependent = { frameDevice: 1, frameCanvas: scale };

    const dist = (x: number): FrameDependent => ({ frameDevice: x, frameCanvas: x * scale });
    const coord = (x: number): FrameDependent => ({ frameDevice: x, frameCanvas: borderWidth.frameCanvas + x * scale });

    const separatorHeight = dimensions.silkscreenHeight > 0 ? dist(1) : dist(0);
    const buttonHeight = dist(buttonHeightForScreenSize(dimensions.screenSize));

    return {
        scale,
        borderWidth,
        height: dist(
            2 * borderWidth.frameDevice +
                dimensions.height +
                separatorHeight.frameDevice +
                dimensions.silkscreenHeight +
                buttonHeight.frameDevice,
        ),
        width: dist(2 * borderWidth.frameDevice + dimensions.width),
        screenHeight: dist(dimensions.height),
        screenWidth: dist(dimensions.width),
        screenLeft: coord(0),
        screenTop: coord(0),
        screenBottom: coord(dimensions.height),
        separatorHeight,
        silkscreenHeight: dist(dimensions.silkscreenHeight),
        silkscreenTop: coord(dimensions.height + separatorHeight.frameDevice),
        silkscreenBottom: coord(dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight),
        softSilkscreenHeight: dist(dimensions.silkscreenHeight > 0 ? 0 : dimensions.height - dimensions.width),
        buttonHeight,
        buttonWidth: buttonHeight,
        buttonTop: coord(dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight),
        buttonBottom: coord(
            dimensions.height + separatorHeight.frameDevice + dimensions.silkscreenHeight + buttonHeight.frameDevice,
        ),
        radiusDpadSelect: { frameCanvas: buttonHeight.frameCanvas / 5, frameDevice: buttonHeight.frameDevice / 5 },
        angleDpad:
            Math.PI / 2 - Math.atan((dimensions.width - 4 * buttonHeight.frameDevice) / buttonHeight.frameDevice),
    };
}

export abstract class AbstractCanvasDisplayService {
    constructor(protected skinLoader: SkinLoader) {}

    get width(): number {
        switch (this.getOrientation()) {
            case DeviceOrientation.portrait:
            case DeviceOrientation.portrait180:
            case undefined:
                return this.layout.width.frameCanvas;

            default:
                return this.layout.height.frameCanvas;
        }
    }

    get height(): number {
        switch (this.getOrientation()) {
            case DeviceOrientation.portrait:
            case DeviceOrientation.portrait180:
            case undefined:
                return this.layout.height.frameCanvas;

            default:
                return this.layout.width.frameCanvas;
        }
    }

    updateEmulationCanvas(canvas?: HTMLCanvasElement) {
        this.setupTransformation();
        this.drawEmulationCanvas(canvas);

        this.onAfterUpdateCanvas();
    }

    eventToPalmCoordinates(e: MouseEvent | Touch, clip = false): [number, number] | undefined {
        if (!this.ctx) return;

        const bb = this.ctx.canvas.getBoundingClientRect();

        let contentX: number;
        let contentY: number;
        let contentWidth: number;
        let contentHeight: number;

        // CSS object-fit keeps the aspect ratio of the canvas content, but the canvas itself
        // looses aspect and fills the container -> manually calculate the metrics for the content
        if (bb.width / bb.height > this.width / this.height) {
            contentHeight = bb.height;
            contentWidth = (this.width / this.height) * bb.height;
            contentY = bb.top;
            contentX = bb.left + (bb.width - contentWidth) / 2;
        } else {
            contentWidth = bb.width;
            contentHeight = (this.height / this.width) * bb.width;
            contentX = bb.left;
            contentY = bb.top + (bb.height - contentHeight) / 2;
        }

        // Transform coordinate to device frame
        let x = (((e.clientX - contentX) / contentWidth) * this.width) / this.layout.scale;
        let y = (((e.clientY - contentY) / contentHeight) * this.height) / this.layout.scale;

        // Rotate if applicable
        switch (this.getOrientation()) {
            case DeviceOrientation.landscape90: {
                const tmp = y;
                y = this.layout.height.frameDevice - x;
                x = tmp;

                break;
            }

            case DeviceOrientation.landscape270: {
                const tmp = y;
                y = x;
                x = this.layout.width.frameDevice - tmp;
                break;
            }

            case DeviceOrientation.portrait180: {
                x = this.layout.width.frameDevice - x;
                y = this.layout.height.frameDevice - y;
                break;
            }
        }

        // Compensate for the border
        x -= this.layout.borderWidth.frameDevice;
        y -= this.layout.borderWidth.frameDevice;

        // The canvas layout inside the border is as follows:
        //
        // * 0 .. 159   : LCD
        // * 160        : separator
        // * 161 .. 220 : silkscreen
        // * 221 .. 250 : buttons
        //
        // we map this to 160x250 lines by mapping the separator to the silkscreen

        if (y >= this.layout.screenHeight.frameDevice) {
            if (y <= this.layout.screenHeight.frameDevice + this.layout.separatorHeight.frameDevice) {
                y = this.layout.screenHeight.frameDevice;
            } else {
                y -= this.layout.separatorHeight.frameDevice;
            }
        }

        const totalHeight =
            this.layout.screenHeight.frameDevice +
            this.layout.silkscreenHeight.frameDevice +
            this.layout.buttonHeight.frameDevice;

        if (clip) {
            if (x < 0) x = 0;
            if (x >= this.layout.screenWidth.frameDevice) x = this.layout.screenWidth.frameDevice - 1;
            if (y < 0) y = 0;
            if (y >= totalHeight) y = totalHeight - 1;
        } else {
            if (x < 0 || x >= this.layout.screenWidth.frameDevice || y < 0 || y >= totalHeight) {
                return undefined;
            }
        }

        return [x, y];
    }

    isSilkscreen(coords: [number, number]): boolean {
        const [, y] = coords;

        return !this.isButtons(coords) && y >= this.layout.screenHeight.frameDevice;
    }

    isButtons([, y]: [number, number]): boolean {
        return y >= this.layout.screenHeight.frameDevice + this.layout.silkscreenHeight.frameDevice;
    }

    determineButton([x, y]: [number, number]): PalmButton {
        if (x >= this.layout.screenWidth.frameDevice - this.layout.buttonWidth.frameDevice) return PalmButton.notes;

        if (x >= this.layout.screenWidth.frameDevice - 2 * this.layout.buttonWidth.frameDevice) return PalmButton.todo;

        if (x >= 2 * this.layout.buttonWidth.frameDevice) {
            if (this.dpad) {
                const center = {
                    x: this.layout.screenWidth.frameDevice / 2,
                    y: this.layout.buttonTop.frameDevice + this.layout.buttonHeight.frameDevice / 2,
                };

                const rel = { x: Math.abs(x - center.x), y: Math.abs(y - center.y) };

                const tanPhi = Math.tan(this.layout.angleDpad);

                if (rel.x ** 2 + rel.y ** 2 <= this.layout.radiusDpadSelect.frameDevice ** 2) {
                    return PalmButton.rockerEnter;
                }

                if (y < center.y && rel.y / rel.x >= tanPhi) return PalmButton.up;

                if (y > center.y && rel.y / rel.x >= tanPhi) return PalmButton.down;

                if (x < center.x && rel.y / rel.x < tanPhi) return PalmButton.rockerLeft;

                return PalmButton.rockerRight;
            } else {
                return y >=
                    this.layout.screenHeight.frameDevice +
                        this.layout.silkscreenHeight.frameDevice +
                        this.layout.buttonHeight.frameDevice / 2
                    ? PalmButton.down
                    : PalmButton.up;
            }
        }

        if (x >= this.layout.buttonWidth.frameDevice) return PalmButton.phone;

        return PalmButton.cal;
    }

    protected async initWithCanvas(
        dpad: boolean,
        canvas: HTMLCanvasElement | undefined = this.ctx?.canvas,
    ): Promise<void> {
        if (!canvas) return;

        this.dpad = dpad;
        this.layout = calculateLayout(this.getDeviceId());

        this.onResize.dispatch();

        canvas.width = this.width;
        canvas.height = this.height;

        const ctx = canvas.getContext('2d');
        if (!ctx) {
            throw new Error('canvas not supported - get a new browser');
        }

        this.ctx = ctx;

        this.setupTransformation();
        this.fillCanvasRect(0, 0, this.layout.width.frameCanvas, this.layout.height.frameCanvas, this.frameColor());
        this.fillRect(
            0,
            0,
            this.layout.screenWidth.frameDevice,
            this.layout.screenHeight.frameDevice +
                this.layout.silkscreenHeight.frameDevice +
                this.layout.separatorHeight.frameDevice +
                this.layout.buttonHeight.frameDevice,
            this.backgroundColor(),
        );

        this.drawEmulationCanvas();
        await Promise.all([this.drawSilkscreen(), this.drawButtons()]);
    }

    async drawButtons(activeButtons: Array<PalmButton> = []): Promise<void> {
        if (!this.ctx) return;
        this.setupTransformation();

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(
            await this.buttonsImage()(this.layout.screenWidth.frameCanvas, this.layout.buttonHeight.frameCanvas),
            this.layout.screenLeft.frameCanvas,
            this.layout.buttonTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.buttonHeight.frameCanvas,
        );

        if (activeButtons.includes(PalmButton.cal)) {
            this.fillRect(
                0,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON,
            );
        }
        if (activeButtons.includes(PalmButton.phone)) {
            this.fillRect(
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON,
            );
        }
        if (activeButtons.includes(PalmButton.todo)) {
            this.fillRect(
                this.layout.screenWidth.frameDevice - 2 * this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON,
            );
        }
        if (activeButtons.includes(PalmButton.notes)) {
            this.fillRect(
                this.layout.screenWidth.frameDevice - this.layout.buttonWidth.frameDevice,
                this.layout.buttonTop.frameDevice,
                this.layout.buttonWidth.frameDevice,
                this.layout.buttonHeight.frameDevice,
                BACKGROUND_ACTIVE_BUTTON,
            );
        }
        if (!this.dpad) {
            if (activeButtons.includes(PalmButton.up)) {
                this.fillRect(
                    2 * this.layout.buttonWidth.frameDevice,
                    this.layout.buttonTop.frameDevice,
                    this.layout.screenWidth.frameDevice - 4 * this.layout.buttonWidth.frameDevice,
                    this.layout.buttonHeight.frameDevice / 2,
                    BACKGROUND_ACTIVE_BUTTON,
                );
            }
            if (activeButtons.includes(PalmButton.down)) {
                this.fillRect(
                    2 * this.layout.buttonWidth.frameDevice,
                    this.layout.buttonTop.frameDevice + this.layout.buttonHeight.frameDevice / 2,
                    this.layout.screenWidth.frameDevice - 4 * this.layout.buttonWidth.frameDevice,
                    this.layout.buttonHeight.frameDevice / 2,
                    BACKGROUND_ACTIVE_BUTTON,
                );
            }
        } else {
            const center = {
                x: this.layout.borderWidth.frameCanvas + this.layout.screenWidth.frameCanvas / 2,
                y: this.layout.buttonTop.frameCanvas + this.layout.buttonHeight.frameCanvas / 2,
            };

            const topLeft = {
                x: this.layout.borderWidth.frameCanvas + 2 * this.layout.buttonWidth.frameCanvas,
                y: this.layout.buttonTop.frameCanvas,
            };

            const topRight = {
                x:
                    this.layout.borderWidth.frameCanvas +
                    this.layout.screenWidth.frameCanvas -
                    2 * this.layout.buttonWidth.frameCanvas,
                y: this.layout.buttonTop.frameCanvas,
            };

            const bottomLeft = {
                x: topLeft.x,
                y: this.layout.buttonTop.frameCanvas + this.layout.buttonHeight.frameCanvas,
            };

            const bottomRight = {
                x: topRight.x,
                y: this.layout.buttonTop.frameCanvas + this.layout.buttonHeight.frameCanvas,
            };

            const r = this.layout.radiusDpadSelect.frameCanvas;
            const phi = this.layout.angleDpad;

            if (activeButtons.includes(PalmButton.up)) {
                this.ctx.beginPath();

                this.ctx.moveTo(topLeft.x, topLeft.y);
                this.ctx.lineTo(topRight.x, topRight.y);
                this.ctx.lineTo(center.x + r * Math.cos(phi), center.y - r * Math.sin(phi));
                this.ctx.arc(center.x, center.y, r, -phi, phi - Math.PI, true);
                this.ctx.lineTo(topLeft.x, topLeft.y);

                this.ctx.fillStyle = BACKGROUND_ACTIVE_BUTTON;
                this.ctx.fill();
            }

            if (activeButtons.includes(PalmButton.down)) {
                this.ctx.beginPath();

                this.ctx.moveTo(bottomLeft.x, bottomLeft.y);
                this.ctx.lineTo(bottomRight.x, bottomRight.y);
                this.ctx.lineTo(center.x + r * Math.cos(phi), center.y + r * Math.sin(phi));
                this.ctx.arc(center.x, center.y, r, phi, Math.PI - phi, false);
                this.ctx.lineTo(bottomLeft.x, bottomLeft.y);

                this.ctx.fillStyle = BACKGROUND_ACTIVE_BUTTON;
                this.ctx.fill();
            }

            if (activeButtons.includes(PalmButton.rockerLeft)) {
                this.ctx.beginPath();

                this.ctx.moveTo(topLeft.x, topLeft.y);
                this.ctx.lineTo(bottomLeft.x, bottomLeft.y);
                this.ctx.lineTo(center.x - r * Math.cos(phi), center.y + r * Math.sin(phi));
                this.ctx.arc(center.x, center.y, r, Math.PI - phi, Math.PI + phi, false);
                this.ctx.lineTo(topLeft.x, topLeft.y);

                this.ctx.fillStyle = BACKGROUND_ACTIVE_BUTTON;
                this.ctx.fill();
            }

            if (activeButtons.includes(PalmButton.rockerRight)) {
                this.ctx.beginPath();

                this.ctx.moveTo(topRight.x, topRight.y);
                this.ctx.lineTo(bottomRight.x, bottomRight.y);
                this.ctx.lineTo(center.x + r * Math.cos(phi), center.y + r * Math.sin(phi));
                this.ctx.arc(center.x, center.y, r, phi, -phi, true);
                this.ctx.lineTo(topRight.x, topRight.y);

                this.ctx.fillStyle = BACKGROUND_ACTIVE_BUTTON;
                this.ctx.fill();
            }

            if (activeButtons.includes(PalmButton.rockerEnter)) {
                this.ctx.beginPath();

                this.ctx.arc(center.x, center.y, r, 0, Math.PI * 2);

                this.ctx.fillStyle = BACKGROUND_ACTIVE_BUTTON;
                this.ctx.fill();
            }
        }
    }

    protected abstract getDeviceId(): DeviceId;

    protected abstract getOrientation(): DeviceOrientation;

    protected onAfterUpdateCanvas(): void {}

    protected setupTransformation(): void {
        if (!this.ctx) return;

        this.ctx.resetTransform();

        switch (this.getOrientation()) {
            case DeviceOrientation.portrait:
            case undefined:
                break;

            case DeviceOrientation.landscape90:
                this.ctx.translate(this.layout.height.frameCanvas, 0);
                this.ctx.rotate(Math.PI / 2);
                break;

            case DeviceOrientation.landscape270:
                this.ctx.translate(0, this.layout.width.frameCanvas);
                this.ctx.rotate((3 * Math.PI) / 2);
                break;

            case DeviceOrientation.portrait180:
                this.ctx.translate(this.layout.width.frameCanvas, this.layout.height.frameCanvas);
                this.ctx.rotate(Math.PI);
                break;
        }
    }

    protected async drawSilkscreen(): Promise<void> {
        if (this.layout.silkscreenHeight.frameCanvas === 0 || !this.ctx) return;
        this.setupTransformation();

        this.fillRect(
            this.layout.screenLeft.frameDevice,
            this.layout.silkscreenTop.frameDevice,
            this.layout.screenWidth.frameDevice,
            this.layout.silkscreenHeight.frameDevice,
            BACKGROUND_COLOR_SILKSCREEN,
        );

        this.ctx.imageSmoothingEnabled = true;
        this.ctx.imageSmoothingQuality = 'high';

        this.ctx.drawImage(
            await this.silkscreenImage()(this.layout.screenWidth.frameCanvas, this.layout.silkscreenHeight.frameCanvas),
            this.layout.borderWidth.frameCanvas,
            this.layout.silkscreenTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.silkscreenHeight.frameCanvas,
        );
    }

    protected drawEmulationCanvas(canvas = this.lastEmulationCanvas): void {
        if (!canvas || !this.ctx) return;
        this.lastEmulationCanvas = canvas;

        this.ctx.imageSmoothingEnabled = false;
        this.ctx.drawImage(
            canvas,
            this.layout.screenLeft.frameCanvas,
            this.layout.screenTop.frameCanvas,
            this.layout.screenWidth.frameCanvas,
            this.layout.screenHeight.frameCanvas,
        );
    }

    protected fillRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(
            this.layout.borderWidth.frameCanvas + this.layout.scale * x,
            this.layout.borderWidth.frameCanvas + this.layout.scale * y,
            this.layout.scale * width,
            this.layout.scale * height,
        );
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    protected fillCanvasRect(x: number, y: number, width: number, height: number, style: string): void {
        if (!this.ctx) return;

        this.ctx.beginPath();
        this.ctx.rect(x, y, width, height);
        this.ctx.fillStyle = style;
        this.ctx.fill();
    }

    protected silkscreenImage(): PrerenderedImage {
        return this.skinLoader.loadSkinElement(this.silkscreenSkinElement());
    }

    protected silkscreenSkinElement(): SkinElement {
        switch (this.getDeviceId()) {
            case DeviceId.pegT600c:
            case DeviceId.pegT650c:
                return SkinElement.SILKSCREEN_PEG_T600;

            case DeviceId.pegN700c:
            case DeviceId.pegN600c:
                return SkinElement.SILKSCREEN_PEG_N610;

            case DeviceId.pegS320:
            case DeviceId.pegS300:
                return SkinElement.SILKSCREEN_PEG_S300;

            case DeviceId.pegS500c:
                return SkinElement.SILKSCREEN_PEG_S500;

            case DeviceId.pegT400:
                return SkinElement.SILKSCREEN_PEG_T415;

            case DeviceId.acerS11:
                return SkinElement.SILKSCREEN_ACER_S10;

            case DeviceId.palmPilot:
            case DeviceId.pilot:
            case DeviceId.iii:
            case DeviceId.palmVII:
                return SkinElement.SILKSCREEN_PILOT;

            case DeviceId.m500:
                return SkinElement.SILKSCREEN_M500;

            case DeviceId.m505:
            case DeviceId.m515:
            case DeviceId.m520:
                return SkinElement.SILKSCREEN_M515;

            case DeviceId.iiic:
                return SkinElement.SILKSCREEN_IIIC;

            case DeviceId.m100:
            case DeviceId.m105:
            case DeviceId.m125:
                return SkinElement.SILKSCREEN_M100;

            case DeviceId.m130:
                return SkinElement.SILKSCREEN_M130;

            case DeviceId.i710:
            case DeviceId.te2:
                return SkinElement.SILKSCREEN_TUNGSTEN_W;

            case DeviceId.i705:
                return SkinElement.SILKSCREEN_I705;

            case DeviceId.iiix:
            case DeviceId.iiixe:
            case DeviceId.palmVx:
            case DeviceId.palmV:
            case DeviceId.palmVIIEZ:
            case DeviceId.palmVIIx:
            default:
                return SkinElement.SILKSCREEN_V;
        }
    }

    protected buttonsImage(): PrerenderedImage {
        return this.skinLoader.loadSkinElement(this.buttonsSkinElement());
    }

    protected buttonsSkinElement(): SkinElement {
        switch (this.getDeviceId()) {
            case DeviceId.acerS11:
                return SkinElement.HARD_BUTTONS_ACER_S10;

            case DeviceId.pegNR70:
                return SkinElement.HARD_BUTTONS_PEG_NR70;

            case DeviceId.pegN700c:
                return SkinElement.HARD_BUTTONS_PEG_N700;

            case DeviceId.pegN600c:
                return SkinElement.HARD_BUTTONS_PEG_N610;

            case DeviceId.pegS300:
                return SkinElement.HARD_BUTTONS_PEG_S300;

            case DeviceId.pegS500c:
                return SkinElement.HARD_BUTTONS_PEG_S500;

            case DeviceId.pegS320:
                return SkinElement.HARD_BUTTONS_PEG_S320;

            case DeviceId.pegT400:
            case DeviceId.pegT600c:
            case DeviceId.pegT650c:
                return SkinElement.HARD_BUTTONS_PEG_T415;

            case DeviceId.pilot:
            case DeviceId.palmPilot:
                return SkinElement.HARD_BUTTONS_PILOT;

            case DeviceId.m500:
                return SkinElement.HARD_BUTTONS_M500;

            case DeviceId.m505:
            case DeviceId.m515:
            case DeviceId.m520:
                return SkinElement.HARD_BUTTONS_M515;

            case DeviceId.iiic:
                return SkinElement.HARD_BUTTONS_IIIC;

            case DeviceId.m100:
            case DeviceId.m105:
                return SkinElement.HARD_BUTTONS_M100;

            case DeviceId.m125:
                return SkinElement.HARD_BUTTONS_M125;

            case DeviceId.m130:
                return SkinElement.HARD_BUTTONS_M130;

            case DeviceId.i710:
            case DeviceId.te2:
            case DeviceId.frankene2:
                return SkinElement.HARD_BUTTONS_TUNGSTEN_W;

            case DeviceId.iii:
            case DeviceId.iiix:
            case DeviceId.iiixe:
            case DeviceId.palmVII:
            case DeviceId.palmVIIEZ:
            case DeviceId.palmVIIx:
                return SkinElement.HARD_BUTTONS_IIIX;

            case DeviceId.iiie:
                return SkinElement.HARD_BUTTONS_IIIE;

            case DeviceId.i705:
                return SkinElement.HARD_BUTTONS_I705;

            case DeviceId.handera330:
                return SkinElement.HARD_BUTTONS_HANDERA330;

            case DeviceId.handera330c:
                return SkinElement.HARD_BUTTONS_HANDERA330C;

            case DeviceId.lp168:
                return SkinElement.HARD_BUTTONS_LEGEND_P168;

            case DeviceId.palmVx:
            case DeviceId.palmV:
            default:
                return SkinElement.HARD_BUTTONS_PALM_V;
        }
    }

    protected backgroundColor(): string {
        return isColor(this.getDeviceId()) ? BACKGROUND_COLOR_COLOR_DEVICE : BACKGROUND_COLOR_GRAYSCALE_DEVICE;
    }

    protected frameColor(): string {
        return isColor(this.getDeviceId()) ? FRAME_COLOR_COLOR_DEVICE : FRAME_COLOR_GRAYSCALE_DEVICE;
    }

    protected ctx: CanvasRenderingContext2D | undefined;
    protected layout = calculateLayout(DEFAULT_DEVICE);
    protected lastEmulationCanvas: HTMLCanvasElement | undefined;
    protected dpad = false;

    readonly onResize = new Event<void>();

    // protected dimensions = DEFAULT_DIMENSIONS;
}
