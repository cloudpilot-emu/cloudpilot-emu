import './setimmediate/setimmediate.js';
import { Emulator } from './emulator.js';
import { Database } from './database.js';
import { AudioDriver } from './audiodriver.js';

(function () {
    const isMacOSSafari = 'safari' in window;
    const isIOS =
        !!navigator.platform.match(/iPhone|iPad|iPod/) ||
        (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
    const isWebkit =
        isMacOSSafari || isIOS || (!!navigator.userAgent.match(/Safari/) && !navigator.userAgent.match(/Chrome/));

    let binary = isWebkit ? 'uarm_web_webkit.wasm' : 'uarm_web_other.wasm';

    const labelNor = document.getElementById('nor-image');
    const labelNand = document.getElementById('nand-image');
    const labelSD = document.getElementById('sd-image');
    const speedDisplay = document.getElementById('speed');
    const logContainer = document.getElementById('log');
    const maxLoadLabel = document.getElementById('max-load');
    const mipsLimitLabel = document.getElementById('mips-limit');
    const maxLoadSlider = document.getElementById('max-load-slider');
    const mipsLimitSlider = document.getElementById('mips-limit-slider');

    const uploadNor = document.getElementById('upload-nor');
    const uploadNand = document.getElementById('upload-nand');
    const uploadSD = document.getElementById('upload-sd');
    const clearLog = document.getElementById('clear-log');

    const audioButton = document.getElementById('audio-button');

    const canvas = document.getElementsByTagName('canvas')[0];
    const canvasCtx = canvas.getContext('2d');

    let fileNor, fileNand, fileSd;
    let emulator;
    let audioDriver;
    let maxLoad = 100;
    let mipsLimit = 100;

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
                    await audioDriver.initialize(emulator);
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

    function clearCanvas() {
        canvasCtx.fillStyle = '#fff';
        canvasCtx.fillRect(0, 0, canvas.width, canvas.height);
    }

    const uploadHandler = (assign) => () =>
        openFile()
            .then((file) => {
                assign(file);
                updateLabels();

                restart();
            })
            .catch((e) => console.error(e));

    async function restart() {
        if (!(fileNor && fileNand)) return;

        emulator?.destroy();
        clearCanvas();

        log(`loading ${binary}`);

        emulator = await Emulator.create(
            fileNor.content,
            fileNand.content,
            fileSd?.content,
            maxLoad,
            mipsLimit * 1000000,
            {
                canvas: canvasCtx.canvas,
                speedDisplay,
                log,
                binary,
            }
        );
        emulator?.start();

        if (emulator && audioDriver) audioDriver.setEmulator(emulator);

        if (emulator) audioButton.disabled = false;
    }

    async function main() {
        const database = await Database.create();
        clearCanvas();

        fileNor = await database.getNor();
        fileNand = await database.getNand();
        fileSd = await database.getSd();

        const query = new URLSearchParams(location.search);

        if (query.has('binary')) binary = query.get('binary');

        if (!query.has('noload')) {
            log('Reload with ?noload appended to the URL if the emulator hangs on load due to invalid NOR or NAND');
            log('---');

            await restart(binary);
        }

        updateLabels();

        uploadNor.addEventListener(
            'click',
            uploadHandler((file) => {
                database.putNor(file);
                fileNor = file;
            })
        );
        uploadNand.addEventListener(
            'click',
            uploadHandler((file) => {
                database.putNand(file);
                fileNand = file;
            })
        );
        uploadSD.addEventListener(
            'click',
            uploadHandler((file) => {
                database.putSd(file);
                fileSd = file;
            })
        );

        maxLoadSlider.value = maxLoad;
        mipsLimitSlider.value = mipsLimit;

        maxLoadSlider.addEventListener('input', updateMaxLoad);
        mipsLimitSlider.addEventListener('input', updateMipsLimit);

        updateMaxLoad();
        updateMipsLimit();

        audioButton.addEventListener('click', () => onAudioButtonClick());

        clearLog.addEventListener('click', () => (logContainer.innerHTML = ''));

        document.addEventListener('visibilitychange', () => (document.hidden ? emulator?.stop() : emulator?.start()));
    }

    main().catch((e) => console.error(e));
})();
