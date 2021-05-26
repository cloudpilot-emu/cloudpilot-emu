import { DeviceId } from './DeviceId';
import { SessionMetadata } from './SessionMetadata';

export interface SessionImage {
    metadata?: SessionMetadata;
    deviceId: DeviceId;
    rom: Uint8Array;
    memory: Uint8Array;
    savestate?: Uint8Array;
}
