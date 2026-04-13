import { Channel, invoke } from '@tauri-apps/api/core';
import { emit } from '@tauri-apps/api/event';
import { readText as clipboardReadText, writeText as clipboardWriteText } from '@tauri-apps/plugin-clipboard-manager';
import { Event } from 'microevent.ts';

import { AppChannel } from '@pwa/model/AppChannel';

import { LifecycleService } from '../lifecycle.service';
import { NetRpcResultPayload, PlatformBackend } from './platform-backend';

declare global {
    interface Window {
        __cpe_shim_tauri_version?: number;
        __cpe_shim_tauri_challenge?: string;
        __cpe_shim_tauri_bootstrap_cb?: () => void;
        __cpe_shim_tauri_handshake_sent?: boolean;

        __TAURI__: object;
    }
}

interface RpcResultInternal {
    session_id: number;
    rpc_data: ArrayLike<number>;
}

export function bootstrapTauriApp(): void {
    if (window.__TAURI__) return;

    const match = location.hash.match(/^#f55df283-698e-4ed4-ad5f-6617984e1ca3-(\d+)$/);
    if (!match) return;

    window.__cpe_shim_tauri_version = parseInt(match[1], 10);
}

export class PlatformBackendNativeAppTauri implements PlatformBackend {
    static isSupported(): boolean {
        return window.__cpe_shim_tauri_version !== undefined;
    }

    constructor(private lifecycleService: LifecycleService) {
        this.appChannel = new Promise((r) => (this.resolveAppChannel = r));

        if (window.__TAURI__) {
            this.initialize();
        } else {
            window.__cpe_shim_tauri_bootstrap_cb = this.initialize;
        }
    }

    teardown(): void {
        this.rpcResultChannel.onmessage = () => undefined;
        this.isDestroyed = true;
    }

    async netOpenSession(): Promise<number> {
        if (!this.isInitialized) {
            throw new Error('tauri init pending');
        }

        const id = await invoke<number>('net_open_session');

        if (id >= 0) return id;

        throw new Error('unable to open network session');
    }

    async netCloseSession(sessionId: number): Promise<void> {
        if (!this.isInitialized) return;

        await invoke('net_close_session', { sessionId });
    }

    async netDispatchRpc(sessionId: number, rpcData: ArrayLike<number>): Promise<boolean> {
        if (!this.isInitialized) return false;

        return invoke<boolean>('net_dispatch_rpc', { sessionId, rpcData: Array.from(rpcData) });
    }

    async clipboardRead(): Promise<string> {
        if (!this.isInitialized) return '';

        return clipboardReadText();
    }

    async clipboardWrite(text: string): Promise<void> {
        if (!this.isInitialized) return;

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

    reload(): void {
        if (!this.isInitialized) window.location.reload();

        void invoke('reload');
    }

    getAppChannel(): Promise<AppChannel> {
        return this.appChannel;
    }

    switchAppChannel(channel: AppChannel): void {
        void invoke('switch_app_channel', { channel });
    }

    getAppVersion(): string | undefined {
        const version = window.__cpe_shim_tauri_version;
        if (version === undefined) return undefined;

        const major = version >>> 16;
        const minor = (version >>> 8) & 0xff;
        const patch = version & 0xff;

        return `${major}.${minor}.${patch}`;
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

    private initialize = (): void => {
        void this.initializeRpc().catch((e) => console.error('failed to initializate Tauri RPC', e));
        void invoke<AppChannel>('get_app_channel').then(this.resolveAppChannel);

        this.isInitialized = true;
    };

    private async initializeRpc(): Promise<void> {
        void this.lifecycleService.appIsInitialized().then(() => {
            void emit('handshake', window.__cpe_shim_tauri_challenge);
            window.__cpe_shim_tauri_handshake_sent = true;
        });

        await invoke<void>('net_set_rpc_result_channel', { channel: this.rpcResultChannel });
        if (this.isDestroyed) return;

        this.rpcResultChannel.onmessage = (payload) =>
            this.netRpcResult.dispatch({ sessionId: payload.session_id, rpcData: Uint8Array.from(payload.rpc_data) });
    }

    private isDestroyed = false;
    private rpcResultChannel = new Channel<RpcResultInternal>();

    private isInitialized = false;

    private appChannel: Promise<AppChannel>;
    private resolveAppChannel: (channel: AppChannel | PromiseLike<AppChannel>) => void = () => undefined;
}
