import { EventInterface } from 'microevent.ts';

export interface NetRpcResultPayload {
    sessionId: number;
    rpcData: Uint8Array;
}

export interface PlatformBackend {
    teardown(): void;

    netOpenSession(): Promise<number>;
    netCloseSession(sessionId: number): Promise<void>;
    netDispatchRpc(sessionId: number, rpcData: ArrayLike<number>): Promise<boolean>;

    clipboardRead(): Promise<string>;
    clipboardWrite(data: string): Promise<void>;

    isResumeFromBackground(): boolean;
    setWorkerRegistered(workerRegistered: boolean): Promise<void>;
    getWorkerFailed(): Promise<boolean>;
    clearWorkerFailed(): void;

    supportsNativeNetworkIntegration(): boolean;
    supportsNativeClipboard(): boolean;

    reload(): void;

    readonly netRpcResult: EventInterface<NetRpcResultPayload>;
}
