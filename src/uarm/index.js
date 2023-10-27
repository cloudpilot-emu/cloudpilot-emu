import 'https://unpkg.com/setimmediate@1.0.5';

const labelNor = document.getElementById('nor-image');
const labelNand = document.getElementById('nand-image');
const labelSD = document.getElementById('sd-image');
const speedDisplay = document.getElementById('speed');
const logContainer = document.getElementById('log');

const uploadNor = document.getElementById('upload-nor');
const uploadNand = document.getElementById('upload-nand');
const uploadSD = document.getElementById('upload-sd');
const clearLog = document.getElementById('clear-log');

const canvas = document.getElementsByTagName('canvas')[0];
const canvasCtx = canvas.getContext('2d');

let fileNor, fileNand, fileSd;
let emulator;

function log(message) {
    const line = document.createElement('div');
    line.className = 'log-line';
    line.innerText = message;

    logContainer.appendChild(line);
    logContainer.scrollTop = logContainer.scrollHeight;
}

class Emulator {
    constructor(module) {
        this.module = module;

        this.cycle = module.cwrap('cycle', undefined, ['number']);
        this.getFrame = module.cwrap('getFrame', 'number', []);
        this.resetFrame = module.cwrap('rresetFrame', undefined, []);
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

        this.onMouseDown = (e) => {
            if ((e.buttons & 1) === 0 || !this.module) return;

            const bb = canvas.getBoundingClientRect();
            const x = (e.clientX - bb.x) >>> 1;
            const y = (e.clientY - bb.y) >>> 1;

            if (x < 0 || x >= 320 || y < 0 || y >= 440) return;

            this.penDown(x, y);
        };

        this.onMouseUp = (e) => {
            if (e.button !== 0 || !this.module) return;

            this.penUp();
        };
    }

    static async create(nor, nand, sd) {
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

        return new Emulator(module);
    }

    stop() {
        canvas.removeEventListener('mousedown', this.onMouseDown);
        canvas.removeEventListener('mousemove', this.onMouseDown);
        canvas.removeEventListener('mouseup', this.onMouseUp);

        if (this.immediateHandle) clearImmediate(this.immediateHandle);
        if (this.timeoutHandle) clearTimeout(this.timeoutHandle);

        this.timeoutHandle = this.immediateHandle = undefined;
        speedDisplay.innerText = '-';

        log('emulator stopped');
    }

    start() {
        if (this.timeoutHandle || this.immediateHandle) return;
        this.lastSpeedUpdate = performance.now();

        canvas.addEventListener('mousedown', this.onMouseDown);
        canvas.addEventListener('mousemove', this.onMouseDown);
        canvas.addEventListener('mouseup', this.onMouseUp);

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

        log('emulator running');
        schedule();
    }

    render() {
        if (!this.module) return;

        const framePtr = this.getFrame() >>> 2;
        if (!framePtr) return;

        const frame = this.module.HEAPU32.subarray(framePtr, framePtr + 320 * 320);
        this.imageData32.set(frame);

        this.canvasTmpCtx.putImageData(this.imageData, 0, 0);
        canvasCtx.imageSmoothingEnabled = false;
        canvasCtx.drawImage(this.canvasTmpCtx.canvas, 0, 0, 320, 320, 0, 0, 640, 640);
    }

    updateSpeedDisplay() {
        const currentIps = this.currentIps();
        const currentIpsMax = this.currentIpsMax();

        speedDisplay.innerText = `current ${(currentIps / 1e6).toFixed(2)} MIPS, limit ${(currentIpsMax / 1e6).toFixed(
            2
        )} MIPS -> ${((currentIps / currentIpsMax) * 100).toFixed(2)}%`;
    }
}

function updateLabels() {
    labelNor.innerText = fileNor?.name ?? '[none]';
    labelNand.innerText = fileNand?.name ?? '[none]';
    labelSD.innerText = fileSd?.name ?? '[none]';
}

const openFile = () =>
    new Promise((resolve, reject) => {
        const input = document.createElement('input');
        input.type = 'file';

        input.addEventListener('change', (evt) => {
            const target = evt.target;
            const file = target?.files?.[0];

            if (!file) reject(new Error('no files selected'));

            const reader = new FileReader();

            reader.onload = () =>
                resolve({
                    name: file.name,
                    content: new Uint8Array(reader.result),
                });
            reader.onerror = () => reject(reader.error);

            reader.readAsArrayBuffer(file);
        });

        input.click();
    });

async function restart() {
    if (!(fileNor && fileNand)) return;

    emulator?.stop();

    emulator = await Emulator.create(fileNor.content, fileNand.content, fileSd?.content);
    emulator?.start();
}

function drawSkin() {
    const image = new Image();
    image.src = 'skin.svg';

    image.onload = () => canvasCtx.drawImage(image, 0, 640, 640, 240);
}

async function main() {
    updateLabels();

    canvasCtx.fillStyle = '#fff';
    canvasCtx.fillRect(0, 0, canvas.width, canvas.height);
    drawSkin();

    const uploadHandler = (assign) => () =>
        openFile()
            .then((file) => {
                assign(file);
                updateLabels();

                restart();
            })
            .catch((e) => console.error(e));

    uploadNor.addEventListener(
        'click',
        uploadHandler((file) => (fileNor = file))
    );
    uploadNand.addEventListener(
        'click',
        uploadHandler((file) => (fileNand = file))
    );
    uploadSD.addEventListener(
        'click',
        uploadHandler((file) => (fileSd = file))
    );

    clearLog.addEventListener('click', () => (logContainer.innerHTML = ''));

    document.addEventListener('visibilitychange', () => (document.hidden ? emulator?.stop() : emulator?.start()));

    document;
}

main().catch((e) => console.error(e));
