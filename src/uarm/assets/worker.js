importScripts('../uarm_web.js', './setimmediate/setimmediate.js', './crc.js');

(function () {
    function copyIn(module, data) {
        if (!data) return undefined;

        const bridge = new module.Bridge();

        try {
            const ptr = bridge.Malloc(data.length);
            if (ptr === 0) return undefined;

            const ptrRaw = module.getPointer(ptr);

            module.HEAPU8.subarray(ptrRaw, ptrRaw + data.length).set(data);

            return ptr;
        } finally {
            module.destroy(bridge);
        }
    }

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

            let onRpcSuccess, onRpcError;
            const rpcComplete = new Promise((resolve, reject) => {
                onRpcSuccess = resolve;
                onRpcError = reject;
            });

            try {
                const result = await this.registeredMethods.get(method)(args, rpcComplete);
                this.worker.postMessage({ type: 'rpcSuccess', id, result });

                onRpcSuccess();
            } catch (e) {
                console.error(e);
                this.worker.postMessage({ type: 'rpcError', id, error: `${e}` });

                onRpcError(e);
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

    const BACKUP_STATE_CREATED = 0;
    const BACKUP_STATE_IN_PROGRESS = 1;
    const BACKUP_STATE_DONE = 2;
    const BACKUP_STATE_ERROR = -1;

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
            uarm,
            module,
            maxLoad,
            cyclesPerSecondLimit,
            crcCheck,
            cardId,
            { onSpeedDisplay, onFrame, log, postSnapshot }
        ) {
            this.uarm = uarm;
            this.module = module;
            this.bridge = new module.Bridge();
            this.onSpeedDisplay = onSpeedDisplay;
            this.onFrame = onFrame;
            this.log = log;
            this.postSnapshot = postSnapshot;
            this.cardId = cardId;
            this.crcCheck = crcCheck;

            this.amIDead = false;
            this.pcmEnabled = false;
            this.pcmPort = undefined;
            this.snapshotPending = false;
            this.snapshotPromise = Promise.resolve();
            this.resolveSnapshot = () => undefined;
            this.deviceType = uarm.GetDeviceType();
            this.savestate = undefined;

            this.uarm.SetMaxLoad(maxLoad);
            this.uarm.SetCyclesPerSecondLimit(cyclesPerSecondLimit);

            const nandSize = uarm.GetNandDataSize();
            this.nandTracker = new DirtyPageTracker({
                pageSize: 4224,
                pageCount: (nandSize / 4224) | 0,
                name: 'NAND snapshot',
                crcCheck,
                getDataPtr: () => module.getPointer(uarm.GetNandData()),
                getDirtyPagesPtr: () => module.getPointer(uarm.GetNandDirtyPages()),
                isDirty: () => uarm.IsNandDirty(),
                setDirty: (isDirty) => uarm.SetNandDirty(isDirty),
                module,
            });

            this.setupSdCardTracker();

            const ramSize = uarm.GetMemoryDataSize();
            this.ramTracker = new DirtyPageTracker({
                pageSize: 1024,
                pageCount: (ramSize / 1024) | 0,
                name: 'RAM snapshot',
                crcCheck,
                getDataPtr: () => module.getPointer(uarm.GetMemoryData()),
                getDirtyPagesPtr: () => module.getPointer(uarm.GetMemoryDirtyPages()),
                isDirty: () => true,
                setDirty: () => undefined,
                module,
            });
        }

        free(ptr) {
            if (!ptr) return;

            this.bridge.Free(ptr);
        }

        copyIn(data) {
            return copyIn(this.module, data);
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

            const norPtr = copyIn(module, nor);
            const nandPtr = copyIn(module, nand);
            const sdPtr = copyIn(module, sd);
            const savestatePtr = copyIn(module, savestate);
            const ramPtr = copyIn(module, ram);

            module.callMain([]);

            const uarm = new module.Uarm();

            uarm.SetRamSize(ramSize).SetNand(nand.length, nandPtr);

            if (ram) uarm.SetMemory(ram.length, ramPtr);
            if (savestate) uarm.SetSavestate(savestate.length, savestatePtr);
            if (sd) uarm.SetSd(sd.length, sdPtr, cardId ?? '');

            uarm.Launch(nor.length, norPtr);

            return new Emulator(uarm, module, maxLoad, cyclesPerSecondLimit, crcCheck, cardId, env);
        }

        setupSdCardTracker() {
            const sdCardSize = this.uarm.GetSdCardDataSize();
            this.sdCardTracker = new DirtyPageTracker({
                pageSize: 8192,
                pageCount: (sdCardSize / 8192) | 0,
                name: 'SD card snapshot',
                crcCheck: this.crcCheck,
                getDataPtr: () => this.module.getPointer(this.uarm.GetSdCardData()),
                getDirtyPagesPtr: () => this.module.getPointer(this.uarm.GetSdCardDirtyPages()),
                isDirty: () => this.uarm.IsSdCardDirty(),
                setDirty: (isDirty) => this.uarm.SetSdCardDirty(isDirty),
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
            this.lastSnapshot = Number(this.uarm.GetTimestampUsec());

            const schedule = () => {
                const now64 = this.uarm.GetTimestampUsec();
                const now = Number(now64);

                try {
                    this.uarm.Cycle(now64);
                } catch (e) {
                    this.amIDead = true;
                    console.error(e);
                    this.log(e.message);

                    return;
                }

                this.render();
                this.processAudio();

                const timesliceRemainning =
                    (this.uarm.GetTimesliceSizeUsec() - Number(this.uarm.GetTimestampUsec()) + now) / 1000;
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

            this.uarm.Save();
            const savestatePtr = this.module.getPointer(this.uarm.GetSavestateData());
            const savestateSize = this.uarm.GetSavestateSize();

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
                    ramSize: this.uarm.GetRamSize(),
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

            const framePtr = this.module.getPointer(this.uarm.GetFrame()) >>> 2;
            if (!framePtr) return;

            const frame = this.module.HEAPU32.subarray(framePtr, framePtr + 320 * (this.deviceType === 1 ? 480 : 320));

            if (framePool.length === 0) {
                this.onFrame(frame.slice().buffer);
            } else {
                const frameCopy = new Uint32Array(framePool.pop());
                frameCopy.set(frame);

                this.onFrame(frameCopy.buffer);
            }

            this.uarm.ResetFrame();
        }

        getPcmBuffer() {
            if (pcmPool.length > 0) return new Uint32Array(pcmPool.pop());

            return new Uint32Array(PCM_BUFFER_SIZE);
        }

        processAudio() {
            if (!this.module || !this.pcmPort || !this.pcmEnabled) return;

            const pendingSamples = this.uarm.PendingSamples();
            if (pendingSamples === 0) return;

            const samplesPtr = this.module.getPointer(this.uarm.PopQueuedSamples()) >>> 2;
            const samples = this.getPcmBuffer();

            samples.set(
                this.module.HEAPU32.subarray(samplesPtr, samplesPtr + Math.min(pendingSamples, samples.length))
            );

            this.pcmPort.postMessage({ type: 'sample-data', count: pendingSamples, buffer: samples.buffer }, [
                samples.buffer,
            ]);
        }

        updateSpeedDisplay() {
            const currentIps = this.uarm.CurrentIps();
            const currentIpsMax = Number(this.uarm.CurrentIpsMax());

            this.onSpeedDisplay(
                `current ${(currentIps / 1e6).toFixed(2)} MIPS, limit ${(currentIpsMax / 1e6).toFixed(2)} MIPS -> ${(
                    (currentIps / currentIpsMax) *
                    100
                ).toFixed(2)}%`
            );
        }

        setupPcm(port) {
            this.pcmPort = port;
            this.uarm.SetPcmOutputEnabled(true);
            this.pcmEnabled = true;

            this.pcmPort.onmessage = (evt) => this.handlePcmMessage(evt.data);
        }

        handlePcmMessage(message) {
            switch (message.type) {
                case 'suspend-pcm':
                    this.uarm.SetPcmSuspended(true);
                    break;

                case 'resume-pcm':
                    this.uarm.SetPcmSuspended(false);
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
            this.uarm.Save();

            const romPtr = this.module.getPointer(this.uarm.GetRomData());
            const nandPtr = this.module.getPointer(this.uarm.GetNandData());
            const ramPtr = this.module.getPointer(this.uarm.GetMemoryData());
            const savestatePtr = this.module.getPointer(this.uarm.GetSavestateData());

            const deviceId = this.uarm.GetDeviceType();
            const nor = this.module.HEAPU8.subarray(romPtr, romPtr + this.uarm.GetRomDataSize());
            const nand = this.module.HEAPU8.subarray(nandPtr, nandPtr + this.uarm.GetNandDataSize());
            const ram = this.module.HEAPU8.subarray(ramPtr, ramPtr + this.uarm.GetMemoryDataSize());
            const savestate = this.module.HEAPU8.subarray(savestatePtr, savestatePtr + this.uarm.GetSavestateSize());
            const ramSize = this.uarm.GetRamSize();

            return { deviceId, nor, nand, ram, savestate, ramSize };
        }

        ejectCard() {
            this.uarm.SdCardEject();
        }

        async insertCard(data, cardId) {
            await this.snapshotPromise;

            if (this.uarm.IsSdInserted()) throw new Error('SD card already inserted');

            const ptr = this.copyIn(data);

            if (!this.uarm.SdCardInsert(ptr, data.length, cardId)) {
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
                    const filePtr = this.copyIn(file.content);
                    if (!filePtr) throw new Error('unable to allocate memory');

                    const result = this.uarm.InstallDatabase(file.content.length, filePtr);
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

        async backup(type, transactionComplete) {
            if (!!this.timeoutHandle) {
                this.stop();
                transactionComplete.finally(() => this.start());
            }

            await this.snapshotPromise;
            this.triggerSnapshot();
            await this.snapshotPromise;

            const backup = this.uarm.NewDbBackup(type);
            transactionComplete.finally(() => this.module.destroy(backup));

            if (!backup.Init()) throw new Error('backup failed to initialise');

            while (backup.GetState() === BACKUP_STATE_IN_PROGRESS) {
                if (backup.Continue()) {
                    if (backup.HasLastProcessedDb()) this.log(`OK ${backup.GetLastProcessedDb()}`);
                } else {
                    this.log(`FAILED ${backup.GetLastProcessedDb()}`);
                }
            }

            const backupPtr = backup.GetArchiveData();
            const backupSize = backup.GetArchiveSize();

            if (!backupPtr) {
                this.log('backup failed');
                return;
            }

            const backupPtrRaw = this.module.getPointer(backupPtr);

            return this.module.HEAPU8.subarray(backupPtrRaw, backupPtrRaw + backupSize);
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
                emulator.uarm.SetMaxLoad(message.maxLoad);
                break;

            case 'setCyclesPerSecondLimit':
                assertEmulator('cyclesPerSecondLimit');
                emulator.uarm.SetCyclesPerSecondLimit(message.cyclesPerSecondLimit);
                break;

            case 'penDown':
                assertEmulator('penDown');
                emulator.uarm.PenDown(message.x, message.y);
                break;

            case 'penUp':
                assertEmulator('penUp');
                emulator.uarm.PenUp();
                break;

            case 'buttonDown': {
                const button = mapButton(message.button);
                if (button < 0) {
                    console.error(`ignoring unknown button ${message.button}`);
                    return;
                }

                assertEmulator('buttonDown');

                emulator.uarm.KeyDown(button);
                break;
            }

            case 'buttonUp': {
                const button = mapButton(message.button);
                if (button < 0) {
                    console.error(`ignoring unknown button ${message.button}`);
                    return;
                }

                assertEmulator('buttonUp');

                emulator.uarm.KeyUp(button);
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

                emulator.uarm.SetPcmOutputEnabled(false);
                break;

            case 'enablePcm':
                assertEmulator('enablePcm');

                emulator.uarm.SetPcmOutputEnabled(true);
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
            deviceType: emulator.uarm.GetDeviceType(),
            cardInserted: emulator.uarm.IsSdInserted(),
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
        emulator.uarm.Reset();
    }

    function install(files) {
        assertEmulator('install');
        return emulator.install(files);
    }

    function backup({ type }, rpcComplete) {
        assertEmulator('backup');
        return emulator.backup(type, rpcComplete);
    }

    async function main() {
        rpcClient = new RpcClient(self)
            .register('initialize', initialize)
            .register('getSession', getSession)
            .register('stop', stop)
            .register('ejectCard', ejectCard)
            .register('insertCard', insertCard)
            .register('reset', reset)
            .register('install', install)
            .register('backup', backup);

        onmessage = (e) => handleMessage(e.data);

        postReady();
    }

    main().catch((e) => console.error(e));
})();
