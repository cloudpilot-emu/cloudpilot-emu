import { EventInterface } from 'microevent.ts';

import { AppChannel } from '@pwa/model/AppChannel';

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

    reload(): void;

    getAppChannel(): Promise<AppChannel>;
    switchAppChannel(channel: AppChannel): void;

    getAppVersion(): string | undefined;
    getAppPlatform(): string | undefined;

    saveFile(content: Uint8Array, name: string): Promise<void>;

    supportsNativeNetworkIntegration(): boolean;
    supportsNativeClipboard(): boolean;
    supportsChannelManagement(): boolean;
    supportsSaveFile(): boolean;

    readonly netRpcResult: EventInterface<NetRpcResultPayload>;
}
