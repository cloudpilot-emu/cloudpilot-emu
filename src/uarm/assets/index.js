import './setimmediate/setimmediate.js';
import { Mutex } from './async-mutex/async-mutex.js';
import { Emulator, loadModule, module } from './emulator.js';
import { Database, KVS_MAX_LOAD, KVS_MIPS } from './database.js';
import { AudioDriver } from './audiodriver.js';
import { SessionFile } from './sessionfile.js';

(function () {
    const isMacOSSafari = 'safari' in window;
    const isIOS =
        !!navigator.platform.match(/iPhone|iPad|iPod/) ||
        (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
    const isWebkit =
        isMacOSSafari || isIOS || (!!navigator.userAgent.match(/Safari/) && !navigator.userAgent.match(/Chrome/));

    let binary = isWebkit ? 'uarm_web_webkit.wasm' : 'uarm_web_other.wasm';
    let sessionFile;

    const labelNor = document.getElementById('nor-image');
    const labelNand = document.getElementById('nand-image');
    const labelSD = document.getElementById('sd-image');
    const speedDisplay = document.getElementById('speed');
    const logContainer = document.getElementById('log');
    const maxLoadLabel = document.getElementById('max-load');
    const mipsLimitLabel = document.getElementById('mips-limit');
    const maxLoadSlider = document.getElementById('max-load-slider');
    const mipsLimitSlider = document.getElementById('mips-limit-slider');
    const snapshotStatus = document.getElementById('snapshot-status');

    const rotateButton = document.getElementById('rotate');
    const uploadNorButton = document.getElementById('upload-nor');
    const uploadNandButton = document.getElementById('upload-nand');
    const clearNandButton = document.getElementById('clear-nand');
    const downloadNandButton = document.getElementById('download-nand');
    const uploadSDButton = document.getElementById('upload-sd');
    const downloadSDButton = document.getElementById('download-sd');
    const insertEjectSDButton = document.getElementById('insert-eject-sd');
    const exportImageButton = document.getElementById('export-image');
    const importImageButton = document.getElementById('import-image');
    const clearLogButton = document.getElementById('clear-log');
    const resetButton = document.getElementById('reset');
    const powerCycleButton = document.getElementById('power-cycle');
    const installButton = document.getElementById('install');
    const loader = document.getElementById('loading-mask');

    const audioButton = document.getElementById('audio-button');

    const canvas = document.getElementsByTagName('canvas')[0];
    const canvasCtx = canvas.getContext('2d');

    const mutex = new Mutex();

    let fileNor, fileNand, fileSd;
    let emulator;
    let audioDriver;
    let database;
    let maxLoad = 100;
    let mipsLimit = 100;
    let crcCheck = false;
    let sampleRate;

    function showLoader() {
        loader.style.display = 'flex';
    }

    function hideLoader() {
        loader.style.display = 'none';
    }

    function filenameFragment(prefix) {
        const now = new Date();

        const year = now.getFullYear();
        const month = (now.getMonth() + 1).toString().padStart(2, '0');
        const day = now.getDate().toString().padStart(2, '0');
        const hour = now.getHours().toString().padStart(2, '0');
        const minute = now.getMinutes().toString().padStart(2, '0');
        const second = now.getSeconds().toString().padStart(2, '0');

        return `${prefix}_${year}${month}${day}-${hour}${minute}${second}`;
    }

    function updateUi() {
        downloadNandButton.disabled = !fileNand;
        downloadSDButton.disabled = !fileSd;
        uploadSDButton.disabled = !!emulator?.cardInserted;
        insertEjectSDButton.innerText = emulator?.cardInserted ? 'Eject SD' : 'Insert SD';
        insertEjectSDButton.disabled = !(emulator && (fileSd || emulator.cardInserted));
        exportImageButton.disabled = !emulator;
        resetButton.disabled = !emulator;
        powerCycleButton.disabled = !emulator;
        installButton.disabled = !emulator;

        labelNor.innerText = fileNor?.name ?? '[none]';
        labelNand.innerText = fileNand?.name ?? '[none]';
        labelSD.innerText = fileSd?.name ?? '[none]';
    }

    const updateMaxLoad = () =>
        mutex.runExclusive(() => {
            maxLoad = parseFloat(maxLoadSlider.value);
            maxLoadLabel.innerText = `${Math.floor(maxLoad)}%`;

            database.kvsPut(KVS_MAX_LOAD, maxLoad);
            if (emulator) emulator.setMaxLoad(maxLoad);
        });

    const updateMipsLimit = () =>
        mutex.runExclusive(() => {
            mipsLimit = parseFloat(mipsLimitSlider.value);
            mipsLimitLabel.innerText = `${Math.floor(mipsLimit)} MIPS`;

            database.kvsPut(KVS_MIPS, mipsLimit);
            if (emulator) emulator.setCyclesPerSecondLimit(mipsLimit * 1000000);
        });

    function log(message) {
        const line = document.createElement('div');
        line.className = 'log-line';
        line.innerText = message;

        logContainer.appendChild(line);
        logContainer.scrollTop = logContainer.scrollHeight;
    }

    async function onAudioButtonClick() {
        if (!emulator) return;

        audioButton.disabled = true;

        try {
            if (!audioDriver) {
                audioDriver = new AudioDriver(
                    log,
                    () => (audioButton.innerText = audioDriver.isRunning() ? 'Stop audio' : 'Resume audio')
                );

                try {
                    await audioDriver.initialize(sampleRate);
                    audioDriver.setEmulator(emulator);
                } catch (e) {
                    console.error('failed to initialize audio driver', e);
                    audioButton.disabled = false;
                }
            } else {
                if (audioDriver.isRunning()) await audioDriver.pause();
                else await audioDriver.resume();
            }
        } finally {
            audioButton.disabled = false;
        }
    }

    let fileInput;
    const openFile = (multi = false) =>
        new Promise((resolve, reject) => {
            if (isWebkit && fileInput) document.body.removeChild(fileInput);
            fileInput = document.createElement('input');

            const readFile = (file) => {
                const reader = new FileReader();

                return new Promise((_resolve, _reject) => {
                    reader.onerror = () => _reject(reader.error);
                    reader.onload = () => _resolve({ name: file.name, content: new Uint8Array(reader.result) });

                    reader.readAsArrayBuffer(file);
                });
            };

            fileInput.onchange = (evt) => {
                const target = evt.target;

                if (multi) {
                    Promise.all(Array.from(target?.files ?? []).map(readFile))
                        .then(resolve)
                        .catch(reject);
                } else {
                    readFile(target?.files?.[0]).then(resolve).catch(reject);
                }
            };

            fileInput.type = 'file';
            fileInput.value = '';
            fileInput.multiple = multi;
            fileInput.style.display = 'none';

            if (isWebkit) document.body.appendChild(fileInput);

            fileInput.click();
        });

    function saveFile(name, content, type = 'application/octet-stream') {
        const file = new Blob([content], { type });
        const url = URL.createObjectURL(file);

        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.click();
    }

    function clearCanvas() {
        canvasCtx.fillStyle = '#fff';
        canvasCtx.fillRect(0, 0, canvas.width, canvas.height);
    }

    const uploadHandler = (assign) => async () => {
        const file = await openFile();

        await mutex.runExclusive(async () => {
            try {
                await emulator?.stop();
                await assign(file);
            } catch (e) {
                console.error(e);
            } finally {
                updateUi();
                restartUnguarded();
            }
        });
    };

    async function restartUnguarded() {
        if (!(fileNor && fileNand)) return;

        emulator?.destroy();
        clearCanvas();

        const ramSize = await database.getRamSize();
        const ram = await database.getRam(crcCheck, ramSize);

        if (fileNor.content.length === 0) fileNor = await database.getNor();
        if (fileNand.content.length === 0) fileNand = await database.getNand(crcCheck);
        if (fileSd?.content?.length === 0) fileSd = await database.getSd(crcCheck);
        const savestate = await database.getSavestate();

        emulator = await Emulator.create(
            ramSize,
            fileNor.content,
            fileNand.content,
            fileSd?.mounted ? fileSd?.content : undefined,
            fileSd?.mounted ? fileSd?.id : undefined,
            ram,
            savestate,
            maxLoad,
            mipsLimit * 1000000,
            {
                canvas: canvasCtx.canvas,
                database,
                speedDisplay,
                log,
                crcCheck,
                setSnapshotStatus,
            }
        );

        if (!document.hidden) emulator?.start();

        if (emulator && audioDriver) audioDriver.setEmulator(emulator);

        if (emulator) audioButton.disabled = false;

        updateUi();
    }

    const restart = () => mutex.runExclusive(restartUnguarded);

    function setSnapshotStatus(status) {
        switch (status) {
            case 'ok':
                snapshotStatus.innerText = 'no pending data';
                snapshotStatus.className = 'snapshot-ok';
                break;

            case 'saving':
                snapshotStatus.innerText = 'saving...';
                snapshotStatus.className = 'snapshot-saving';
                break;

            default:
                snapshotStatus.innerText = 'failed!';
                snapshotStatus.className = 'snapshot-failed';
                break;
        }
    }

    const exportImage = () =>
        mutex.runExclusive(async () => {
            if (!emulator) return;

            const { deviceId, nor, nand, ram, savestate, ramSize } = await emulator.getSession();
            const metadata = {
                norName: fileNor?.name ?? 'saved NOR',
                nandName: fileNand?.name ?? 'saved NAND',
            };

            const serializedSession = await sessionFile.serializeSession(
                ramSize,
                deviceId,
                metadata,
                nor,
                nand,
                ram,
                savestate
            );
            if (!serializedSession) return;

            saveFile(`${filenameFragment('uarm-session')}.bin`, serializedSession);
        });

    async function main() {
        setSnapshotStatus('ok');
        updateUi();

        database = await Database.create();
        clearCanvas();

        const query = new URLSearchParams(location.search);

        if (query.has('verify_crc')) crcCheck = true;
        if (crcCheck) {
            log('snapshot CRC checks enabled');
        } else {
            log('snapshot CRC checks disabled, reload with ?verify_crc to enable them');
        }

        sampleRate = isWebkit ? undefined : 44100;
        if (query.has('samplerate')) {
            sampleRate = parseInt(query.get('samplerate'));
            if (isNaN(sampleRate)) sampleRate = undefined;
        }

        if (query.has('binary')) binary = query.get('binary');
        log(`loading ${binary}`);
        loadModule(binary);

        sessionFile = new SessionFile(module);

        maxLoad = (await database.kvsGet(KVS_MAX_LOAD)) ?? maxLoad;
        mipsLimit = (await database.kvsGet(KVS_MIPS)) ?? mipsLimit;
        maxLoadSlider.value = maxLoad;
        mipsLimitSlider.value = mipsLimit;
        updateMaxLoad();
        updateMipsLimit();

        try {
            fileNor = await database.getNor();
            fileNand = await database.getNand(crcCheck);
            fileSd = await database.getSd(crcCheck);

            updateUi();

            if (!query.has('noload')) {
                log('Reload with ?noload appended to the URL if the emulator hangs on load due to invalid NOR or NAND');
                log('---');

                await restart();
            }
        } catch (e) {
            console.error('failed to launch!', e);
        }

        updateUi();

        exportImageButton.addEventListener('click', exportImage);

        uploadNorButton.addEventListener(
            'click',
            uploadHandler(async (file) => {
                await database.putNor(file);
                fileNor = file;
            })
        );

        uploadNandButton.addEventListener(
            'click',
            uploadHandler(async (file) => {
                await database.putNand(file);
                fileNand = file;
            })
        );

        clearNandButton.addEventListener('click', () =>
            mutex.runExclusive(async () => {
                if (fileNand && !confirm('This will clear the NAND content and wipe RAM. Do you want to continue?'))
                    return;

                await emulator?.stop();
                await database.clearNand();

                fileNand = await database.getNand(crcCheck);

                await restartUnguarded();
            })
        );

        importImageButton.addEventListener(
            'click',
            uploadHandler(async (file) => {
                try {
                    const { metadata, nor, nand, ram, savestate, ramSize } = await sessionFile.deserializeSession(
                        file.content
                    );

                    fileNor = { content: nor, name: metadata?.norName ?? 'saved ROM' };
                    fileNand = { content: nand, name: metadata?.nandName ?? 'saved NAND' };

                    await database.importSession(ramSize, fileNor, fileNand, ram, savestate);
                } catch (e) {
                    alert(`Failed to load session: ${e.message}`);
                    console.error(e);
                }
            })
        );

        uploadSDButton.addEventListener('click', async () => {
            if (emulator?.cardInserted) return;
            let file = await openFile();

            await mutex.runExclusive(async () => {
                try {
                    await emulator?.stop();

                    fileSd = await database.putSd(file, true);
                    await emulator?.insertCard(fileSd.content, fileSd.id);
                } finally {
                    emulator?.start();
                    updateUi();
                }
            });
        });

        downloadNandButton.addEventListener('click', () => {
            database.getNand(false).then((nand) => saveFile(`${filenameFragment('nand')}.bin`, nand.content));
        });

        downloadSDButton.addEventListener('click', () => {
            database.getSd(false).then((sd) => saveFile(`${filenameFragment('sd')}.bin`, sd.content));
        });

        insertEjectSDButton.addEventListener('click', () =>
            mutex.runExclusive(async () => {
                if (!emulator) return;

                if (emulator.cardInserted) {
                    await emulator.ejectCard();
                    await database.setCardMounted(false);
                } else {
                    fileSd = await database.getSd(crcCheck);

                    await database.setCardMounted(true);
                    await emulator.insertCard(fileSd.content, fileSd.id);
                }

                console.log('update');

                updateUi();
            })
        );

        powerCycleButton.addEventListener('click', () =>
            mutex.runExclusive(async () => {
                if (!emulator) return;

                await emulator.stop();
                await database.removeSavestate();

                await restartUnguarded();
            })
        );

        maxLoadSlider.addEventListener('input', updateMaxLoad);
        mipsLimitSlider.addEventListener('input', updateMipsLimit);

        audioButton.addEventListener('click', () => onAudioButtonClick());

        clearLogButton.addEventListener('click', () => (logContainer.innerHTML = ''));

        rotateButton.addEventListener('click', () => mutex.runExclusive(() => emulator?.rotate()));

        resetButton.addEventListener('click', () => mutex.runExclusive(() => emulator?.reset()));

        installButton.addEventListener('click', async () => {
            if (!emulator) return;

            const files = await openFile(true);
            showLoader();

            try {
                const failingFile = await emulator.install(files);
                if (failingFile === undefined) return;

                alert(
                    `There was a fatal error installing ${failingFile} . There is a known PalmOS bug that may cause installation of large files with several MB to cause crashes on NVFS based (all official E2) ROMs. In this case, please copy such files to SD instead, or use a ROM without NVFS.`
                );

                window.location.reload();
            } finally {
                hideLoader();
            }
        });

        document.addEventListener('visibilitychange', () =>
            mutex.runExclusive(() => (document.hidden ? emulator?.stop() : emulator?.start()))
        );
    }

    main().catch((e) => console.error(e));
})();
