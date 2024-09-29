const MAGIC = 0x19800819;
const VERSION = 0;
const HEADER_SIZE = 36;

const BLOCK_TYPE_COPY = 0;
const BLOCK_TYPE_REPEAT = 1;

// A copy block has a potential overhead of 8 bytes:
//
// * 1 byte type
// * 1 byte length
// * 1 byte value
// * 1 byte type (repeat)
// * 4 byte length
const REPEAT_THRESHOLD = 8;

// Compressing data without repeating bytes will generate 9 bytes overhead:
//
// * 4 bytes file size
// * 1 byte type
// * 4 bytes length
//
// If we start with a copy block there will be an additional byte overhead due
// the type byte, so add another overhead byte
const MAX_OVERHEAD = 9;

function compress(data, log = false) {
    const compressedData = new Uint8Array(data.length + MAX_OVERHEAD);
    compressedData[0] = data.length & 0xff;
    compressedData[1] = (data.length >>> 8) & 0xff;
    compressedData[2] = (data.length >>> 16) & 0xff;
    compressedData[3] = (data.length >>> 24) & 0xff;

    let compressedCursor = 4;
    let state = 'first';
    let repeatFirst = 0;
    let cursor = 0;
    let crc = crc32_start();
    let lastValue;

    let putType = (type) => {
        compressedData[compressedCursor++] = type;
    };

    let putLength = (length) => {
        do {
            compressedData[compressedCursor++] = (length & 0x7f) | (length > 0x7f ? 0x80 : 0x00);
            length >>>= 7;
        } while (length !== 0);
    };

    let putCopy = (end) => {
        if (cursor === end) return;

        putType(BLOCK_TYPE_COPY);
        putLength(end - cursor);

        compressedData.subarray(compressedCursor, compressedCursor + (end - cursor)).set(data.subarray(cursor, end));
        compressedCursor += end - cursor;
    };

    let putRepeat = (value, repeatCount) => {
        putType(BLOCK_TYPE_REPEAT);
        putLength(repeatCount);

        compressedData[compressedCursor++] = value;
    };

    for (let i = 0; i <= data.length; i++) {
        const value = i < data.length ? data[i] : 0xffff;
        if (i < data.length) crc = crc32_add(crc, value);

        switch (state) {
            case 'first':
                lastValue = value;
                state = 'scan_copy';
                break;

            case 'scan_copy':
                if (value === lastValue) {
                    state = 'scan_repeat';
                    repeatFirst = i - 1;
                } else {
                    lastValue = value;
                }

                break;

            case 'scan_repeat':
                if (value !== lastValue) {
                    if (i - repeatFirst >= REPEAT_THRESHOLD) {
                        putCopy(repeatFirst);
                        cursor = repeatFirst;

                        putRepeat(lastValue, i - repeatFirst);
                        cursor = i;
                    }

                    state = 'scan_copy';
                    lastValue = value;
                }
        }
    }

    if (state === 'scan_copy') {
        if (cursor < data.length) putCopy(data.length);
    } else {
        throw new Error('unreachable');
    }

    return { data: compressedData.slice(0, compressedCursor), crc: crc32_finish(crc) };
}

