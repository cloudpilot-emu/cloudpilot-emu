import './setimmediate/setimmediate.js';
import { Emulator } from './emulator.js';
import { Database } from './database.js';
import { AudioDriver } from './audiodriver.js';

(function () {
    const isIOSSafari = !navigator.userAgent.match(/(crios)|(fxios)/i);
    const isIOS =
        !!navigator.platform.match(/iPhone|iPad|iPod/) ||
        (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);
    const isSafari = 'safari' in window || (isIOS && isIOSSafari);

    const labelNor = document.getElementById('nor-image');
    const labelNand = document.getElementById('nand-image');
    const labelSD = document.getElementById('sd-image');
    const speedDisplay = document.getElementById('speed');
    const logContainer = document.getElementById('log');

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
            if (isSafari && fileInput) document.body.removeChild(fileInput);
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

            if (isSafari) document.body.appendChild(fileInput);

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

        emulator = await Emulator.create(fileNor.content, fileNand.content, fileSd?.content, {
            canvas: canvasCtx.canvas,
            speedDisplay,
            log,
        });
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

        if (window.location.search.indexOf('?noload') < 0) {
            log('Reload with ?noload appended to the URL if the emulator hangs on load due to invalid NOR or NAND');
            log('---');

            await restart();
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

        audioButton.addEventListener('click', () => onAudioButtonClick());

        clearLog.addEventListener('click', () => (logContainer.innerHTML = ''));

        document.addEventListener('visibilitychange', () => (document.hidden ? emulator?.stop() : emulator?.start()));
    }

    main().catch((e) => console.error(e));
})();
