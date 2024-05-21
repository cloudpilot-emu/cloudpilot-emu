const DB_NAME = 'cp-uarm';
const DB_VERSION = 2;

function compressCard(image) {
    const compressed = new Uint8Array(image.length + ((image.length >>> 13) + 1) * 2 + 6);
    let pos = 0;

    compressed[pos++] = image.length;
    compressed[pos++] = image.length >>> 8;
    compressed[pos++] = image.length >>> 16;
    compressed[pos++] = image.length >>> 24;

    for (let i = 0; i << 13 < image.length; i++) {
        const block = image.subarray(i << 13, (i + 1) << 13);

        let value = block[0];
        for (let j = 1; j < block.length; j++) {
            if (block[j] !== value) {
                value = -1;
                break;
            }
        }

        let blockType = value === -1 ? 0 : 1;
        if (block.length !== 0x2000) blockType |= 0x80;

        compressed[pos++] = blockType;

        if (blockType & 0x80) {
            compressed[pos++] = block.length;
            compressed[pos++] = block.length >>> 8;
        }

        if (blockType & 0x01) compressed[pos++] = value;
        else {
            compressed.subarray(pos, pos + block.length).set(block);
            pos += block.length;
        }
    }

    return compressed.subarray(0, pos);
}

function decompressCard(compressed) {
    let pos = 0;
    const len = compressed[pos++] | (compressed[pos++] << 8) | (compressed[pos++] << 16) | (compressed[pos++] << 24);

    const image = new Uint8Array(len);

    for (let i = 0; pos < compressed.length; i++) {
        const blockType = compressed[pos++];

        let blockLen = 0x2000;
        if (blockType & 0x80) {
            blockLen = compressed[pos++] | (compressed[pos++] << 8);
        }

        const block = image.subarray(i << 13, (i + 1) << 13);

        if (blockType & 0x01) block.fill(compressed[pos++]);
        else {
            block.set(compressed.subarray(pos, pos + block.length));
            pos += block.length;
        }
    }

    if (pos !== compressed.length) throw new Error('compressed card image corrupt');

    return image;
}

function generateUuid() {
    const bytes = new Uint8Array(16);
    crypto.getRandomValues(bytes);

    const hex = Array.from(bytes)
        .map((i) => i.toString(16).padStart(2, '0'))
        .join('');

    return `${hex.substring(0, 8)}-${hex.substring(8, 12)}-${hex.substring(12, 16)}-${hex.substring(
        16,
        20
    )}-${hex.substring(20, 32)}`;
}

function complete(txOrRequest) {
    return new Promise((resolve, reject) => {
        if (!!txOrRequest.abort) {
            txOrRequest.oncomplete = () => resolve();
        } else {
            txOrRequest.onsuccess = () => resolve(txOrRequest.result);
        }

        txOrRequest.onerror = () => reject(txOrRequest.error);
    });
}

function initdDb() {
    return new Promise((resolve, reject) => {
        const request = indexedDB.open(DB_NAME, DB_VERSION);

        request.onerror = () => reject('failed to open database');
        request.onblocked = () => reject('necessary db update blocked by open app');
        request.onupgradeneeded = (e) => {
            const db = request.result;

            if (e.oldVersion < 1) {
                db.createObjectStore('kvs');
            }

            if (e.oldVersion < 2) {
                db.createObjectStore('lock');
            }
        };

        request.onsuccess = () => resolve(request.result);
    });
}

async function acquireLock(db) {
    const lockToken = generateUuid();
    const tx = db.transaction(['lock'], 'readwrite');

    await complete(tx.objectStore('lock').put(lockToken, 0));

    return lockToken;
}

export class Database {
    constructor(db, lockToken) {
        this.db = db;
        this.lockToken = lockToken;

        console.log(generateUuid());
    }

    static async create() {
        const db = await initdDb();
        const lockToken = await acquireLock(db);

        return new Database(db, lockToken);
    }

    async assertLock(tx) {
        const lockToken = await complete(tx.objectStore('lock').get(0));

        if (lockToken !== this.lockToken) {
            tx.abort();

            setTimeout(() => alert('CloudpilotEmu has been opened in another tab. No more data will be persisted.'), 0);

            throw new Error('page lock lost');
        }
    }

    async tx(stores, mode) {
        const tx = this.db.transaction([...stores, ...(mode === 'readwrite' ? ['lock'] : [])], mode);

        if (mode === 'readwrite') {
            await this.assertLock(tx);
        }

        return tx;
    }

    async kvsGet(key) {
        const tx = await this.tx(['kvs'], 'readonly');

        return await complete(tx.objectStore('kvs').get(key));
    }

    async kvsPut(key, value) {
        const tx = await this.tx(['kvs'], 'readwrite');

        return await complete(tx.objectStore('kvs').put(value, key));
    }

    getNor() {
        return this.kvsGet('romNor');
    }

    putNor(nor) {
        return this.kvsPut('romNor', nor);
    }

    getNand() {
        return this.kvsGet('romNand');
    }

    putNand(nand) {
        return this.kvsPut('romNand', nand);
    }

    async getSd() {
        const sd = await this.kvsGet('sdImage');
        if (!sd) return;

        return { name: sd.name, content: decompressCard(sd.content) };
    }

    putSd(sd) {
        this.kvsPut('sdImage', { name: sd.name, content: compressCard(sd.content) });
    }
}
