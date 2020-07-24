import Cloudpilot from './Cloudpilot';

const PALETTE_GRAYSCALE = [
    0xffd2d2d2,
    0xffc4c4c4,
    0xffb6b6b6,
    0xffa8a8a8,
    0xff9a9a9a,
    0xff8c8c8c,
    0xff7e7e7e,
    0xff707070,
    0xff626262,
    0xff545454,
    0xff464646,
    0xff383838,
    0xff2a2a2a,
    0xff1c1c1c,
    0xff0e0e0e,
    0xff000000,
];

const BACKGROUND_COLOR = PALETTE_GRAYSCALE[0];
const FOREGROUND_COLOR = PALETTE_GRAYSCALE[15];

class EmulationService {
    private constructor(private cloudpilot: Cloudpilot) {}

    static async create(): Promise<EmulationService> {
        return new EmulationService(await Cloudpilot.create());
    }

    async initialize(rom: Uint8Array, deviceType: string): Promise<void> {
        const romInfo = this.cloudpilot.getRomInfo(rom);

        console.log(romInfo);

        if (!this.cloudpilot.initializeSession(rom, deviceType)) {
            throw new Error('unable to initialize session');
        }
    }

    setCanvas(canvas: HTMLCanvasElement): void {
        this.renderingCtx = canvas.getContext('2d');
    }

    unsetCanvas(): void {
        this.renderingCtx = null;
    }

    start(): void {
        if (this.animationFrameHandle) return;

        this.initialTimestamp = performance.now() - 10;
        this.emulatedTime = 0;

        this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame.bind(this));
    }

    private onAnimationFrame(): void {
        const realTime = performance.now() - this.initialTimestamp;

        let timeToRun = realTime - this.emulatedTime;

        if (timeToRun > 500) {
            timeToRun = 10;
            this.emulatedTime = realTime - timeToRun;
        }

        if (timeToRun > 0) {
            const cyclesPerSecond = this.cloudpilot.cyclesPerSecond();

            const cycles = this.cloudpilot.runEmulation(Math.round((timeToRun / 1000) * cyclesPerSecond));
            this.emulatedTime += (cycles / cyclesPerSecond) * 1000;

            this.drawFrame();

            this.animationFrameHandle = requestAnimationFrame(this.onAnimationFrame.bind(this));
        }
    }

    private drawFrame() {
        if (!this.renderingCtx) return;

        const frame = this.cloudpilot.getFrame();

        if (!frame || frame.lineWidth !== 160 || frame.lines !== 160) return;

        const imageBuffer = new Uint32Array(frame.lineWidth * frame.lines);

        switch (frame.bpp) {
            case 1:
                for (let y = 0; y < 160; y++)
                    for (let x = 0; x < 160; x++) {
                        const byte = frame.buffer[y * frame.bytesPerLine + ((x + frame.margin) >>> 3)];
                        const mask = 0x80 >>> ((x + frame.margin) & 0x07);

                        imageBuffer[y * frame.lineWidth + x] =
                            (byte & mask) === 0 ? BACKGROUND_COLOR : FOREGROUND_COLOR;
                    }

                break;

            default:
        }

        const imageData = new ImageData(new Uint8ClampedArray(imageBuffer.buffer), frame.lineWidth, frame.lines);

        this.renderingCtx.putImageData(imageData, 0, 0);
    }

    private animationFrameHandle = 0;

    private initialTimestamp = 0;
    private emulatedTime = 0;

    private renderingCtx: CanvasRenderingContext2D | null = null;
}

export default EmulationService;
