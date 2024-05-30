// @ts-check

/**
 * @typedef {{name: string, content: Uint8Array}} Image
 */

const DB_NAME = 'cp-uarm';
const DB_VERSION = 3;

const NAND_SIZE = 528 * 2 * 1024 * 32;

const OBJECT_STORE_KVS = 'kvs';
const OBJECT_STORE_NAND = 'nand';
const OBJECT_STORE_LOCK = 'lock';

const KVS_ROM_NOR = 'romNor';
const KVS_ROM_NAND = 'romNand';
const KVS_SD_IMAGE = 'sdImage';
const KVS_NAND_NAME = 'nandName';
const KVS_NAND_CRC = 'nandCrc';

/**
 *
 * @param {Uint8Array} image
 * @returns {Uint8Array}
 */
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

/**
 *
 * @param {Uint8Array} compressed
 * @returns {Uint8Array}
 */
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

/**
 *
 * @param {Uint32Array} page
 * @returns {Uint32Array | number}
 */
export function compressNandPage(page) {
    const fst = page[0];

    for (let i = 1; i < 1056; i++) {
        if (page[i] !== fst) return page;
    }

    return fst;
}

/**
 *
 * @returns {string}
 */
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

/**
 * @template T
 * @param {IDBRequest<T> | IDBTransaction} txOrRequest
 * @returns {Promise<T>}
 */
function complete(txOrRequest) {
    return new Promise((resolve, reject) => {
        const request = /** @type IDBRequest */ (txOrRequest);
        const tx = /** @type IDBTransaction */ (txOrRequest);

        if (!!tx.abort) {
            tx.oncomplete = () => resolve(/** @type T */ (undefined));
        } else {
            request.onsuccess = () => resolve(request.result);
        }

        txOrRequest.onerror = () => reject(txOrRequest.error);
    });
}

/**
 *
 * @param {number} size
 * @param {IDBTransaction} tx
 * @returns {Promise<Uint8Array>}
 */
async function getNandData(size, tx) {
    if (size % 4224) throw new Error('invalid NAND size');

    const pagesTotal = (size / 4224) | 0;
    const data32 = new Uint32Array(size >>> 2);
    const data8 = new Uint8Array(data32.buffer, 0, size);
    data32.fill(0xffffffff);

    const store = tx.objectStore(OBJECT_STORE_NAND);

    const cursorRequest = store.openCursor();
    return new Promise((resolve, reject) => {
        cursorRequest.onsuccess = () => {
            const cursor = cursorRequest.result;
            if (!cursor) return resolve(data8);

            const pageIndex = /** @type number */ (cursor.key);
            if (pageIndex > pagesTotal) return reject(new Error(`page index ${pageIndex} out of bounds`));

            try {
                const page32 = data32.subarray(pageIndex * 1056, (pageIndex + 1) * 1056);
                if (typeof cursor.value === 'number') page32.fill(cursor.value);
                else page32.set(cursor.value);
            } catch (e) {
                reject(e);
            }

            cursor.continue();
        };
    });
}

/**
 * @param {Uint8Array} data8
 * @param {IDBTransaction} tx
 * @returns {Promise<void>}
 */
async function putNandData(data8, tx) {
    if (data8.length % 4224) throw new Error('invalid NAND size');

    const pagesTotal = (data8.length / 4224) | 0;
    const data32 = new Uint32Array(data8.buffer, 0, data8.length >>> 2);

    const store = tx.objectStore(OBJECT_STORE_NAND);

    await complete(store.clear());

    for (let iPage = 0; iPage < pagesTotal; iPage++) {
        const compressedPage = compressNandPage(data32.subarray(iPage * 1056, (iPage + 1) * 1056));
        if (compressedPage === 0xffffffff) continue;

        store.put(typeof compressedPage === 'number' ? compressedPage : compressedPage.slice(), iPage);
    }
}

/**
 *
 * @param {IDBOpenDBRequest} request
 * @returns {Promise<void>}
 */
async function migrateNand(request) {
    const tx = request.transaction;
    if (!tx) throw new Error('no version change transaction');

    const storeKvs = tx.objectStore(OBJECT_STORE_KVS);

    const nandImage = /** @type Image | undefined*/ (await complete(storeKvs.get(KVS_ROM_NAND)));
    if (!nandImage) return;

    storeKvs.delete(KVS_ROM_NAND);

    if (nandImage.content.length % 4224 === 0) {
        storeKvs.put(nandImage.name, KVS_NAND_NAME);
        putNandData(nandImage.content, tx);

        console.log('migrating NAND image');
    } else {
        console.warn('removing NAND image with invalid length');
    }
}

/**
 *
 * @returns {Promise<IDBDatabase>}
 */
function initdDb() {
    return new Promise((resolve, reject) => {
        const request = indexedDB.open(DB_NAME, DB_VERSION);

        request.onerror = () => reject('failed to open database');
        request.onblocked = () => reject('necessary db update blocked by open app');
        request.onupgradeneeded = (e) => {
            const db = request.result;

            if (e.oldVersion < 1) {
                db.createObjectStore(OBJECT_STORE_KVS);
            }

            if (e.oldVersion < 2) {
                db.createObjectStore(OBJECT_STORE_LOCK);
            }

            if (e.oldVersion < 3) {
                db.createObjectStore(OBJECT_STORE_NAND);
                migrateNand(request);
            }
        };

        request.onsuccess = () => resolve(request.result);
    });
}

/**
 *
 * @param {IDBDatabase} db
 * @returns {Promise<string>}
 */
