import './setimmediate/setimmediate.js';
import { Emulator } from './emulator.js';
import { Database } from './database.js';

const labelNor = document.getElementById('nor-image');
const labelNand = document.getElementById('nand-image');
const labelSD = document.getElementById('sd-image');
const speedDisplay = document.getElementById('speed');
const logContainer = document.getElementById('log');

const uploadNor = document.getElementById('upload-nor');
const uploadNand = document.getElementById('upload-nand');
const uploadSD = document.getElementById('upload-sd');
const clearLog = document.getElementById('clear-log');

const canvas = document.getElementsByTagName('canvas')[0];
const canvasCtx = canvas.getContext('2d');

let fileNor, fileNand, fileSd;
let emulator;

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

const openFile = () =>
    new Promise((resolve, reject) => {
        const input = document.createElement('input');
        input.type = 'file';

        input.addEventListener('change', (evt) => {
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
        });

        input.click();
    });

function drawSkin() {
    const image = new Image();
    image.src = 'web/skin.svg';

    image.onload = () => canvasCtx.drawImage(image, 0, 640, 640, 240);
}

function clearCanvas() {
    canvasCtx.fillStyle = '#fff';
    canvasCtx.fillRect(0, 0, canvas.width, canvas.height);

    drawSkin();
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

    emulator?.stop();
    clearCanvas();

    emulator = await Emulator.create(fileNor.content, fileNand.content, fileSd?.content, {
        canvasCtx,
        speedDisplay,
        log,
    });
    emulator?.start();
}

async function main() {
    const database = await Database.create();

    fileNor = await database.getNor();
    fileNand = await database.getNand();
    fileSd = await database.getSd();
    await restart();

    updateLabels();
    clearCanvas();

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

    clearLog.addEventListener('click', () => (logContainer.innerHTML = ''));

    document.addEventListener('visibilitychange', () => (document.hidden ? emulator?.stop() : emulator?.start()));
}

main().catch((e) => console.error(e));
