import { Event } from 'microevent.ts';

import { NetRpcResultPayload, PlatformBackend } from './platform-backend';

export class PlatformBrowser implements PlatformBackend {
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

    reload(): void {
        location.reload();
    }

    readonly netRpcResult = new Event<NetRpcResultPayload>();
}
