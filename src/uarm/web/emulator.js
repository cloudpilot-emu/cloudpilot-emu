import { EventHandler } from './eventhandler.js';

export class Emulator {
    constructor(module, { canvasCtx, speedDisplay, log }) {
        this.module = module;
        this.canvasCtx = canvasCtx;
        this.speedDisplay = speedDisplay;
        this.log = log;

        this.cycle = module.cwrap('cycle', undefined, ['number']);
        this.getFrame = module.cwrap('getFrame', 'number', []);
        this.resetFrame = module.cwrap('resetFrame', undefined, []);
        this.currentIps = module.cwrap('currentIps', undefined, []);
        this.currentIpsMax = module.cwrap('currentIpsMax', undefined, []);
        this.getTimesliceSizeUsec = module.cwrap('getTimesliceSizeUsec', 'number', []);
        this.penDown = module.cwrap('penDown', undefined, ['number', 'number']);
        this.penUp = module.cwrap('penUp', undefined, []);

        this.imageData = new ImageData(320, 320);
        this.imageData32 = new Uint32Array(this.imageData.data.buffer);
        this.canvasTmpCtx = document.createElement('canvas').getContext('2d');
        this.canvasTmpCtx.canvas.width = 320;
        this.canvasTmpCtx.canvas.height = 320;

        this.eventHandler = new EventHandler(this, canvasCtx.canvas);

        this.onMouseDown = (e) => {
            if ((e.buttons & 1) === 0 || !this.module) return;

            const bb = this.canvasCtx.canvas.getBoundingClientRect();
            const x = (e.clientX - bb.x) >>> 1;
            const y = (e.clientY - bb.y) >>> 1;

            if (x < 0 || x >= 320 || y < 0 || y >= 440) return;

            console.log(x, y);

            setImmediate(() => this.penDown(x, y));
        };

        this.onMouseUp = (e) => {
            if (e.button !== 0 || !this.module) return;

            e.stopPropagation();
            e.preventDefault();

            console.log('penUp');

            setImmediate(() => this.penUp());
        };
    }

    static async create(nor, nand, sd, env) {
        const { canvasCtx, speedDisplay, log } = env;
        let module;

        try {
            module = await createModule({
                noInitialRun: true,
                print: log,
                printErr: log,
            });
        } catch (e) {
            console.error('failed to load and compile WASM module', e);
            return;
        }

        module.FS.writeFile('/nor.bin', nor);
        module.FS.writeFile('/nand.bin', nand);
        if (sd) module.FS.writeFile('/sd.img', sd);

        try {
            if (module.callMain(['-r', '/nor.bin', '-n', '/nand.bin', ...(sd ? ['-s', '/sd.img'] : [])]) !== 0) {
                log('uARM terminated with error');
                return;
            }
        } catch (e) {
            log('uARM aborted');
            return;
        }

        return new Emulator(module, env);
    }

    stop() {
        this.eventHandler.stop();

        if (this.immediateHandle) clearImmediate(this.immediateHandle);
        if (this.timeoutHandle) clearTimeout(this.timeoutHandle);

        this.timeoutHandle = this.immediateHandle = undefined;
        this.speedDisplay.innerText = '-';

        this.log('emulator stopped');
    }

    start() {
        if (this.timeoutHandle || this.immediateHandle) return;
        this.lastSpeedUpdate = performance.now();

        this.eventHandler.start();

        const schedule = () => {
            const now = performance.now();

            this.cycle(now * 1000);
            this.render();

            const timesliceRemainning = this.getTimesliceSizeUsec() / 1000 - performance.now() + now;
            this.timeoutHandle = this.immediateHandle = undefined;

            if (timesliceRemainning < 10) this.immediateHandle = setImmediate(schedule);
            else this.timeoutHandle = setTimeout(schedule, timesliceRemainning);

            if (now - this.lastSpeedUpdate > 1000) {
                this.updateSpeedDisplay();
                this.lastSpeedUpdate = now;
            }
        };

        this.log('emulator running');
        schedule();
    }

    render() {
        if (!this.module) return;

        const framePtr = this.getFrame() >>> 2;
        if (!framePtr) return;

        const frame = this.module.HEAPU32.subarray(framePtr, framePtr + 320 * 320);
        this.imageData32.set(frame);
        this.resetFrame();

        this.canvasTmpCtx.putImageData(this.imageData, 0, 0);
        this.canvasCtx.imageSmoothingEnabled = false;
        this.canvasCtx.drawImage(this.canvasTmpCtx.canvas, 0, 0, 320, 320, 0, 0, 640, 640);
    }

    updateSpeedDisplay() {
        const currentIps = this.currentIps();
        const currentIpsMax = this.currentIpsMax();

        this.speedDisplay.innerText = `current ${(currentIps / 1e6).toFixed(2)} MIPS, limit ${(
            currentIpsMax / 1e6
        ).toFixed(2)} MIPS -> ${((currentIps / currentIpsMax) * 100).toFixed(2)}%`;
    }
}
