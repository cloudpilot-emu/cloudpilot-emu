// tslint:disable: no-bitwise

import { Injectable } from '@angular/core';
import { fileURLToPath } from 'url';

const IMAGE_MAGIC = 0x20150103 | 0;
const IMAGE_VERSION = 0x00000001 | 0;
const IMAGE_VERSION_MASK = 0x80000000 | 0;

export interface SessionMetadata {}

export interface FileDescriptor {
    name: string;
    content: Uint8Array;
}

export interface SessionImage {
    metadata?: SessionMetadata;
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
    openFile(handler: (file: FileDescriptor) => void): void {
        return this.openFilesImpl(false, (files) => {
            if (files.length > 0) handler(files[0]);
        });
    }

    openFiles(handler: (files: Array<FileDescriptor>) => void): void {
        return this.openFilesImpl(true, handler);
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
            savestate: buffer.subarray(28 + deviceIdSize + metadataSize + romSize + memorySize),
        };
    }

    private parseSessionImageLegacy(buffer: Uint8Array): SessionImage | undefined {
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

    private openFilesImpl(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): void {
        const input: HTMLInputElement = document.createElement('input');

        input.type = 'file';
        input.multiple = multiple;
        input.onchange = async (e) => {
            const target = e.target as HTMLInputElement;

            if (!target?.files?.length) return [];

            const result: Array<Promise<FileDescriptor>> = [];

            for (let i = 0; i < target.files?.length; i++) {
                const file = target.files.item(i);

                if (!file) continue;

                const content = new Promise<FileDescriptor>((resolve, reject) => {
                    const reader = new FileReader();

                    reader.onload = () =>
                        resolve({ content: new Uint8Array(reader.result as ArrayBuffer), name: file.name });
                    reader.onerror = () => reject(reader.error);

                    reader.readAsArrayBuffer(file);
                });

                result.push(content);
            }

            handler(await Promise.all(result));
        };

        input.click();
    }
}
