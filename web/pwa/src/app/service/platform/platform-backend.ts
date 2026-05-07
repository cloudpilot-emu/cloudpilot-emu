import { EventInterface } from 'microevent.ts';

import { AppChannel } from '@pwa/model/AppChannel';

export interface NetRpcResultPayload {
    sessionId: number;
    rpcData: Uint8Array;
}

export interface SaveFileContext {
    showLoader: <T>(fn: (updateProgress: (progress: number) => void) => Promise<T>) => Promise<T>;
    onFail: () => void;
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

    saveFile(content: Uint8Array, name: string, context: SaveFileContext): Promise<void>;

    supportsNativeNetworkIntegration(): boolean;
    supportsNativeClipboard(): boolean;
    supportsChannelManagement(): boolean;
    needsPlatformSaveFile(): boolean;

    readonly netRpcResult: EventInterface<NetRpcResultPayload>;
}
