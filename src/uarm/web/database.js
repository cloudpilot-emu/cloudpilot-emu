const DB_NAME = 'cp-uarm';
const DB_VERSION = 1;

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

export class Database {
    constructor(db) {
        this.db = db;
    }

    static create() {
        return new Promise((resolve, reject) => {
            const request = indexedDB.open(DB_NAME, DB_VERSION);

            request.onerror = () => reject('failed to open database');
            request.onblocked = () => reject('necessary db update blocked by open app');
            request.onupgradeneeded = (e) => {
                const db = request.result;

                db.createObjectStore('kvs');
            };
            request.onsuccess = () => resolve(new Database(request.result));
        });
    }

    kvsGet(key) {
        return new Promise((resolve, reject) => {
            const tx = this.db.transaction(['kvs'], 'readonly');
            tx.onerror = () => reject(tx.error);

            const request = tx.objectStore('kvs').get(key);
            request.onsuccess = () => resolve(request.result);
        });
    }

    kvsPut(key, value) {
        return new Promise((resolve, reject) => {
            const tx = this.db.transaction(['kvs'], 'readwrite');
            tx.onerror = () => reject(tx.error);

            const request = tx.objectStore('kvs').put(value, key);
            request.onsuccess = () => resolve();
        });
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
