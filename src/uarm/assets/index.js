import './setimmediate/setimmediate.js';
import { Emulator } from './emulator.js';
import { Database } from './database.js';
import { AudioDriver } from './audiodriver.js';
import { compressSession, decompressSession } from './session.js';

(function () {
    const isMacOSSafari = 'safari' in window;
    const isIOS =
        !!navigator.platform.match(/iPhone|iPad|iPod/) ||
        (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
    const isWebkit =
        isMacOSSafari || isIOS || (!!navigator.userAgent.match(/Safari/) && !navigator.userAgent.match(/Chrome/));

    let binary = isWebkit ? 'uarm_web_webkit.wasm' : 'uarm_web_other.wasm';
    let disableAutoPause = false;

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
    const downloadNandButton = document.getElementById('download-nand');
    const uploadSDButton = document.getElementById('upload-sd');
    const downloadSDButton = document.getElementById('download-sd');
    const exportImageButton = document.getElementById('export-image');
    const importImageButton = document.getElementById('import-image');
    const clearLogButton = document.getElementById('clear-log');

    const audioButton = document.getElementById('audio-button');

    const canvas = document.getElementsByTagName('canvas')[0];
    const canvasCtx = canvas.getContext('2d');

    let fileNor, fileNand, fileSd;
    let emulator;
    let audioDriver;
    let database;
    let maxLoad = 100;
    let mipsLimit = 100;
    let crcCheck = false;
    let sampleRate;

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

    function updateMaxLoad() {
        maxLoad = parseFloat(maxLoadSlider.value);
        maxLoadLabel.innerText = `${Math.floor(maxLoad)}%`;

        if (emulator) emulator.setMaxLoad(maxLoad);
    }

    function updateMipsLimit() {
        mipsLimit = parseFloat(mipsLimitSlider.value);
        mipsLimitLabel.innerText = `${Math.floor(mipsLimit)} MIPS`;

        if (emulator) emulator.setCyclesPerSecondLimit(mipsLimit * 1000000);
    }

    function log(message) {
        const line = document.createElement('div');
        line.className = 'log-line';
        line.innerText = message;

        logContainer.appendChild(line);
        logContainer.scrollTop = logContainer.scrollHeight;
    }

    function updateLabels() {
        labelNor.innerText = fileNor?.name ?? '[none]';
        labelNand.innerText = fileNand?.name ?? '[none]';
        labelSD.innerText = fileSd?.name ?? '[none]';
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
    const openFile = () =>
        new Promise((resolve, reject) => {
            if (isWebkit && fileInput) document.body.removeChild(fileInput);
            fileInput = document.createElement('input');

            fileInput.onchange = (evt) => {
                console.log('onchange');

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
            };

            fileInput.type = 'file';
            fileInput.value = '';
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
        try {
            const file = await openFile();

            disableAutoPause = true;
            await emulator?.stop();

            await assign(file);

            updateLabels();
            restart();
        } catch (e) {
            console.error(e);
        }
    };

    async function restart() {
        if (!(fileNor && fileNand)) return;

        emulator?.destroy();
        clearCanvas();

        const ram = await database.getRam(crcCheck);

        log(`loading ${binary}`);

        emulator = await Emulator.create(
            fileNor.content,
            fileNand.content,
            fileSd?.content,
            ram,
            maxLoad,
            mipsLimit * 1000000,
            {
                canvas: canvasCtx.canvas,
                database,
                speedDisplay,
                log,
                binary,
                crcCheck,
                setSnapshotStatus,
            }
        );

        disableAutoPause = false;
        if (!document.hidden) emulator?.start();

        if (emulator && audioDriver) audioDriver.setEmulator(emulator);

        if (emulator) audioButton.disabled = false;
    }

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

    async function exportImage() {
        disableAutoPause = true;
        await emulator?.stop();

        const nor = await database.getNor();
        const ram = await database.getRam();
        const nand = await database.getNand();

        disableAutoPause = false;
        if (!document.hidden) emulator?.start();

        const session = compressSession({
            nor: nor.content,
            ram,
            nand: nand.content,
            metadata: {
                norName: nor.name,
                nandName: nand.name,
            },
        });

        saveFile(`${filenameFragment('uarm-session')}.bin`, session);
    }

    async function importImage() {
        const sessionFile = await openFile();

        try {
            const session = decompressSession(sessionFile.content);

            console.log(session);
        } catch (e) {
            alert(`Failed to load session: ${e.message}`);
        }
    }

    async function main() {
        setSnapshotStatus('ok');

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

        try {
            fileNor = await database.getNor();
            fileNand = await database.getNand(crcCheck);
            fileSd = await database.getSd(crcCheck);

            if (!query.has('noload')) {
                log('Reload with ?noload appended to the URL if the emulator hangs on load due to invalid NOR or NAND');
                log('---');

                await restart();
            }
        } catch (e) {
            console.error('failed to launch!');
        }

        updateLabels();

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

        uploadSDButton.addEventListener(
            'click',
            uploadHandler(async (file) => {
                await database.putSd(file);
                fileSd = file;
            })
        );

        importImageButton.addEventListener(
            'click',
            uploadHandler(async (file) => {
                try {
                    const session = decompressSession(file.content);

                    fileNor = { content: session.nor, name: session.metadata?.norName ?? 'saved ROM' };
                    fileNand = { content: session.nand, name: session.metadata?.nandName ?? 'saved NAND' };

                    await database.putNor(fileNor);
                    await database.putNand(fileNand);
                    await database.putRam(session.ram);
                } catch (e) {
                    alert(`Failed to load session: ${e.message}`);
                    console.error(e);
                }
            })
        );

        downloadNandButton.addEventListener('click', () => {
            database.getNand(false).then((nand) => saveFile(`${filenameFragment('nand')}.bin`, nand.content));
        });

        downloadSDButton.addEventListener('click', () => {
            database.getSd(false).then((sd) => saveFile(`${filenameFragment('sd')}.bin`, sd.content));
        });

        maxLoadSlider.value = maxLoad;
        mipsLimitSlider.value = mipsLimit;

        maxLoadSlider.addEventListener('input', updateMaxLoad);
        mipsLimitSlider.addEventListener('input', updateMipsLimit);

        updateMaxLoad();
        updateMipsLimit();

        audioButton.addEventListener('click', () => onAudioButtonClick());

        clearLogButton.addEventListener('click', () => (logContainer.innerHTML = ''));

        rotateButton.addEventListener('click', () => emulator?.rotate());

        document.addEventListener(
            'visibilitychange',
            () => !disableAutoPause && (document.hidden ? emulator?.stop() : emulator?.start())
        );
    }

    main().catch((e) => console.error(e));
})();
