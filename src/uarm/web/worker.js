importScripts('../src/uarm_web.js', './setimmediate/setimmediate.js', './crc.js');

(function () {
    const PCM_BUFFER_SIZE = (44100 / 60) * 10;
    const INITIAL_NAND_PAGE_POOL_SIZE = 256; // 1MB + OOB
    const NAND_PAGE_POOL_GROWTH_FACTOR = 1.5;

    const messageQueue = [];
    let dispatchInProgress = false;

    let emulator;

    let framePool = [];
    let pcmPool = [];

    class Emulator {
        constructor(module, maxLoad, cyclesPerSecondLimit, crcCheck, { onSpeedDisplay, onFrame, log, onSnapshot }) {
            this.module = module;
            this.onSpeedDisplay = onSpeedDisplay;
            this.onFrame = onFrame;
            this.log = log;
            this.onSnapshot = onSnapshot;
            this.crcCheck = crcCheck;

            this.cycle = module.cwrap('cycle', undefined, ['number']);
            this.getFrame = module.cwrap('getFrame', 'number', []);
            this.resetFrame = module.cwrap('resetFrame', undefined, []);
            this.currentIps = module.cwrap('currentIps', 'number', []);
            this.currentIpsMax = module.cwrap('currentIpsMax', 'number', []);
            this.setMaxLoad = module.cwrap('setMaxLoad', undefined, ['number']);
            this.setCyclesPerSecondLimit = module.cwrap('setCyclesPerSecondLimit', undefined, ['number']);
            this.getTimesliceSizeUsec = module.cwrap('getTimesliceSizeUsec', 'number', []);
            this.getTimestampUsec = module.cwrap('getTimestampUsec', 'number', []);
            this.penDown = module.cwrap('penDown', undefined, ['number', 'number']);
            this.penUp = module.cwrap('penUp', undefined, []);
            this.keyDown = module.cwrap('keyDown', undefined, ['number']);
            this.keyUp = module.cwrap('keyUp', undefined, ['number']);
            this.pendingSamples = module.cwrap('pendingSamples', 'number', []);
            this.popQueuedSamples = module.cwrap('popQueuedSamples', 'number', []);
            this.setPcmOutputEnabled = module.cwrap('setPcmOutputEnabled', undefined, ['number']);
            this.setPcmSuspended = module.cwrap('setPcmSuspended', undefined, ['number']);
            this.getNandDataSize = module.cwrap('getNandDataSize', 'number');
            this.getNandDataPtr = module.cwrap('getNandData', 'number');
            this.getNandDirtyPagesSize = module.cwrap('getNandDirtyPagesSize', 'number');
            this.getNandDirtyPagesPtr = module.cwrap('getNandDirtyPages', 'number');

            this.amIDead = false;
            this.pcmEnabled = false;
            this.pcmPort = undefined;

            this.setMaxLoad(maxLoad);
            this.setCyclesPerSecondLimit(cyclesPerSecondLimit);

            const nandSize = this.getNandDataSize();
            this.nandScheduledPages = new Uint32Array((nandSize / 4224) | 0);
            this.nandPagePool = new Uint8Array(4224 * INITIAL_NAND_PAGE_POOL_SIZE);
            this.snapshotPending = false;
        }

        static async create(nor, nand, sd, maxLoad, cyclesPerSecondLimit, crcCheck, env) {
            const { log, binary } = env;
            let module;

            module = await createModule({
                noInitialRun: true,
                print: log,
                printErr: log,
                locateFile: () => `../src/${binary}`,
            });

            const malloc = module.cwrap('malloc', 'number', ['number']);

            let norPtr = malloc(nor.length);
            let nandPtr = malloc(nand.length);
            let sdPtr = sd ? malloc(sd.length) : 0;

            module.HEAPU8.subarray(norPtr, norPtr + nor.length).set(nor);
            module.HEAPU8.subarray(nandPtr, nandPtr + nand.length).set(nand);
            if (sd) module.HEAPU8.subarray(sdPtr, sdPtr + sd.length).set(sd);

            module.callMain([]);

            module.ccall(
                'webMain',
                undefined,
                ['number', 'number', 'number', 'number', 'number', 'number'],
                [norPtr, nor.length, nandPtr, nand.length, sdPtr, sd ? sd.length : 0]
            );

            return new Emulator(module, maxLoad, cyclesPerSecondLimit, crcCheck, env);
        }

        stop() {
            if (this.amIDead) return;

            if (this.immediateHandle) clearImmediate(this.immediateHandle);
            if (this.timeoutHandle) clearTimeout(this.timeoutHandle);

            this.timeoutHandle = this.immediateHandle = undefined;

            this.log('emulator stopped');
        }

        start() {
            if (this.amIDead) return;

            if (this.timeoutHandle || this.immediateHandle) return;
            this.lastSnapshot = Number(this.getTimestampUsec());

            const schedule = () => {
                const now64 = this.getTimestampUsec();
                const now = Number(now64);

                try {
                    this.cycle(now64);
                } catch (e) {
                    this.amIDead = true;
                    console.error(e);
                    this.log(e.message);

                    return;
                }

                this.render();
                this.processAudio();

                const timesliceRemainning =
                    (this.getTimesliceSizeUsec() - Number(this.getTimestampUsec()) + now) / 1000;
                this.timeoutHandle = this.immediateHandle = undefined;

                if (timesliceRemainning < 5) this.immediateHandle = setImmediate(schedule);
                else this.timeoutHandle = setTimeout(schedule, timesliceRemainning);

                if (now - this.lastSnapshot > 1000000) {
                    this.snapshot();
                    this.updateSpeedDisplay();

                    this.lastSnapshot = now;
                }
            };

            this.log('emulator running');
            schedule();
        }

        getNandData() {
            const size = this.getNandDataSize();
            const ptr = this.getNandDataPtr();

            return this.module.HEAPU8.subarray(ptr, ptr + size);
        }

        getNandDirtyPages() {
            const size32 = this.getNandDirtyPagesSize() >>> 2;
            const ptr32 = this.getNandDirtyPagesPtr() >>> 2;

            return this.module.HEAPU32.subarray(ptr32, ptr32 + size32);
        }

        growNandPagePool() {
            const newPagePool = new Uint8Array(
                Math.min(
                    Math.ceil((this.nandPagePool.length * NAND_PAGE_POOL_GROWTH_FACTOR) / 4224) * 4224,
                    this.nandScheduledPages.length * 4224
                )
            );

            console.log(`growing NAND page pool to ${newPagePool.length} bytes`);

            newPagePool.set(this.nandPagePool);
            this.nandPagePool = newPagePool;
        }

        snapshot() {
            if (this.snapshotPending) {
                console.warn('snapshot pending, skipping...');
            }

            const nandData = this.getNandData();
            const nandDirtyPages = this.getNandDirtyPages();

            let iPage = 0;
            let page = 0;
            for (let i = 0; i < nandDirtyPages.length; i++) {
                let pageSet32 = nandDirtyPages[i];

                if (pageSet32 === 0) {
                    page += 32;
                    continue;
                }

                for (let j = 0; j < 4; j++) {
                    let pageSet8 = pageSet32 & 0xff;
                    pageSet32 >>>= 8;

                    if (pageSet8 === 0) {
                        page += 8;
                        continue;
                    }

                    for (let mask = 1; mask < 0x100; mask <<= 1) {
                        if (!pageSet8 & mask) {
                            page++;
                            continue;
                        }

                        this.nandScheduledPages[iPage] = page;
                        const pagePoolOffset = iPage * 4224;
                        const nandDataOffset = page * 4224;

                        if (pagePoolOffset >= this.nandPagePool.length) this.growNandPagePool();

                        this.nandPagePool
                            .subarray(pagePoolOffset, pagePoolOffset + 4224)
                            .set(nandData.subarray(nandDataOffset, nandDataOffset + 4224));

                        iPage++;
                        page++;
                    }
                }
            }

            if (iPage > 0) {
                nandDirtyPages.fill(0);
                this.snapshotPending = true;

                const crc = this.crcCheck ? crc32(nandData) : undefined;

                this.onSnapshot(iPage, this.nandScheduledPages.buffer, this.nandPagePool.buffer, crc);
            }
        }

        snapshotDone(success, nandScheduledPageCount, nandScheduledPages, nandPagePool) {
            this.nandScheduledPages = new Uint32Array(nandScheduledPages);
            this.nandPagePool = new Uint8Array(nandPagePool);
            this.snapshotPending = false;

            if (success) return;

            console.log('snapshot failed');

            const nandDirtyPages = this.getNandDirtyPages();

            for (let i = 0; i < nandScheduledPageCount; i++) {
                const page = this.nandScheduledPages[i];

                nandDirtyPages[page >>> 5] |= 1 << (page & 0x1f);
            }
        }

        render() {
            if (!this.module) return;

            const framePtr = this.getFrame() >>> 2;
            if (!framePtr) return;

            const frame = this.module.HEAPU32.subarray(framePtr, framePtr + 320 * 320);

            if (framePool.length === 0) {
                this.onFrame(frame.slice().buffer);
            } else {
                const frameCopy = new Uint32Array(framePool.pop());
                frameCopy.set(frame);

                this.onFrame(frameCopy.buffer);
            }

            this.resetFrame();
        }

        getPcmBuffer() {
            if (pcmPool.length > 0) return new Uint32Array(pcmPool.pop());

            return new Uint32Array(PCM_BUFFER_SIZE);
        }

        processAudio() {
            if (!this.module || !this.pcmPort || !this.pcmEnabled) return;

            const pendingSamples = this.pendingSamples();
            if (pendingSamples === 0) return;

            const samplesPtr = this.popQueuedSamples() >>> 2;
            const samples = this.getPcmBuffer();

            samples.set(
                this.module.HEAPU32.subarray(samplesPtr, samplesPtr + Math.min(pendingSamples, samples.length))
            );

            this.pcmPort.postMessage({ type: 'sample-data', count: pendingSamples, buffer: samples.buffer }, [
                samples.buffer,
            ]);
        }

        updateSpeedDisplay() {
            const currentIps = this.currentIps();
            const currentIpsMax = Number(this.currentIpsMax());

            this.onSpeedDisplay(
                `current ${(currentIps / 1e6).toFixed(2)} MIPS, limit ${(currentIpsMax / 1e6).toFixed(2)} MIPS -> ${(
                    (currentIps / currentIpsMax) *
                    100
                ).toFixed(2)}%`
            );
        }

        setupPcm(port) {
            this.pcmPort = port;
            this.setPcmOutputEnabled(true);
            this.pcmEnabled = true;

            this.pcmPort.onmessage = (evt) => this.handlePcmMessage(evt.data);
        }

        handlePcmMessage(message) {
            switch (message.type) {
                case 'suspend-pcm':
                    this.setPcmSuspended(true);
                    break;

                case 'resume-pcm':
                    this.setPcmSuspended(false);
                    break;

                case 'return-buffer':
                    pcmPool.push(message.buffer);
                    break;

                default:
                    console.error(`worker: invalid PCM message ${message.type}`);
                    break;
            }
        }
    }

    function mapButton(name) {
        switch (name) {
            case 'cal':
                return 1;

            case 'phone':
                return 2;

            case 'todo':
                return 3;

            case 'notes':
                return 4;

            case 'up':
                return 5;

            case 'down':
                return 6;

            default:
                return -1;
        }
    }

    function postReady() {
        postMessage({ type: 'ready' });
    }

    function postFrame(data) {
        postMessage({ type: 'frame', data }, [data]);
    }

    function postSpeed(text) {
        postMessage({ type: 'speed', text });
    }

    function postLog(message) {
        postMessage({ type: 'log', message });
    }

    function postError(reason) {
        postMessage({ type: 'error', reason });
    }

    function postInitialized() {
        postMessage({ type: 'initialized' });
    }

    function postSnapshot(nandScheduledPageCount, nandScheduledPages, nandPagePool, crc) {
        postMessage(
            {
                type: 'snapshot',
                nandScheduledPageCount,
                nandScheduledPages,
                nandPagePool,
                crc,
            },
            [nandScheduledPages, nandPagePool]
        );
    }

    async function handleMessage(message) {
        let assertEmulator = (context) => {
            if (!emulator) {
                throw new Error(`${context}: emulator not running`);
            }
        };

        switch (message.type) {
            case 'initialize':
                emulator = await Emulator.create(
                    message.nor,
                    message.nand,
                    message.sd,
                    message.maxLoad,
                    message.cyclesPerSecondLimit,
                    message.crcCheck,
                    {
                        onFrame: postFrame,
                        onSpeedDisplay: postSpeed,
                        onSnapshot: postSnapshot,
                        log: postLog,
                        binary: message.binary,
                    }
                );

                postInitialized();

                break;

            case 'start':
                assertEmulator('start');
                emulator.start();
                break;

            case 'stop':
                assertEmulator('stop');
                emulator.stop();
                break;

            case 'setMaxLoad':
                assertEmulator('setMaxLoad');
                emulator.setMaxLoad(message.maxLoad);
                break;

            case 'setCyclesPerSecondLimit':
                assertEmulator('cyclesPerSecondLimit');
                emulator.setCyclesPerSecondLimit(message.cyclesPerSecondLimit);
                break;

            case 'penDown':
                assertEmulator('penDown');
                emulator.penDown(message.x, message.y);
                break;

            case 'penUp':
                assertEmulator('penUp');
                emulator.penUp();
                break;

            case 'buttonDown': {
                const button = mapButton(message.button);
                if (button < 0) {
                    console.error(`ignoring unknown button ${message.button}`);
                    return;
                }

                assertEmulator('buttonDown');

                emulator.keyDown(button);
                break;
            }

            case 'buttonUp': {
                const button = mapButton(message.button);
                if (button < 0) {
                    console.error(`ignoring unknown button ${message.button}`);
                    return;
                }

                assertEmulator('buttonUp');

                emulator.keyUp(button);
                break;
            }

            case 'returnFrame':
                framePool.push(message.frame);
                break;

            case 'setupPcm':
                assertEmulator('setupPcm');

                emulator.setupPcm(message.port);
                break;

            case 'disablePcm':
                assertEmulator('disablePcm');

                emulator.setPcmOutputEnabled(false);
                break;

            case 'enablePcm':
                assertEmulator('enablePcm');

                emulator.setPcmOutputEnabled(true);
                break;

            case 'snapshotDone':
                assertEmulator('snapshotDone');

                emulator.snapshotDone(
                    message.success,
                    message.nandScheduledPageCount,
                    message.nandScheduledPages,
                    message.nandPagePool
                );
                break;

            default:
                console.error('unknown message from main thread', message);
        }
    }

    async function dispatchMessages() {
        if (dispatchInProgress || messageQueue.length === 0) return;
        dispatchInProgress = true;

        while (messageQueue.length > 0) {
            try {
                await handleMessage(messageQueue.shift());
            } catch (e) {
                postError(e);
            }
        }

        dispatchInProgress = false;
    }

    async function main() {
        postReady();

        onmessage = (e) => {
            messageQueue.push(e.data);
            dispatchMessages();
        };
    }

    main().catch((e) => console.error(e));
})();
