import { Channel, invoke } from '@tauri-apps/api/core';
import { emit } from '@tauri-apps/api/event';
import { readText as clipboardReadText, writeText as clipboardWriteText } from '@tauri-apps/plugin-clipboard-manager';
import { Event } from 'microevent.ts';

import { AppChannel } from '@pwa/model/AppChannel';

import { LifecylceService } from '../lifecycle.service';
import { NetRpcResultPayload, PlatformBackend } from './platform-backend';

declare global {
    interface Window {
        __cpe_shim_tauri_version?: number;
        __cpe_shim_tauri_challenge?: string;
    }
}

interface RpcResultInternal {
    session_id: number;
    rpc_data: ArrayLike<number>;
}

export class PlatformBackendNativeAppTauri implements PlatformBackend {
    static isSupported(): boolean {
        return typeof window.__cpe_shim_tauri_version === 'number';
    }

    constructor(private lifecycleService: LifecylceService) {
        void this.initializeRpc().catch((e) => console.error('failed to initializate Tauri RPC', e));

        this.appChannel = invoke('get_app_channel');
    }

    teardown(): void {
        this.rpcResultChannel.onmessage = () => undefined;
        this.isDestroyed = true;
    }

    async netOpenSession(): Promise<number> {
        const id = await invoke<number>('net_open_session');

        if (id >= 0) return id;

        throw new Error('unable to open network session');
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

    async setWorkerRegistered(workerInstalled: boolean): Promise<void> {
        void invoke('set_service_worker_installed', { workerInstalled });
    }

    async getWorkerFailed(): Promise<boolean> {
        return false;
    }

    clearWorkerFailed(): void {}

    reload(): void {
        void invoke('reload');
    }

    getAppChannel(): Promise<AppChannel> {
        return this.appChannel;
    }

    switchAppChannel(channel: AppChannel): void {
        void invoke('switch_app_channel', { channel });
    }

    supportsNativeNetworkIntegration(): boolean {
        return true;
    }

    supportsNativeClipboard(): boolean {
        return true;
    }

    supportsChannelManagement(): boolean {
        return true;
    }

    readonly netRpcResult = new Event<NetRpcResultPayload>();

    private async initializeRpc(): Promise<void> {
        if (typeof window.__cpe_shim_tauri_challenge !== 'undefined') {
            void this.lifecycleService
                .appIsInitialized()
                .then(() => emit('handshake', window.__cpe_shim_tauri_challenge));
        }

        await invoke<void>('net_set_rpc_result_channel', { channel: this.rpcResultChannel });
        if (this.isDestroyed) return;

        this.rpcResultChannel.onmessage = (payload) =>
            this.netRpcResult.dispatch({ sessionId: payload.session_id, rpcData: Uint8Array.from(payload.rpc_data) });
    }

    private isDestroyed = false;
    private rpcResultChannel = new Channel<RpcResultInternal>();
    private appChannel: Promise<AppChannel>;
}
