import { invoke } from '@tauri-apps/api/core';
import { UnlistenFn, listen } from '@tauri-apps/api/event';
import { readText as clipboardReadText, writeText as clipboardWriteText } from '@tauri-apps/plugin-clipboard-manager';
import { Event } from 'microevent.ts';

import { NativeAppBackend, NetRpcResultPayload } from './native-app-backend';

declare global {
    interface Window {
        __cpe_shim_tauri_api_version__?: number;
    }
}

interface NetRpcResultEvent {
    session_id: number;
    rpc_data: number[];
}

export class NativeAppBackendTauri implements NativeAppBackend {
    static isSupported(): boolean {
        return typeof window.__cpe_shim_tauri_api_version__ === 'number';
    }

    constructor() {
        this.initializeRpc();
    }

    teardown(): void {
        this.unlistenNetRpcResult?.();
        this.isDestroyed = true;
    }

    netOpenSession(): Promise<number> {
        return invoke<number>('net_open_session');
    }

    netCloseSession(sessionId: number): Promise<void> {
        return invoke('net_close_session', { sessionId });
    }

    netDispatchRpc(sessionId: number, rpcData: ArrayLike<number>): Promise<boolean> {
        return invoke<boolean>('net_dispatch_rpc', { sessionId, rpcData: Array.from(rpcData) });
    }

    clipboardRead(): Promise<string> {
        return clipboardReadText();
    }

    async clipboardWrite(text: string): Promise<void> {
        await clipboardWriteText(text);
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
        return true;
    }

    supportsNativeClipboard(): boolean {
        return true;
    }

    readonly netRpcResult = new Event<NetRpcResultPayload>();

    private initializeRpc(): void {
        void listen<NetRpcResultEvent>('net-rpc-result', (event) => {
            this.netRpcResult.dispatch({
                sessionId: event.payload.session_id,
                rpcData: new Uint8Array(event.payload.rpc_data),
            });
        }).then((unlisten) => {
            if (this.isDestroyed) {
                unlisten();
            } else {
                this.unlistenNetRpcResult = unlisten;
            }
        });
    }

    private unlistenNetRpcResult: UnlistenFn | undefined;
    private isDestroyed = false;
}
