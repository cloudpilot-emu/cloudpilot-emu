import { Event } from 'microevent.ts';

import { NativeAppBackend, NetRpcResultPayload } from './native-app-backend';

declare global {
    interface Window {
        __cpe_shim_tauri_api_version__?: number;
    }
}

export class NativeAppBackendTauri implements NativeAppBackend {
    static isSupported(): boolean {
        return typeof window.__cpe_shim_tauri_api_version__ === 'number';
    }

    teardown(): void {}

    netOpenSession(): Promise<number> {
        throw new Error('Native network integration not supported.');
    }

    netCloseSession(): Promise<void> {
        throw new Error('Native network integration not supported.');
    }

    netDispatchRpc(): Promise<boolean> {
        throw new Error('Native network integration not supported.');
    }

    clipboardRead(): Promise<string> {
        throw new Error('Native clipboard integration not supported.');
    }

    clipboardWrite(): Promise<void> {
        throw new Error('Native clipboard integration not supported.');
    }

    isResumeFromBackground(): boolean {
        return false;
    }

    async setWorkerRegistered(): Promise<void> {}

    async getWorkerFailed(): Promise<boolean> {
        return false;
    }

    clearWorkerFailed(): void {}

    supportsNativeNetworkIntegration(): boolean {
        return false;
    }

    supportsNativeClipboard(): boolean {
        return false;
    }

    readonly netRpcResult = new Event<NetRpcResultPayload>();
}