async function acquireLock(db) {
    const lockToken = generateUuid();
    const tx = db.transaction([OBJECT_STORE_LOCK], 'readwrite');

    await complete(tx.objectStore(OBJECT_STORE_LOCK).put(lockToken, 0));

    return lockToken;
}

export class Database {
    /**
     *
     * @param {IDBDatabase} db
     * @param {string} lockToken
     */
    constructor(db, lockToken) {
        this.db = db;
        this.lockToken = lockToken;
        this.lockLost = false;
        this.nandPagePool = /** @type Array<Uint32Array> */ (new Array());
    }

    /**
     *
     * @returns {Promise<Database>}
     */
    static async create() {
        const db = await initdDb();
        const lockToken = await acquireLock(db);

        return new Database(db, lockToken);
    }

    /**
     *
     * @param {IDBTransaction} tx
     * @returns {Promise<void>}
     */
    async assertLock(tx) {
        const lockToken = /** @type string */ (await complete(tx.objectStore(OBJECT_STORE_LOCK).get(0)));

        if (lockToken !== this.lockToken) {
            tx.abort();

            if (!this.lockLost) {
                setTimeout(
                    () => alert('CloudpilotEmu has been opened in another tab. No more data will be persisted.'),
                    0
                );
            }

            this.lockLost = true;
            throw new Error('page lock lost');
        }
    }

    /**
     *
     * @param  {...string} stores
     * @returns {Promise<IDBTransaction>}
     */
    async tx(...stores) {
        const tx = this.db.transaction([...stores, OBJECT_STORE_LOCK], 'readwrite', { durability: 'relaxed' });
        await this.assertLock(tx);

        return tx;
    }

    /**
     *
     * @param {string} key
     * @returns {Promise<any>}
     */
    async kvsGet(key) {
        const tx = await this.tx(OBJECT_STORE_KVS);

        return complete(tx.objectStore(OBJECT_STORE_KVS).get(key));
    }

    /**
     *
     * @param {string} key
     * @param {any} value
     * @returns {Promise<void>}
     */
    async kvsPut(key, value) {
        const tx = await this.tx(OBJECT_STORE_KVS);

        await complete(tx.objectStore(OBJECT_STORE_KVS).put(value, key));
    }

    /**
     *
     * @param {string} key
     * @returns {Promise<void>}
     */
    async kvsDelete(key) {
        const tx = await this.tx(OBJECT_STORE_KVS);

        await complete(tx.objectStore(OBJECT_STORE_KVS).delete(key));
    }

    /**
     *
     * @returns {Promise<Image>}
     */
    getNor() {
        return this.kvsGet(KVS_ROM_NOR);
    }

    /**
     *
     * @param {Image} nor
     * @returns {Promise<void>}
     */
    putNor(nor) {
        return this.kvsPut(KVS_ROM_NOR, nor);
    }

    /**
     * @param {boolean} crcCheck
     * @returns {Promise<Image | undefined>}
     */
    async getNand(crcCheck) {
        const tx = await this.tx(OBJECT_STORE_NAND, OBJECT_STORE_KVS);
        const storeKvs = tx.objectStore(OBJECT_STORE_KVS);

        const name = /** @type string */ (await complete(storeKvs.get(KVS_NAND_NAME)));
        if (!name) return;

        const content = await getNandData(NAND_SIZE, tx);

        const crc = await complete(storeKvs.get(KVS_NAND_CRC));
        const crc32 = /** @type any */ (window).crc32;

        if (crcCheck && typeof crc === 'number') {
            if (crc !== crc32(content)) {
                alert(`CRC mismatch on NAND load!`);
            } else {
                console.log('NAND CRC OK');
            }
        }

        return { name, content };
    }

    /**
     *
     * @param {Image} nand
     */
    async putNand(nand) {
        const tx = await this.tx(OBJECT_STORE_NAND, OBJECT_STORE_KVS);

        tx.objectStore(OBJECT_STORE_KVS).put(nand.name, KVS_NAND_NAME);
        putNandData(nand.content, tx);

        await complete(tx);
    }

    /**
     * @returns {Promise <Image | undefined>}
     */
    async getSd() {
        const sd = await this.kvsGet(KVS_SD_IMAGE);
        if (!sd) return;

        return { name: sd.name, content: decompressCard(sd.content) };
    }

    /**
     *
     * @param {Image} sd
     * @returns {Promise<void>}
     */
    putSd(sd) {
        return this.kvsPut(KVS_SD_IMAGE, { name: sd.name, content: compressCard(sd.content) });
    }

    /**
     *
     * @param {number} nandScheduledPageCount
     * @param {Uint32Array} nandScheduledPages
     * @param {Uint32Array} nandPagePool
     * @returns {Promise<void>}
     */
    async storeSnapshot(nandScheduledPageCount, nandScheduledPages, nandPagePool, crc) {
        const tx = await this.tx(OBJECT_STORE_NAND, OBJECT_STORE_KVS);
        const store = tx.objectStore(OBJECT_STORE_NAND);

        let iPool = 0;
        for (let iPage = 0; iPage < nandScheduledPageCount; iPage++) {
            let compressedPage = compressNandPage(nandPagePool.subarray(iPage * 1056, (iPage + 1) * 1056));

            if (typeof compressedPage !== 'number') {
                if (this.nandPagePool.length >= iPool) this.nandPagePool.push(new Uint32Array(1056));
                const pageBuffer = this.nandPagePool[iPool++];

                pageBuffer.set(compressedPage);
                compressedPage = pageBuffer;
            }

            store.put(compressedPage, nandScheduledPages[iPage]);
        }

        tx.objectStore(OBJECT_STORE_KVS).put(crc, KVS_NAND_CRC);

        await complete(tx);
    }
}
