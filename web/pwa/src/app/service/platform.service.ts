import { Injectable, OnDestroy } from '@angular/core';
import { EventInterface } from 'microevent.ts';

import { AppChannel } from '@pwa/model/AppChannel';

import { LifecylceService } from './lifecycle.service';
import { createNativeAppBackend } from './platform/factory';
import { NetRpcResultPayload, PlatformBackend } from './platform/platform-backend';

@Injectable({ providedIn: 'root' })
export class PlatformService implements OnDestroy {
    constructor(lifecycleService: LifecylceService) {
        this.backend = createNativeAppBackend(lifecycleService);

        if (this.backend.supportsChannelManagement()) {
            void this.backend.getAppChannel().then((channel) => (this.appChannel = channel));
        }
    }

    ngOnDestroy(): void {
        this.backend.teardown();
    }

    netOpenSession(): Promise<number> {
        return this.backend.netOpenSession();
    }

    netCloseSession(sessionId: number): Promise<void> {
        return this.backend.netCloseSession(sessionId);
    }

    netDispatchRpc(sessionId: number, rpcData: ArrayLike<number>): Promise<boolean> {
        return this.backend.netDispatchRpc(sessionId, rpcData);
    }

    clipboardRead(): Promise<string> {
        return this.backend.clipboardRead();
    }

    clipboardWrite(data: string): Promise<void> {
        return this.backend.clipboardWrite(data);
    }

    isResumeFromBackground(): boolean {
        return this.backend.isResumeFromBackground();
    }

    setWorkerRegistered(workerRegistered: boolean): Promise<void> {
        return this.backend.setWorkerRegistered(workerRegistered);
    }

    getWorkerFailed(): Promise<boolean> {
        return this.backend.getWorkerFailed();
    }

    clearWorkerFailed(): void {
        return this.backend.clearWorkerFailed();
    }

    get netRpcResult(): EventInterface<NetRpcResultPayload> {
        return this.backend.netRpcResult;
    }

    reload(): void {
        this.backend.reload();
    }

    getAppChannelSync(): AppChannel | undefined {
        return this.appChannel;
    }

    getAppChannel(): Promise<AppChannel> {
        return this.backend.getAppChannel();
    }

    switchAppChannel(channel: AppChannel): void {
        this.backend.switchAppChannel(channel);
    }

    supportsNativeNetworkIntegration(): boolean {
        return this.backend.supportsNativeNetworkIntegration();
    }

    supportsNativeClipboard(): boolean {
        return this.backend.supportsNativeClipboard();
    }

    supportsChannelManagement(): boolean {
        return this.backend.supportsChannelManagement();
    }

    private backend: PlatformBackend;
    private appChannel: AppChannel | undefined;
}
