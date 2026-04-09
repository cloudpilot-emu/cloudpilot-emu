import { Event } from 'microevent.ts';

import { PlatformService } from '../platform-service.service';
import { NetworkBackend } from './network-backend';

export class NativeBackendNative implements NetworkBackend {
    constructor(private platformService: PlatformService) {}

    async connect(): Promise<boolean> {
        if (this.sessionId !== undefined) throw new Error('session already open');

        this.sessionId = await this.platformService.netOpenSession();
        this.platformService.netRpcResult.addHandler(this.onNetRpcResult);

        return true;
    }

    async disconnect(): Promise<void> {
        if (this.sessionId === undefined) return;

        this.platformService.netRpcResult.removeHandler(this.onNetRpcResult);
        await this.platformService.netCloseSession(this.sessionId);

        this.sessionId = undefined;
    }

    async handleRpc(rpcData: Uint8Array): Promise<boolean> {
        if (this.sessionId === undefined) throw new Error('no open session');

        return this.platformService.netDispatchRpc(this.sessionId, rpcData);
    }

    private onNetRpcResult = ({ sessionId, rpcData }: { sessionId: number; rpcData: Uint8Array }) =>
        sessionId === this.sessionId && this.rpcResultEvent.dispatch(rpcData);

    rpcResultEvent = new Event<Uint8Array>();
    unexpectedCloseEvent = new Event<void>();

    private sessionId: number | undefined;
}
