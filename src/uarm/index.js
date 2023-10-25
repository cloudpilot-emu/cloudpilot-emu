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

const canvas = document.getElementById('canvas-sdl');

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
        this.cycle = module.cwrap('cycle', 'number', []);
        this.currentIps = module.cwrap('currentIps', undefined, []);
        this.currentIpsMax = module.cwrap('currentIpsMax', undefined, []);
    }

    static async create(nor, nand) {
        let module;
        try {
            module = await createModule({
                noInitialRun: true,
                canvas,
                print: log,
                printErr: log,
            });
        } catch (e) {
            console.error('failed to load and compile WASM module', e);
            return;
        }

        module.FS.writeFile('/nor.bin', nor);
        module.FS.writeFile('/nand.bin', nand);

        try {
            if (module.callMain(['-r', '/nor.bin', '-n', '/nand.bin']) !== 0) {
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
        if (this.immediateHandle) clearImmediate(this.immediateHandle);
        if (this.timeoutHandle) clearTimeout(this.timeoutHandle);

        this.timeoutHandle = this.immediateHandle = undefined;
        speedDisplay.innerText = '-';

        log('emulator stopped');
    }

    start() {
        if (this.timeoutHandle || this.immediateHandle) return;
        this.lastSpeedUpdate = performance.now();

        const schedule = () => {
            const now = performance.now();

            const delay = this.cycle() / 1000;
            this.timeoutHandle = this.immediateHandle = undefined;

            if (delay < 10) this.immediateHandle = setImmediate(schedule);
            else this.timeoutHandle = setTimeout(schedule, delay);

            if (now - this.lastSpeedUpdate > 1000) {
                this.updateSpeedDisplay();
                this.lastSpeedUpdate = now;
            }
        };

        log('emulator running');
        schedule();
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

    emulator = await Emulator.create(fileNor.content, fileNand.content);
    emulator?.start();
}

async function main() {
    updateLabels();

    const ctx = canvas.getContext('2d');
    ctx.fillStyle = '#ddd';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

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

    clearLog.addEventListener('click', () => (logContainer.innerHTML = ''));

    document.addEventListener('visibilitychange', () => (document.hidden ? emulator?.stop() : emulator?.start()));

    document;
}

main().catch((e) => console.error(e));
