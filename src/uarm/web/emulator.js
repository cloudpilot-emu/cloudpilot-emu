import { DisplayService } from './displayservice.js';
import { EventHandler } from './eventhandler.js';

const isMacOSSafari = 'safari' in window;
const isIOS =
    !!navigator.platform.match(/iPhone|iPad|iPod/) ||
    (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
const isWebkit =
    isMacOSSafari || isIOS || (!!navigator.userAgent.match(/Safari/) && !navigator.userAgent.match(/Chrome/));

export class Emulator {
    constructor(worker, displayService, { canvas, speedDisplay, log }) {
        this.worker = worker;
        this.displayService = displayService;

        this.speedDisplay = speedDisplay;
        this.log = log;
        this.canvas = canvas;

        this.canvasTmpCtx = document.createElement('canvas').getContext('2d');
        this.canvasTmpCtx.canvas.width = 320;
        this.canvasTmpCtx.canvas.height = 320;

        this.running = false;

        this.onMessage = (e) => {
            switch (e.data.type) {
                case 'frame':
                    this.render(e.data.data);
                    break;

                case 'speed':
                    this.updateSpeedDisplay(e.data.text);
                    break;

                case 'log':
                    this.log(e.data.message);
                    break;

                case 'error':
                    console.error(e.data.reason);
                    break;

                default:
                    console.error('unknown message from worker', e.data);
                    break;
            }
        };

        this.worker.addEventListener('message', this.onMessage);
        this.eventHandler = new EventHandler(this, this.displayService);
    }

    static async create(nor, nand, sd, env) {
        const { log } = env;
        const worker = new Worker('web/worker.js');

        const displayService = new DisplayService();
        displayService.initWithCanvas(env.canvas);

        return new Promise((resolve, reject) => {
            const onMessage = (e) => {
                switch (e.data.type) {
                    case 'ready':
                        worker.postMessage({ type: 'initialize', nor, nand, sd, isWebkit });
                        break;

                    case 'initialized':
                        worker.removeEventListener('message', onMessage);
                        resolve(new Emulator(worker, displayService, env));
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
}
