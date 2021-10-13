import { DeviceId } from 'src/app/model/DeviceId';
import { SUPPORTED_DEVICES } from './Cloudpilot';
import { Session } from './../model/Session';
import { SessionImage } from '../model/SessionImage';
import { SessionMetadata } from '../model/SessionMetadata';

const IMAGE_MAGIC = 0x20150103 | 0;
const IMAGE_VERSION = 0x00000002 | 0;
const IMAGE_VERSION_MASK = 0x80000000 | 0;

export function deserializeSessionImage(buffer: Uint8Array): SessionImage | undefined {
    if (buffer.length < 16) return undefined;
    if (read32LE(buffer, 0) !== IMAGE_MAGIC) return undefined;
    if ((read32LE(buffer, 4) & IMAGE_VERSION_MASK) === 0) return parseSessionImageLegacy(buffer);

    const version = read32LE(buffer, 4) & ~IMAGE_VERSION_MASK;
    if (version > 2) return undefined;

    if (buffer.length < (version >= 2 ? 32 : 28)) return undefined;

    const deviceIdSize = read32LE(buffer, 8);
    const metadataSize = read32LE(buffer, 12);
    const romSize = read32LE(buffer, 16);
    const memorySize = read32LE(buffer, 20);
    const framebufferSize = version >= 2 ? read32LE(buffer, 24) : 0;
    const savestateSize = read32LE(buffer, version >= 2 ? 28 : 24);
    const dataStart = version >= 2 ? 32 : 28;

    if (dataStart + deviceIdSize + metadataSize + romSize + memorySize + savestateSize !== buffer.length) {
        return undefined;
    }

    const deviceId = new TextDecoder().decode(buffer.subarray(dataStart, dataStart + deviceIdSize)) as DeviceId;
    if (!SUPPORTED_DEVICES.includes(deviceId)) {
        return undefined;
    }

    let metadata: SessionMetadata | undefined;
    try {
        metadata = JSON.parse(
            new TextDecoder().decode(buffer.subarray(dataStart + deviceIdSize, dataStart + deviceIdSize + metadataSize))
        );
    } catch (e) {
        console.warn('metadata is not valid JSON');
    }

    return {
        metadata,
        deviceId,
        rom: buffer.subarray(
            dataStart + deviceIdSize + metadataSize,
            dataStart + deviceIdSize + metadataSize + romSize
        ),
        memory: buffer.subarray(
            dataStart + deviceIdSize + metadataSize + romSize,
            dataStart + deviceIdSize + metadataSize + romSize + memorySize
        ),
        savestate:
            savestateSize > 0
                ? buffer.subarray(dataStart + deviceIdSize + metadataSize + romSize + memorySize)
                : undefined,
        framebufferSize,
        version,
    };
}

export function metadataForSession(session: Session): SessionMetadata {
    const { name, hotsyncName, osVersion, dontManageHotsyncName } = session;

    return { name, hotsyncName, osVersion, dontManageHotsyncName };
}

export function serializeSessionImage(sessionImage: Omit<SessionImage, 'version'>): Uint8Array {
    const metadata = new TextEncoder().encode(JSON.stringify(sessionImage.metadata));
    const deviceId = new TextEncoder().encode(sessionImage.deviceId);

    const serializedImage = new Uint8Array(
        32 +
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
    write32LE(sessionImage.framebufferSize, serializedImage, 24);
    write32LE(sessionImage.savestate?.length ?? 0, serializedImage, 28);

    serializedImage.set(deviceId, 32);
    serializedImage.set(metadata, 32 + deviceId.length);
    serializedImage.set(sessionImage.rom, 32 + deviceId.length + metadata.length);
    serializedImage.set(sessionImage.memory, 32 + deviceId.length + metadata.length + sessionImage.rom.length);

    if (sessionImage.savestate) {
        serializedImage.set(
            sessionImage.savestate,
            32 + deviceId.length + metadata.length + sessionImage.rom.length + sessionImage.memory.length
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
        deviceId: DeviceId.palmV,
        rom: buffer.subarray(16 + romNameSize, 16 + romNameSize + romSize),
        memory: buffer.subarray(16 + romNameSize + romSize),
        savestate: new Uint8Array(0),
        framebufferSize: 0,
        version: 0,
    };
}
