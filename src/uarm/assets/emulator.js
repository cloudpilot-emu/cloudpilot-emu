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

class RpcHost {
    constructor(worker) {
        this.worker = worker;
        this.nextRpcId = 0;
        this.pendingCalls = new Map();

        worker.addEventListener('message', this.onMessage.bind(this));
    }

    call(method, args, transferables = undefined) {
        return new Promise((resolve, reject) => {
            const id = this.nextRpcId++;

            this.pendingCalls.set(id, { resolve, reject });

            this.worker.postMessage({ type: 'rpcCall', id, method, args }, transferables);
        });
    }

    onMessage(e) {
        const message = e.data;
        switch (message.type) {
            case 'rpcSuccess':
                this.handleRpcSuccess(message);
                break;

            case 'rpcError':
                this.handleRpcError(message);
                break;

            default:
                break;
        }
    }

    getContext(id) {
        const ctx = this.pendingCalls.get(id);
        if (!ctx) throw new Error(`RPC failed: no context with ID ${id}`);

        this.pendingCalls.delete(id);
        return ctx;
    }

    handleRpcSuccess(message) {
        this.getContext(message.id).resolve(message.result);
    }

    handleRpcError(message) {
        this.getContext(message.id).reject(new Error(message.error));
    }
}

export class Emulator {
    constructor(
        worker,
        rpc,
        displayService,
        crcCheck,
        deviceType,
        cardInserted,
        { canvas, speedDisplay, log, database, setSnapshotStatus }
    ) {
        this.rpc = rpc;

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
        this.cardInserted = cardInserted;
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

                case 'snapshot':
                    this.handleSnapshot(message.snapshot);
                    break;

                case 'session':
                case 'getSessionFailed':
                case 'rpcSuccess':
                case 'rpcError':
                case 'log':
                case 'error':
                    break;

                default:
                    console.error('unknown message from worker', message);
                    break;
            }
        };

        this.worker.addEventListener('message', this.onMessage);
        this.eventHandler = new EventHandler(this, this.displayService);
    }

    static async create(ramSize, nor, nand, sd, cardId, ram, savestate, maxLoad, cyclesPerSecondLimit, env) {
        const { crcCheck } = env;
        const worker = new Worker('assets/worker.js');
        const rpc = new RpcHost(worker);

        const displayService = new DisplayService();
        displayService.initWithCanvas(env.canvas);

        const onMessage = (e) => {
            switch (e.data.type) {
                case 'log':
                    env.log(e.data.message);
                    break;

                case 'error':
                    console.error(e.data.reason);
                    break;
            }
        };

        worker.addEventListener('message', onMessage);

        try {
            const { deviceType, cardInserted } = await rpc.call(
                'initialize',
                {
                    ramSize,
                    nor,
                    nand,
                    sd,
                    cardId,
                    ram,
                    savestate,
                    maxLoad,
                    cyclesPerSecondLimit,
                    crcCheck,
                    module: await module,
                },
                [
                    nor.buffer,
                    nand.buffer,
                    ...(sd ? [sd.buffer] : []),
                    ...(ram ? [ram.buffer] : []),
                    ...(savestate ? [savestate.buffer] : []),
                ]
            );

            return new Emulator(worker, rpc, displayService, crcCheck, deviceType, cardInserted, env);
        } catch (e) {
            worker.terminate();

            throw e;
        }
    }

    destroy() {
        this.worker.removeEventListener('message', this.onMessage);
        this.worker.terminate();
    }

    async stop() {
        await this.rpc.call('stop');

        this.speedDisplay.innerText = '-';
        this.running = false;
        this.eventHandler.release();
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
        return this.rpc.call('getSession');
    }

    async ejectCard() {
        if (!this.cardInserted) throw new Error('no card inserted');

        await this.rpc.call('ejectCard');
        this.cardInserted = false;
    }

    async insertCard(data, cardId) {
        if (this.cardInserted) throw new Error('card already inserted');

        await this.rpc.call('insertCard', { data, cardId }, [data.buffer]);
        this.cardInserted = true;
    }

    async reset() {
        await this.rpc.call('reset');
    }

    install(files) {
        files = files.filter((file) => /\.(prc|pdb)$/i.test(file.name));

        return this.rpc.call(
            'install',
            files,
            files.map((file) => file.content.buffer)
        );
    }

    backup(type) {
        return this.rpc.call('backup', { type });
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
