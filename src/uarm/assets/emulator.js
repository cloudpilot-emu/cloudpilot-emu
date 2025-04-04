import { DisplayService } from './displayservice.js';
import { EventHandler } from './eventhandler.js';

let resolveModule;
export const module = new Promise((r) => (resolveModule = r));

export function loadModule(path) {
    const compile = WebAssembly.compileStreaming
        ? WebAssembly.compileStreaming(fetch(path))
        : fetch(path)
              .then((response) => response.arrayBuffer())
              .then((binary) => WebAssembly.compile(binary));

    compile.then(resolveModule).catch((e) => console.error('failed to load and compile binary', e));
}

export class Emulator {
    constructor(
        worker,
        displayService,
        crcCheck,
        deviceType,
        { canvas, speedDisplay, log, database, setSnapshotStatus }
    ) {
        this.stopping = Promise.resolve();
        this.onStopped = () => undefined;

        this.worker = worker;
        this.displayService = displayService;
        this.database = database;
        this.setSnapshotStatus = setSnapshotStatus;
        this.crcCheck = crcCheck;

        this.speedDisplay = speedDisplay;
        this.log = log;
        this.canvas = canvas;

        this.deviceType = deviceType;
        displayService.setDeviceType(deviceType);

        this.canvasTmpCtx = document.createElement('canvas').getContext('2d');
        this.canvasTmpCtx.canvas.width = 320;
        this.canvasTmpCtx.canvas.height = deviceType === 1 ? 480 : 320;

        this.running = false;
        this.snapshotStatus = 'ok';
        this.clearSnapshotStatusHandle = undefined;

        this.onMessage = (e) => {
            const message = e.data;
            switch (message.type) {
                case 'frame':
                    this.render(e.data.data);
                    break;

                case 'speed':
                    this.updateSpeedDisplay(message.text);
                    break;

                case 'log':
                    this.log(message.message);
                    break;

                case 'error':
                    console.error(message.reason);
                    break;

                case 'snapshot':
                    this.handleSnapshot(message.snapshot);
                    break;

                case 'stopped':
                    this.onStopped();
                    break;

                case 'session':
                case 'getSessionFailed':
                    break;

                default:
                    console.error('unknown message from worker', message);
                    break;
            }
        };

        this.worker.addEventListener('message', this.onMessage);
        this.eventHandler = new EventHandler(this, this.displayService);
    }

    static async create(nor, nand, sd, ram, maxLoad, cyclesPerSecondLimit, env) {
        const { log, crcCheck } = env;
        const worker = new Worker('assets/worker.js');

        const displayService = new DisplayService();
        displayService.initWithCanvas(env.canvas);

        return new Promise((resolve, reject) => {
            const onMessage = (e) => {
                switch (e.data.type) {
                    case 'ready':
                        module.then((module) =>
                            worker.postMessage({
                                type: 'initialize',
                                nor,
                                nand,
                                sd,
                                ram,
                                maxLoad,
                                cyclesPerSecondLimit,
                                crcCheck,
                                module,
                            })
                        );

                        break;

                    case 'initialized':
                        worker.removeEventListener('message', onMessage);
                        resolve(new Emulator(worker, displayService, crcCheck, e.data.deviceType, env));
                        break;

                    case 'error':
                        worker.removeEventListener('message', onMessage);
                        worker.terminate();
                        reject(new Error(e.data.reason));
                        break;

                    case 'log':
                        log(e.data.message);
                        break;

                    default:
                        console.error('unknown message from worker', e.data);
                        break;
                }
            };

            worker.addEventListener('message', onMessage);
        });
    }

    destroy() {
        this.worker.removeEventListener('message', this.onMessage);
        this.worker.terminate();
    }

    async stop() {
        await this.stopping;

        this.speedDisplay.innerText = '-';
        this.running = false;
        this.eventHandler.release();

        this.stopping = new Promise((resolve) => {
            this.onStopped = resolve;
            this.worker.postMessage({ type: 'stop' });
        });

        await this.stopping;
    }

    start() {
        this.running = true;
        this.eventHandler.bind(this.canvas);
        this.worker.postMessage({ type: 'start' });
    }

    setMaxLoad(maxLoad) {
        this.worker.postMessage({ type: 'setMaxLoad', maxLoad });
    }

    setCyclesPerSecondLimit(cyclesPerSecondLimit) {
        this.worker.postMessage({ type: 'setCyclesPerSecondLimit', cyclesPerSecondLimit });
    }

    penDown(x, y) {
        this.worker.postMessage({ type: 'penDown', x, y });
    }

    penUp() {
        this.worker.postMessage({ type: 'penUp' });
    }

    buttonDown(button) {
        this.worker.postMessage({ type: 'buttonDown', button });
    }

    buttonUp(button) {
        this.worker.postMessage({ type: 'buttonUp', button });
    }

    render(data) {
        if (!this.running) return;

        const imageData = new ImageData(new Uint8ClampedArray(data), 320, this.deviceType === 1 ? 480 : 320);

        this.canvasTmpCtx.putImageData(imageData, 0, 0);
        this.displayService.updateEmulationCanvas(this.canvasTmpCtx.canvas);

        this.worker.postMessage({ type: 'returnFrame', frame: data }, [data]);
    }

    updateSpeedDisplay(text) {
        if (!this.running) return;

        this.speedDisplay.innerText = text;
    }

    setupAudio(port) {
        this.worker.postMessage({ type: 'setupPcm', port }, [port]);
    }

    disablePcm() {
        this.worker.postMessage({ type: 'disablePcm' });
    }

    enablePcm() {
        this.worker.postMessage({ type: 'enablePcm' });
    }

    rotate() {
        this.displayService.rotate();
    }

    getSession() {
        this.worker.postMessage({ type: 'getSession' });

        return new Promise((resolve, reject) => {
            this.worker.addEventListener(
                'message',
                (e) => {
                    const message = e.data;
                    switch (message.type) {
                        case 'session': {
                            const { type, ...rest } = message;
                            resolve(rest);

                            break;
                        }

                        case 'getSessionFailed':
                            reject(new Error('failed to get session'));
                            break;

                        default:
                    }
                },
                { once: true }
            );
        });
    }

    async handleSnapshot(snapshot) {
        const snapshotStatus = snapshot.nand || snapshot.sd ? 'saving' : 'ok';

        if (this.snapshotStatus !== 'failed') {
            this.setSnapshotStatus((this.snapshotStatus = snapshotStatus));
        }

        if (!this.clearSnapshotStatusHandle !== undefined) clearTimeout(this.clearSnapshotStatusHandle);

        let success = true;

        try {
            this.setSnapshotStatus((this.snapshotStatus = snapshotStatus));

            const now = performance.now();
            await this.database.storeSnapshot(snapshot);
            console.log(`save took ${Math.round(performance.now() - now)} msec`);

            if (snapshotStatus !== 'ok') {
                this.clearSnapshotStatusHandle = setTimeout(
                    () => this.setSnapshotStatus((this.snapshotStatus = 'ok')),
                    1500
                );
            }
        } catch (e) {
            console.error(`snapshot failed: ${e}`);
            this.setSnapshotStatus((this.snapshotStatus = 'failed'));
            success = false;
        }

        const transferables = (snapshot) => (snapshot ? [snapshot.scheduledPages, snapshot.pagePool] : []);
        this.worker.postMessage(
            {
                type: 'snapshotDone',
                success,
                snapshot,
            },
            [...transferables(snapshot.nand), ...transferables(snapshot.sd), ...transferables(snapshot.ram)]
        );
    }
}
