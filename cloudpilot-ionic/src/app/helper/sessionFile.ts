import { Session } from './../model/Session';
import { SessionImage } from '../model/SessionImage';
import { SessionMetadata } from '../model/SessionMetadata';

const IMAGE_MAGIC = 0x20150103 | 0;
const IMAGE_VERSION = 0x00000001 | 0;
const IMAGE_VERSION_MASK = 0x80000000 | 0;

export function deserializeSessionImage(buffer: Uint8Array): SessionImage | undefined {
    if (buffer.length < 16) return undefined;
    if (read32LE(buffer, 0) !== IMAGE_MAGIC) return undefined;
    if ((read32LE(buffer, 4) & IMAGE_VERSION_MASK) === 0) return parseSessionImageLegacy(buffer);
    if (read32LE(buffer, 4) !== (IMAGE_VERSION | IMAGE_VERSION_MASK)) return undefined;
    if (buffer.length < 28) return undefined;

    const deviceIdSize = read32LE(buffer, 8);
    const metadataSize = read32LE(buffer, 12);
    const romSize = read32LE(buffer, 16);
    const memorySize = read32LE(buffer, 20);
    const savestateSize = read32LE(buffer, 24);

    if (28 + deviceIdSize + metadataSize + romSize + memorySize + savestateSize !== buffer.length) return undefined;

    let metadata: SessionMetadata | undefined;
    try {
        metadata = JSON.parse(
            new TextDecoder().decode(buffer.subarray(28 + deviceIdSize, 28 + deviceIdSize + metadataSize))
        );
    } catch (e) {
        console.warn('metadata is not valid JSON');
    }

    return {
        metadata,
        deviceId: new TextDecoder().decode(buffer.subarray(28, 28 + deviceIdSize)),
        rom: buffer.subarray(28 + deviceIdSize + metadataSize, 28 + deviceIdSize + metadataSize + romSize),
        memory: buffer.subarray(
            28 + deviceIdSize + metadataSize + romSize,
            28 + deviceIdSize + metadataSize + romSize + memorySize
        ),
        savestate:
            savestateSize > 0 ? buffer.subarray(28 + deviceIdSize + metadataSize + romSize + memorySize) : undefined,
    };
}

export function metadataForSession(session: Session): SessionMetadata {
    return { name: session.name, hotsyncName: session.hotsyncName };
}

export function serializeSessionImage(sessionImage: SessionImage): Uint8Array {
    const metadata = new TextEncoder().encode(JSON.stringify(sessionImage.metadata));
    const deviceId = new TextEncoder().encode(sessionImage.deviceId);

    const serializedImage = new Uint8Array(
        28 +
            deviceId.length +
            metadata.length +
            sessionImage.rom.length +
            sessionImage.memory.length +
            (sessionImage.savestate?.length ?? 0)
    );

    write32LE(IMAGE_MAGIC, serializedImage, 0);
    write32LE(IMAGE_VERSION | IMAGE_VERSION_MASK, serializedImage, 4);
    write32LE(deviceId.length, serializedImage, 8);
    write32LE(metadata.length, serializedImage, 12);
    write32LE(sessionImage.rom.length, serializedImage, 16);
    write32LE(sessionImage.memory.length, serializedImage, 20);
    write32LE(sessionImage.savestate?.length ?? 0, serializedImage, 24);

    serializedImage.set(deviceId, 28);
    serializedImage.set(metadata, 28 + deviceId.length);
    serializedImage.set(sessionImage.rom, 28 + deviceId.length + metadata.length);
    serializedImage.set(sessionImage.memory, 28 + deviceId.length + metadata.length + sessionImage.rom.length);

    if (sessionImage.savestate) {
        serializedImage.set(
            sessionImage.savestate,
            28 + deviceId.length + metadata.length + sessionImage.rom.length + sessionImage.memory.length
        );
    }

    return serializedImage;
}

function write32LE(value: number, target: Uint8Array, i: number): void {
    target[i] = value & 0xff;
    target[i + 1] = (value >>> 8) & 0xff;
    target[i + 2] = (value >>> 16) & 0xff;
    target[i + 3] = (value >>> 24) & 0xff;
}

function read32LE(target: Uint8Array, i: number): number {
    return target[i] | (target[i + 1] << 8) | (target[i + 2] << 16) | (target[i + 3] << 24);
}

function parseSessionImageLegacy(buffer: Uint8Array): SessionImage | undefined {
    const romNameSize = read32LE(buffer, 4);
    const romSize = read32LE(buffer, 8);
    const memorySize = read32LE(buffer, 12);

    if (romNameSize + romSize + memorySize + 16 !== buffer.length) return undefined;

    return {
        deviceId: 'PalmV',
        rom: buffer.subarray(16 + romNameSize, 16 + romNameSize + romSize),
        memory: buffer.subarray(16 + romNameSize + romSize),
        savestate: new Uint8Array(0),
    };
}
