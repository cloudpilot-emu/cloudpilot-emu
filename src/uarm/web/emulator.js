import { DisplayService } from './displayservice.js';
import { EventHandler } from './eventhandler.js';

export class Emulator {
    constructor(worker, displayService, crcCheck, { canvas, speedDisplay, log, database, setSnapshotStatus }) {
        this.worker = worker;
        this.displayService = displayService;
        this.database = database;
        this.setSnapshotStatus = setSnapshotStatus;
        this.crcCheck = crcCheck;

        this.speedDisplay = speedDisplay;
        this.log = log;
        this.canvas = canvas;

        this.canvasTmpCtx = document.createElement('canvas').getContext('2d');
        this.canvasTmpCtx.canvas.width = 320;
        this.canvasTmpCtx.canvas.height = 320;

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

                default:
                    console.error('unknown message from worker', message);
                    break;
            }
        };

        this.worker.addEventListener('message', this.onMessage);
        this.eventHandler = new EventHandler(this, this.displayService);
    }

    static async create(nor, nand, sd, maxLoad, cyclesPerSecondLimit, env) {
        const { log, binary, crcCheck } = env;
        const worker = new Worker('web/worker.js');

        const displayService = new DisplayService();
        displayService.initWithCanvas(env.canvas);

        return new Promise((resolve, reject) => {
            const onMessage = (e) => {
                switch (e.data.type) {
                    case 'ready':
                        worker.postMessage({
                            type: 'initialize',
                            nor,
                            nand,
                            sd,
                            maxLoad,
                            cyclesPerSecondLimit,
                            binary,
                            crcCheck,
                        });
                        break;

                    case 'initialized':
                        worker.removeEventListener('message', onMessage);
                        resolve(new Emulator(worker, displayService, crcCheck, env));
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

    stop() {
        this.speedDisplay.innerText = '-';
        this.running = false;
        this.eventHandler.release();
        this.worker.postMessage({ type: 'stop' });
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

        const imageData = new ImageData(new Uint8ClampedArray(data), 320, 320);

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

    async handleSnapshot(snapshot) {
        if (this.snapshotStatus !== 'failed') {
            this.setSnapshotStatus((this.snapshotStatus = 'saving'));
        }

        if (!this.clearSnapshotStatusHandle !== undefined) clearTimeout(this.clearSnapshotStatusHandle);

        let success = true;

        try {
            this.setSnapshotStatus((this.snapshotStatus = 'saving'));

            const now = performance.now();
            await this.database.storeSnapshot(snapshot);
            console.log(`save took ${Math.round(performance.now() - now)} msec`);

            this.clearSnapshotStatusHandle = setTimeout(
                () => this.setSnapshotStatus((this.snapshotStatus = 'ok')),
                1500
            );
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
            [...transferables(snapshot.nand), ...transferables(snapshot.sd)]
        );
    }
}
