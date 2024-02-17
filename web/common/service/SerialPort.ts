import { EventInterface } from 'microevent.ts';

export interface ReceivePayload {
    data: Uint8Array | undefined;
    isFrameComplete: boolean;
}

export interface SerialPort {
    send(data: Uint8Array | undefined, isFrameComplete: boolean): void;

    setModeSync(modeSync: boolean): void;
    getModeSync(): boolean;

    receiveEvent: EventInterface<ReceivePayload>;
}