function decompress(compressed, log = false) {
    if (compressed.length < 4) throw new Error('bad file length');

    const length = compressed[0] + (compressed[1] << 8) + (compressed[2] << 16) + (compressed[3] << 24);
    const data = new Uint8Array(length);

    let cursor = 0;
    let cursorCompressed = 4;
    let crc = crc32_start();

    while (cursor < length) {
        if (cursorCompressed >= compressed.length) throw new Error('not enough blocks');
        const blockType = compressed[cursorCompressed++];

        let blockLen = 0;
        let shift = 0;
        while (true) {
            if (cursorCompressed >= compressed.length) throw new Error('unable to read block length');

            blockLen |= (compressed[cursorCompressed] & 0x7f) << shift;
            shift += 7;

            if ((compressed[cursorCompressed++] & 0x80) === 0) break;
        }

        if (cursor + blockLen > length) throw new Error(`block exceeds file size @ 0x${cursorCompressed.toString(16)}`);

        switch (blockType) {
            case BLOCK_TYPE_COPY:
                if (cursorCompressed + blockLen > compressed.length)
                    throw new Error('copy block prematurely terminated');

                data.subarray(cursor, cursor + blockLen).set(
                    compressed.subarray(cursorCompressed, cursorCompressed + blockLen)
                );

                for (let i = cursorCompressed; i < cursorCompressed + blockLen; i++)
                    crc = crc32_add(crc, compressed[i]);

                cursorCompressed += blockLen;
                break;

            case BLOCK_TYPE_REPEAT: {
                if (cursorCompressed >= compressed.length) throw new Error('unable to read copy block value');

                const value = compressed[cursorCompressed++];
                data.subarray(cursor, cursor + blockLen).fill(value);

                for (let i = cursor; i < cursor + blockLen; i++) crc = crc32_add(crc, value);

                break;
            }

            default:
                throw new Error(`invalid block type ${blockType}`);
        }

        cursor += blockLen;
    }

    if (cursor !== length) throw new Error('chunk sizes do not add up');

    return { data, crc: crc32_finish(crc) };
}

export function compressSession({ nor, ram, nand, metadata = {} }) {
    const compressedNor = compress(nor);
    const compressedRam = compress(ram);
    const compressedNand = compress(nand);
    const serializedMetadata = new TextEncoder().encode(JSON.stringify(metadata));

    const image = new Uint8Array(
        HEADER_SIZE +
            serializedMetadata.length +
            compressedNor.data.length +
            compressedRam.data.length +
            compressedNand.data.length
    );
    let cursor = 0;

    const put32 = (x) => {
        image[cursor++] = x & 0xff;
        image[cursor++] = (x >> 8) & 0xff;
        image[cursor++] = (x >> 16) & 0xff;
        image[cursor++] = (x >> 24) & 0xff;
    };

    const putBlock = (block) => {
        image.subarray(cursor, cursor + block.length).set(block);
        cursor += block.length;
    };

    put32(MAGIC);
    put32(VERSION);

    put32(serializedMetadata.length);

    put32(compressedNor.data.length);
    put32(compressedNor.crc);

    put32(compressedRam.data.length);
    put32(compressedRam.crc);

    put32(compressedNand.data.length);
    put32(compressedNand.crc);

    putBlock(serializedMetadata);
    putBlock(compressedNor.data);
    putBlock(compressedRam.data);
    putBlock(compressedNand.data);

    return image;
}

export function decompressSession(sessionData) {
    if (sessionData.length < HEADER_SIZE) throw new Error('image too small');

    let cursor = 0;
    const get32 = () =>
        sessionData[cursor++] |
        (sessionData[cursor++] << 8) |
        (sessionData[cursor++] << 16) |
        (sessionData[cursor++] << 24);

    const getBlock = (length) => {
        const block = sessionData.subarray(cursor, cursor + length);
        cursor += length;

        return block;
    };

    if (get32() !== MAGIC) throw new Error('bad magic');
    if (get32() !== VERSION) throw new Error('invalid version');

    const sizeMetadata = get32();

    const sizeNorCompressed = get32();
    const crcNor = get32();

    const sizeRamCompressed = get32();
    const crcRam = get32();

    const sizeNandCompressed = get32();
    const crcNand = get32();

    if (sessionData.length !== HEADER_SIZE + sizeMetadata + sizeNorCompressed + sizeRamCompressed + sizeNandCompressed)
        throw new Error('image size mismatch');

    const metadata = JSON.parse(new TextDecoder().decode(getBlock(sizeMetadata)));
    const nor = decompress(getBlock(sizeNorCompressed));
    const ram = decompress(getBlock(sizeRamCompressed));
    const nand = decompress(getBlock(sizeNandCompressed));

    if (nor.crc !== crcNor) throw new Error('NOR CRC mismatch');
    if (ram.crc !== crcRam) throw new Error('RAM CRC mismatch');
    if (nand.crc !== crcNand) throw new Error('NAND CRC mismatch');

    return { nor: nor.data, ram: ram.data, nand: nand.data, metadata };
}
