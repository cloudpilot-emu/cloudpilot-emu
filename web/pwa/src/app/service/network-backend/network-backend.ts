import { EventInterface } from 'microevent.ts';

export interface NetworkBackend {
    connect(): Promise<boolean>;
    disconnect(): Promise<void>;
    handleRpc(rpcData: Uint8Array): Promise<boolean>;

    rpcResultEvent: EventInterface<Uint8Array>;
    unexpectedCloseEvent: EventInterface<void>;
}
