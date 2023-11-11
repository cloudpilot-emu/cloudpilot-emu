import { EventHandler } from './eventhandler.js';

export class Emulator {
    constructor(worker, { canvasCtx, speedDisplay, log }) {
        this.worker = worker;

        this.canvasCtx = canvasCtx;
        this.speedDisplay = speedDisplay;
        this.log = log;

        this.canvasTmpCtx = document.createElement('canvas').getContext('2d');
        this.canvasTmpCtx.canvas.width = 320;
        this.canvasTmpCtx.canvas.height = 320;

        this.running = false;

        this.eventHandler = new EventHandler(this, canvasCtx.canvas);

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
    }

    static create(nor, nand, sd, env) {
        const { log } = env;
        const worker = new Worker('web/worker.js');

        return new Promise((resolve, reject) => {
            const onMessage = (e) => {
                switch (e.data.type) {
                    case 'ready':
                        worker.postMessage({ type: 'initialize', nor, nand, sd });
                        break;

                    case 'initialized':
                        worker.removeEventListener('message', onMessage);
                        resolve(new Emulator(worker, env));
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
        this.eventHandler.stop();
        this.worker.removeEventListener('message', this.onMessage);
        this.worker.terminate();
    }

    stop() {
        this.eventHandler.stop();
        this.speedDisplay.innerText = '-';
        this.running = false;
        this.worker.postMessage({ type: 'stop' });
    }

    start() {
        this.running = true;
        this.eventHandler.start();
        this.worker.postMessage({ type: 'start' });
    }

    penDown(x, y) {
        this.worker.postMessage({ type: 'penDown', x, y });
    }

    penUp() {
        this.worker.postMessage({ type: 'penUp' });
    }

    render(data) {
        if (!this.running) return;

        const imageData = new ImageData(new Uint8ClampedArray(data), 320, 320);

        this.canvasTmpCtx.putImageData(imageData, 0, 0);
        this.canvasCtx.imageSmoothingEnabled = false;
        this.canvasCtx.drawImage(this.canvasTmpCtx.canvas, 0, 0, 320, 320, 0, 0, 640, 640);

        this.worker.postMessage({ type: 'returnFrame', frame: data }, [data]);
    }

    updateSpeedDisplay(text) {
        if (!this.running) return;

        this.speedDisplay.innerText = text;
    }
}
