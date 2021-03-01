import { SessionMetadata } from './SessionMetadata';

export interface SessionImage {
    metadata?: SessionMetadata;
    deviceId: string;
    rom: Uint8Array;
    memory: Uint8Array;
    savestate?: Uint8Array;
}
