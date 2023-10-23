import 'https://unpkg.com/setimmediate@1.0.5';

const labelNor = document.getElementById('nor-image');
const labelNand = document.getElementById('nand-image');
const labelSD = document.getElementById('sd-image');

const uploadNor = document.getElementById('upload-nor');
const uploadNand = document.getElementById('upload-nand');
const uploadSD = document.getElementById('upload-sd');

const canvas = document.getElementById('canvas-sdl');

let fileNor, fileNand, fileSd;

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

            reader.onload = () => resolve({ name: file.name, content: new Uint8Array(reader.result) });
            reader.onerror = () => reject(reader.error);

            reader.readAsArrayBuffer(file);
        });

        input.click();
    });

async function restart() {
    if (!(fileNor && fileNand)) return;

    let module;
    try {
        module = await createModule({
            noInitialRun: true,
            canvas,
        });
    } catch (e) {
        console.error('failed to load and compile WASM module', e);
    }

    module.FS.writeFile('/nor.bin', fileNor.content);
    module.FS.writeFile('/nand.bin', fileNand.content);

    if (module.callMain(['-r', '/nor.bin', '-n', '/nand.bin']) !== 0) {
        console.error('uARM terminated with error');
        return;
    }

    const cycle = module.cwrap('cycle', 'number', []);
    const schedule = () => {
        const delay = cycle() / 1000;

        if (delay < 10) setImmediate(schedule);
        else setTimeout(schedule, delay);
    };
    schedule();
}

async function main() {
    updateLabels();

    const ctx = canvas.getContext('2d');
    ctx.fillStyle = '#ddd';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    uploadNor.addEventListener('click', () =>
        openFile()
            .then((file) => {
                fileNor = file;
                updateLabels();

                restart();
            })
            .catch((e) => console.error(e))
    );

    uploadNand.addEventListener('click', () =>
        openFile()
            .then((file) => {
                fileNand = file;
                updateLabels();

                restart();
            })
            .catch((e) => console.error(e))
    );
}

main().catch((e) => console.error(e));
