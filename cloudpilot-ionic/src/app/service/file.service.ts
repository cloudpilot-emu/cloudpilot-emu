import { metadataForSession, serializeSessionImage } from '../helper/sessionFile';

import { Cloudpilot } from '../helper/Cloudpilot';
import { Injectable } from '@angular/core';
import { Session } from './../model/Session';
import { SessionImage } from './../model/SessionImage';
import { StorageService } from './storage.service';

// tslint:disable: no-bitwise

export interface FileDescriptor {
    name: string;
    content: Uint8Array;
}

function fileNameForSession(session: Session): string {
    const now = new Date();

    const year = now.getFullYear();
    const month = (now.getMonth() + 1).toString().padStart(2, '0');
    const day = now.getDate().toString().padStart(2, '0');
    const hour = now.getHours().toString().padStart(2, '0');
    const minute = now.getMinutes().toString().padStart(2, '0');
    const second = now.getSeconds().toString().padStart(2, '0');

    return `${session.name}_${year}${month}${day}-${hour}${minute}${second}.bin`;
}

@Injectable({
    providedIn: 'root',
})
export class FileService {
    constructor(private storageService: StorageService) {}

    openFile(handler: (file: FileDescriptor) => void): void {
        return this.openFilesImpl(false, (files) => {
            if (files.length > 0) handler(files[0]);
        });
    }

    openFiles(handler: (files: Array<FileDescriptor>) => void): void {
        return this.openFilesImpl(true, handler);
    }

    async saveSession(session: Session): Promise<void> {
        const [rom, memory, savestate] = await this.storageService.loadSession(session);

        if (!rom) {
            throw new Error(`invalid ROM ${session.rom}`);
        }

        const sessionImage: SessionImage = {
            deviceId: session.device,
            metadata: metadataForSession(session),
            rom,
            memory,
            savestate,
        };

        this.saveFile(fileNameForSession(session), serializeSessionImage(sessionImage));
    }

    emergencySaveSession(session: Session, cloudpilot: Cloudpilot): void {
        const rom = cloudpilot.getRomImage();
        const memory = cloudpilot.getMemory();
        const savestate = cloudpilot.saveState() ? cloudpilot.getSavestate() : undefined;

        const sessionImage: SessionImage = {
            deviceId: session.device,
            metadata: metadataForSession(session),
            rom,
            memory,
            savestate,
        };

        this.saveFile(fileNameForSession(session), serializeSessionImage(sessionImage));
    }

    saveFile(name: string, content: Uint8Array): void {
        const file = new Blob([content], { type: 'application/octet-stream' });
        const url = URL.createObjectURL(file);

        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.click();
    }

    private openFilesImpl(multiple: boolean, handler: (files: Array<FileDescriptor>) => void): void {
        if (this.input) {
            document.body.removeChild(this.input);
        }

        this.input = document.createElement('input');

        this.input.style.display = 'none';
        this.input.multiple = multiple;
        this.input.type = 'file';

        this.input.addEventListener('change', async (e) => {
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
        });

        document.body.appendChild(this.input);

        this.input.click();
    }

    private input: HTMLInputElement | undefined;
}
