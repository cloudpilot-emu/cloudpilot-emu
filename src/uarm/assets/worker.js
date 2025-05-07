importScripts('../uarm_web.js', './setimmediate/setimmediate.js', './crc.js');

(function () {
    class RpcClient {
        constructor(worker) {
            this.worker = worker;
            this.registeredMethods = new Map();
        }

        async dispatch(message) {
            const { id, method, args } = message;

            if (!this.registeredMethods.has(method)) {
                this.worker.postMessage({ type: 'rpcError', id, error: `method not registered: ${method}` });
            }

            try {
                const result = await this.registeredMethods.get(method)(args);
                this.worker.postMessage({ type: 'rpcSuccess', id, result });
            } catch (e) {
                this.worker.postMessage({ type: 'rpcError', id, error: `${e}` });
            }
        }

        register(method, handler) {
            this.registeredMethods.set(method, handler);

            return this;
        }
    }

    const PCM_BUFFER_SIZE = (44100 / 60) * 10;
    const INITIAL_PAGE_POOL_PAGES = 256;
    const PAGE_POOL_GROWTH_FACTOR = 1.5;

    let rpcClient;
    let emulator;

    let framePool = [];
    let pcmPool = [];

    function installResult(result, name) {
        switch (result) {
            case 1:
                return `${name} installed successfully, reset advised`;

            case 0:
                return `${name} installed successfully`;

            case -2:
                return `${name} failed to install: corrupt file`;

            case -3:
                return `${name} failed to install: out of memory`;

            case -4:
                return `${name} failed to install: db is open`;

            case -5:
                return `${name} failed to install: db cannot be overwritten`;

            case -6:
                return `${name} failed to install: installing databases not supported on ROM`;

            case -7:
                return `${name} failed to install: db is currently not possible`;

            default:
                return `${name} failed to install: unknown error`;
        }
    }

    class DirtyPageTracker {
        constructor({ pageSize, pageCount, getDataPtr, getDirtyPagesPtr, isDirty, setDirty, module, name, crcCheck }) {
            this.pageSize = pageSize;
            this.pageCount = pageCount;

            this.getDataPtr = getDataPtr;
            this.getDirtyPagesPtr = getDirtyPagesPtr;
            this.isDirty = isDirty;
            this.setDirty = setDirty;
            this.module = module;

            this.size = pageSize * pageCount;
            this.dirtyPagesSize4 = (pageCount / 32) | 0;
            if (this.dirtyPagesSize4 * 32 < pageCount) this.dirtyPagesSize4++;

            this.name = name;
            this.crcCheck = crcCheck;

            this.scheduledPages = new Uint32Array(pageCount);
            this.pagePool = new Uint8Array(INITIAL_PAGE_POOL_PAGES * pageSize);
            this.scheduledPageCount = 0;
        }

        getData() {
            const ptr = this.getDataPtr();

            return this.module.HEAPU8.subarray(ptr, ptr + this.size);
        }

        getDirtyPages() {
            const ptr32 = this.getDirtyPagesPtr() >>> 2;

            return this.module.HEAPU32.subarray(ptr32, ptr32 + this.dirtyPagesSize4);
        }

        takeSnapshot() {
            if (this.scheduledPageCount > 0) {
                console.warn(`${this.name} pending, skipping...`);
            }

            if (!this.isDirty()) return undefined;

            const data = this.getData();
            const dirtyPages = this.getDirtyPages();

            let iPage = 0;
            let page = 0;
            for (let i = 0; i < dirtyPages.length; i++) {
                let pageSet32 = dirtyPages[i];

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
                        if (!(pageSet8 & mask)) {
                            page++;
                            continue;
                        }

                        this.scheduledPages[iPage] = page;
                        const pagePoolOffset = iPage * this.pageSize;
                        const dataOffset = page * this.pageSize;

                        if (pagePoolOffset >= this.pagePool.length) this.growPagePool();

                        this.pagePool
                            .subarray(pagePoolOffset, pagePoolOffset + this.pageSize)
                            .set(data.subarray(dataOffset, dataOffset + this.pageSize));

                        iPage++;
                        page++;
                    }
                }

                dirtyPages[i] = 0;
            }

            this.scheduledPageCount = iPage;

            if (this.scheduledPageCount > 0) {
                this.setDirty(false);

                return {
                    scheduledPageCount: this.scheduledPageCount,
                    scheduledPages: this.scheduledPages.buffer,
                    pagePool: this.pagePool.buffer,
                    crc: this.crcCheck ? crc32(data) : undefined,
                };
            } else {
                return undefined;
            }
        }

        getTransferables() {
            return this.scheduledPageCount > 0 ? [this.scheduledPages.buffer, this.pagePool.buffer] : [];
        }

        onSnapshotDone(success, snapshot) {
            if (!snapshot || this.scheduledPagesCount === 0) return;

            const scheduledPageCount = this.scheduledPageCount;

            this.scheduledPages = new Uint32Array(snapshot.scheduledPages);
            this.pagePool = new Uint8Array(snapshot.pagePool);
            this.scheduledPageCount = 0;

            if (success) return;

            console.log(`${this.name} failed`);

            const dirtyPages = this.getDirtyPages();

            for (let i = 0; i < scheduledPageCount; i++) {
                const page = this.scheduledPages[i];

                dirtyPages[page >>> 5] |= 1 << (page & 0x1f);
            }

            this.setDirty(true);
        }

        growPagePool() {
            const newPagePool = new Uint8Array(
                Math.min(
                    Math.ceil((this.pagePool.length * PAGE_POOL_GROWTH_FACTOR) / this.pageSize) * this.pageSize,
                    this.pageCount * this.pageSize
                )
            );

            console.log(`growing ${this.name} page pool to ${newPagePool.length} bytes`);

            newPagePool.set(this.pagePool);
            this.pagePool = newPagePool;
        }
    }

    class Emulator {
        constructor(
            module,
            maxLoad,
            cyclesPerSecondLimit,
            crcCheck,
            cardId,
            { onSpeedDisplay, onFrame, log, postSnapshot }
        ) {
            this.module = module;
            this.onSpeedDisplay = onSpeedDisplay;
            this.onFrame = onFrame;
            this.log = log;
            this.postSnapshot = postSnapshot;
            this.cardId = cardId;
            this.crcCheck = crcCheck;

            this.malloc = module.cwrap('malloc', 'number', ['number']);
            this.free = module.cwrap('free', undefined, ['number']);
            this.cycle = module.cwrap('cycle', undefined, ['number']);
            this.getFrame = module.cwrap('getFrame', 'number');
            this.resetFrame = module.cwrap('resetFrame');
            this.currentIps = module.cwrap('currentIps', 'number');
            this.currentIpsMax = module.cwrap('currentIpsMax', 'number');
            this.setMaxLoad = module.cwrap('setMaxLoad', undefined, ['number']);
            this.setCyclesPerSecondLimit = module.cwrap('setCyclesPerSecondLimit', undefined, ['number']);
            this.getTimesliceSizeUsec = module.cwrap('getTimesliceSizeUsec', 'number');
            this.getTimestampUsec = module.cwrap('getTimestampUsec', 'number');
            this.penDown = module.cwrap('penDown', undefined, ['number', 'number']);
            this.penUp = module.cwrap('penUp');
            this.keyDown = module.cwrap('keyDown', undefined, ['number']);
            this.keyUp = module.cwrap('keyUp', undefined, ['number']);
            this.pendingSamples = module.cwrap('pendingSamples', 'number');
            this.popQueuedSamples = module.cwrap('popQueuedSamples', 'number');
            this.setPcmOutputEnabled = module.cwrap('setPcmOutputEnabled', undefined, ['number']);
            this.setPcmSuspended = module.cwrap('setPcmSuspended', undefined, ['number']);
            this.getRomDataSize = module.cwrap('getRomDataSize', 'number');
            this.getRomData = module.cwrap('getRomData', 'number');
            this.getNandDataSize = module.cwrap('getNandDataSize', 'number');
            this.getNandData = module.cwrap('getNandData', 'number');
            this.getSdCardDataSize = module.cwrap('getSdCardDataSize', 'number');
            this.getRamData = module.cwrap('getRamData', 'number');
            this.getRamDataSize = module.cwrap('getRamDataSize', 'number');
            this.clearRamDirtyPages = module.cwrap('clearRamDirtyPages');
            this.getDeviceType = module.cwrap('getDeviceType');
            this.sdCardInsert = module.cwrap('sdCardInsert', 'number', ['number', 'number', 'string']);
            this.sdCardEject = module.cwrap('sdCardEject');
            this.isSdInserted = module.cwrap('isSdInserted');
            this.reset = module.cwrap('reset');
            this.save = module.cwrap('save');
            this.getSavestateSize = module.cwrap('getSavestateSize', 'number');
            this.getSavestateData = module.cwrap('getSavestateData', 'number');
            this.getRamSize = module.cwrap('getRamSize', 'number');
            this.installDatabase = module.cwrap('installDatabase', 'number', ['number', 'number']);

            this.amIDead = false;
            this.pcmEnabled = false;
            this.pcmPort = undefined;
            this.snapshotPending = false;
            this.snapshotPromise = Promise.resolve();
            this.resolveSnapshot = () => undefined;
            this.deviceType = this.getDeviceType();
            this.savestate = undefined;

            this.setMaxLoad(maxLoad);
            this.setCyclesPerSecondLimit(cyclesPerSecondLimit);

            const nandSize = this.getNandDataSize();
            this.nandTracker = new DirtyPageTracker({
                pageSize: 4224,
                pageCount: (nandSize / 4224) | 0,
                name: 'NAND snapshot',
                crcCheck,
                getDataPtr: this.getNandData,
                getDirtyPagesPtr: module.cwrap('getNandDirtyPages', 'number'),
                isDirty: module.cwrap('isNandDirty', 'number'),
                setDirty: module.cwrap('setNandDirty', undefined, ['number']),
                module,
            });

            this.setupSdCardTracker();

            const ramSize = this.getRamDataSize();
            this.ramTracker = new DirtyPageTracker({
                pageSize: 512,
                pageCount: (ramSize / 512) | 0,
                name: 'RAM snapshot',
                crcCheck,
                getDataPtr: this.getRamData,
                getDirtyPagesPtr: module.cwrap('getRamDirtyPages', 'number'),
                isDirty: () => true,
                setDirty: () => undefined,
                module,
            });
        }

        static async create(
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
            wasmModule,
            env
        ) {
            const { log } = env;
            let module;

            module = await createModule({
                noInitialRun: true,
                print: log,
                printErr: log,
                instantiateWasm: (imports, callback) => WebAssembly.instantiate(wasmModule, imports).then(callback),
            });

            const malloc = module.cwrap('malloc', 'number', ['number']);
            const free = module.cwrap('free', undefined, ['number']);
            const norPtr = malloc(nor.length);
            const nandPtr = malloc(nand.length);
            const sdPtr = sd ? malloc(sd.length) : 0;
            const savestatePtr = savestate ? malloc(savestate.length) : 0;
            const ramPtr = ram ? malloc(ram.length) : 0;

            module.HEAPU8.subarray(norPtr, norPtr + nor.length).set(nor);
            module.HEAPU8.subarray(nandPtr, nandPtr + nand.length).set(nand);
            if (sd) module.HEAPU8.subarray(sdPtr, sdPtr + sd.length).set(sd);
            if (savestate) module.HEAPU8.subarray(savestatePtr, savestatePtr + savestate.length).set(savestate);
            if (ram) module.HEAPU8.subarray(ramPtr, ramPtr + ram.length).set(ram);

            module.callMain([]);
            module.ccall(
                'webMain',
                undefined,
                [
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'number',
                    'string',
                ],
                [
                    ramSize,
                    norPtr,
                    nor.length,
                    nandPtr,
                    nand.length,
                    ramPtr,
                    ramPtr ? ram.length : 0,
                    sdPtr,
                    sd?.length ?? 0,
                    savestatePtr,
                    savestate?.length ?? 0,
                    cardId ?? '',
                ]
            );

            if (savestatePtr) free(savestatePtr);
            if (ramPtr) free(ramPtr);

            return new Emulator(module, maxLoad, cyclesPerSecondLimit, crcCheck, cardId, env);
        }

        setupSdCardTracker() {
            const sdCardSize = this.getSdCardDataSize();
            this.sdCardTracker = new DirtyPageTracker({
                pageSize: 8192,
                pageCount: (sdCardSize / 8192) | 0,
                name: 'SD card snapshot',
                crcCheck: this.crcCheck,
                getDataPtr: this.module.cwrap('getSdCardData', 'number'),
                getDirtyPagesPtr: this.module.cwrap('getSdCardDirtyPages', 'number'),
                isDirty: this.module.cwrap('isSdCardDirty', 'number'),
                setDirty: this.module.cwrap('setSdCardDirty', undefined, ['number']),
                module: this.module,
            });
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
                    this.triggerSnapshot();
                    this.updateSpeedDisplay();

                    this.lastSnapshot = now;
                }
            };

            this.log('emulator running');
            schedule();
        }

        triggerSnapshot() {
            if (this.snapshotPending) return;

            this.save();
            const savestatePtr = this.getSavestateData();
            const savestateSize = this.getSavestateSize();

            if (!this.savestate) this.savestate = new Uint8Array(savestateSize);
            this.savestate.set(this.module.HEAPU8.subarray(savestatePtr, savestatePtr + savestateSize));

            const snapshotNand = this.nandTracker.takeSnapshot();
            const snapshotSd = this.sdCardTracker.takeSnapshot();
            const snapshotRam = this.ramTracker.takeSnapshot();

            if (!snapshotNand && !snapshotSd && !snapshotRam) return;

            this.snapshotPromise = new Promise((resolve) => (this.resolveSnapshot = resolve));

            this.postSnapshot(
                {
                    nand: snapshotNand,
                    sd: snapshotSd,
                    ram: snapshotRam,
                    cardId: this.cardId,
                    savestate: this.savestate.buffer,
                    ramSize: this.getRamSize(),
                },
                [
                    ...this.nandTracker.getTransferables(),
                    ...this.sdCardTracker.getTransferables(),
                    ...this.ramTracker.getTransferables(),
                    this.savestate.buffer,
                ]
            );
            this.snapshotPending = true;
        }

        snapshotDone(success, snapshot) {
            this.nandTracker.onSnapshotDone(success, snapshot.nand);
            this.sdCardTracker.onSnapshotDone(success, snapshot.sd);
            this.ramTracker.onSnapshotDone(success, snapshot.ram);
            this.savestate = new Uint8Array(snapshot.savestate);

            this.snapshotPending = false;

            this.resolveSnapshot();
        }

        render() {
            if (!this.module) return;

            const framePtr = this.getFrame() >>> 2;
            if (!framePtr) return;

            const frame = this.module.HEAPU32.subarray(framePtr, framePtr + 320 * (this.deviceType === 1 ? 480 : 320));

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

        getSession() {
            this.save();

            const deviceId = this.getDeviceType();
            const nor = this.module.HEAPU8.subarray(this.getRomData(), this.getRomData() + this.getRomDataSize());
            const nand = this.module.HEAPU8.subarray(this.getNandData(), this.getNandData() + this.getNandDataSize());
            const ram = this.module.HEAPU8.subarray(this.getRamData(), this.getRamData() + this.getRamDataSize());
            const savestate = this.module.HEAPU8.subarray(
                this.getSavestateData(),
                this.getSavestateData() + this.getSavestateSize()
            );
            const ramSize = this.getRamSize();

            return { deviceId, nor, nand, ram, savestate, ramSize };
        }

        ejectCard() {
            this.sdCardEject();
        }

        async insertCard(data, cardId) {
            await this.snapshotPromise;

            if (this.isSdInserted()) throw new Error('SD card already inserted');

            const ptr = this.malloc(data.length);
            this.module.HEAPU8.subarray(ptr, ptr + data.length).set(data);

            if (!this.sdCardInsert(ptr, data.length, cardId)) {
                this.free(ptr);
                throw new Error('failed to insert SD card');
            }

            this.cardId = cardId;
            this.setupSdCardTracker();
        }

        async install(files) {
            const running = !!this.timeoutHandle;
            if (running) this.stop();

            await this.snapshotPromise;
            this.triggerSnapshot();
            await this.snapshotPromise;

            for (const file of files) {
                try {
                    const filePtr = this.malloc(file.content.length);
                    if (!filePtr) throw new Error('unable to allocate memory');

                    this.module.HEAPU8.subarray(filePtr, filePtr + file.content.length).set(file.content);
                    const result = this.installDatabase(file.content.length, filePtr);
                    this.free(filePtr);

                    this.log(installResult(result, file.name));
                } catch (e) {
                    this.log(`fatal error during installation: ${e}`);

                    return file.name;
                }
            }

            this.triggerSnapshot();
            if (running) this.start();

            return undefined;
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

    function postSnapshot(snapshot, transferables) {
        postMessage(
            {
                type: 'snapshot',
                snapshot,
            },
            transferables
        );
    }

    function assertEmulator(context) {
        if (!emulator) {
            throw new Error(`${context}: emulator not running`);
        }
    }

    function handleMessage(message) {
        switch (message.type) {
            case 'rpcCall':
                rpcClient.dispatch(message);
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

                emulator.snapshotDone(message.success, message.snapshot);
                break;

            default:
                console.error('unknown message from main thread', message);
        }
    }

    async function initialize({
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
        module,
    }) {
        emulator = await Emulator.create(
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
            module,
            {
                onFrame: postFrame,
                onSpeedDisplay: postSpeed,
                postSnapshot,
                log: postLog,
            }
        );

        return {
            deviceType: emulator.getDeviceType(),
            cardInserted: emulator.isSdInserted(),
        };
    }

    function getSession() {
        assertEmulator('getSession');

        return emulator.getSession();
    }

    async function stop() {
        assertEmulator('stop');
        emulator.stop();

        await emulator.snapshotPromise;
    }

    function ejectCard() {
        assertEmulator('ejectSd');
        emulator.ejectCard();
    }

    async function insertCard({ data, cardId }) {
        assertEmulator('insertCard');
        await emulator.insertCard(data, cardId);
    }

    function reset() {
        assertEmulator('reset');
        emulator.reset();
    }

    function install(files) {
        assertEmulator('install');
        return emulator.install(files);
    }

    async function main() {
        rpcClient = new RpcClient(self)
            .register('initialize', initialize)
            .register('getSession', getSession)
            .register('stop', stop)
            .register('ejectCard', ejectCard)
            .register('insertCard', insertCard)
            .register('reset', reset)
            .register('install', install);

        onmessage = (e) => handleMessage(e.data);

        postReady();
    }

    main().catch((e) => console.error(e));
})();
