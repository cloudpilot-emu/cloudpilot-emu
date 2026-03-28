import { Channel, invoke } from '@tauri-apps/api/core';
import { UnlistenFn } from '@tauri-apps/api/event';
import { readText as clipboardReadText, writeText as clipboardWriteText } from '@tauri-apps/plugin-clipboard-manager';
import { Event } from 'microevent.ts';

import { NativeAppBackend, NetRpcResultPayload } from './native-app-backend';

declare global {
    interface Window {
        __cpe_shim_tauri_api_version__?: number;
    }
}

interface RpcResultInternal {
    session_id: number;
    rpc_data: ArrayLike<number>;
}

export class NativeAppBackendTauri implements NativeAppBackend {
    static isSupported(): boolean {
        return typeof window.__cpe_shim_tauri_api_version__ === 'number';
    }

    constructor() {
        void this.initializeRpc().catch((e) => console.error('failed to initializate Tauri RPC', e));
    }

    teardown(): void {
        this.rpcResultChannel.onmessage = () => undefined;
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

    private async initializeRpc(): Promise<void> {
        await invoke<void>('net_set_rpc_result_channel', { channel: this.rpcResultChannel });
        if (this.isDestroyed) return;

        this.rpcResultChannel.onmessage = (payload) => {
            console.log(payload);
            this.netRpcResult.dispatch({ sessionId: payload.session_id, rpcData: Uint8Array.from(payload.rpc_data) });
        };
    }

    private unlistenNetRpcResult: UnlistenFn | undefined;
    private isDestroyed = false;
    private rpcResultChannel = new Channel<RpcResultInternal>();
}
