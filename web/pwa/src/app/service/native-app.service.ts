import { Injectable, OnDestroy } from '@angular/core';
import { EventInterface } from 'microevent.ts';

import { createNativeAppBackend } from './native-app/factory';
import { NativeAppBackend, NetRpcResultPayload } from './native-app/native-app-backend';

@Injectable({ providedIn: 'root' })
export class NativeAppService implements OnDestroy {
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

    supportsNativeNetworkIntegration(): boolean {
        return this.backend.supportsNativeNetworkIntegration();
    }

    supportsNativeClipboard(): boolean {
        return this.backend.supportsNativeClipboard();
    }

    private backend: NativeAppBackend = createNativeAppBackend();
}
