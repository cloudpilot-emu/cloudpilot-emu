// tslint:disable: no-bitwise

import { Injectable } from '@angular/core';

const IMAGE_MAGIC = 0x20150103 | 0;
const IMAGE_VERSION = 0x00000001 | 0;
const IMAGE_VERSION_MASK = 0x80000000 | 0;

export interface FileDescriptor {
    name: string;
    content: Uint8Array;
}

export interface SessionImage {
    romName: string;
    deviceId: string;
    rom: Uint8Array;
    memory: Uint8Array;
    savestate: Uint8Array;
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

@Injectable({
    providedIn: 'root',
})
export class FileService {
    openFile(accept: string, handler: (file: FileDescriptor) => void): void {
        const input = document.createElement('input');

        input.type = 'file';
        input.accept = accept;
        input.onchange = async (e) => {
            const target = e.target as HTMLInputElement;

            if (target.files.length === 0) return;
            const file = target.files.item(0);

            const content = await new Promise<Uint8Array>((resolve, reject) => {
                const reader = new FileReader();

                reader.onload = () => resolve(new Uint8Array(reader.result as ArrayBuffer));
                reader.onerror = () => reject(reader.error);

                reader.readAsArrayBuffer(file);
            });

            handler({
                name: file.name,
                content,
            });
        };

        input.click();
    }

    parseSessionImage(buffer: Uint8Array): SessionImage | undefined {
        if (buffer.length < 16) return undefined;
        if (read32LE(buffer, 0) !== IMAGE_MAGIC) return undefined;
        if ((read32LE(buffer, 4) & IMAGE_VERSION_MASK) === 0) return this.parseSessionImageLegacy(buffer);
        if (read32LE(buffer, 4) !== (IMAGE_VERSION | IMAGE_VERSION_MASK)) return undefined;
        if (buffer.length < 28) return undefined;

        const deviceIdSize = read32LE(buffer, 8);
        const metadataSize = read32LE(buffer, 12);
        const romSize = read32LE(buffer, 16);
        const memorySize = read32LE(buffer, 20);
        const savestateSize = read32LE(buffer, 24);

        if (28 + deviceIdSize + metadataSize + romSize + memorySize + savestateSize !== buffer.length) return undefined;

        let romName = '[unknown]';
        try {
            const metadata = JSON.parse(
                new TextDecoder().decode(buffer.subarray(28 + deviceIdSize, 28 + deviceIdSize + metadataSize))
            );
            romName = metadata.romName ? metadata.romName : romName;
        } catch (e) {
            console.warn('metadata is not valid JSON');
        }

        return {
            romName,
            deviceId: new TextDecoder().decode(buffer.subarray(28, 28 + deviceIdSize)),
            rom: buffer.subarray(28 + deviceIdSize + metadataSize, 28 + deviceIdSize + metadataSize + romSize),
            memory: buffer.subarray(
                28 + deviceIdSize + metadataSize + romSize,
                28 + deviceIdSize + metadataSize + romSize + memorySize
            ),
            savestate: buffer.subarray(28 + deviceIdSize + metadataSize + romSize + memorySize),
        };
    }

    private parseSessionImageLegacy(buffer: Uint8Array): SessionImage {
        const romNameSize = read32LE(buffer, 4);
        const romSize = read32LE(buffer, 8);
        const memorySize = read32LE(buffer, 12);

        if (romNameSize + romSize + memorySize + 16 !== buffer.length) return undefined;

        return {
            romName: new TextDecoder().decode(buffer.subarray(16, 16 + romNameSize)),
            deviceId: 'PalmV',
            rom: buffer.subarray(16 + romNameSize, 16 + romNameSize + romSize),
            memory: buffer.subarray(16 + romNameSize + romSize),
            savestate: new Uint8Array(0),
        };
    }
}
