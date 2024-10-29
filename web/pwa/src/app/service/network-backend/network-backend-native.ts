import { Event } from 'microevent.ts';
import { NetworkBackend } from './network-backend';
import { NativeAppService } from '../native-app.service';

export class NativeBackendNative implements NetworkBackend {
    constructor(private nativeAppService: NativeAppService) {}

    async connect(): Promise<boolean> {
        if (this.sessionId !== undefined) throw new Error('session already open');

        this.sessionId = await this.nativeAppService.netOpenSession();
        this.nativeAppService.netRpcResult.addHandler(this.onNetRpcResult);

        return true;
    }

    async disconnect(): Promise<void> {
        if (this.sessionId === undefined) return;

        this.nativeAppService.netRpcResult.removeHandler(this.onNetRpcResult);
        await this.nativeAppService.netCloseSession(this.sessionId);

        this.sessionId = undefined;
    }

    async handleRpc(rpcData: Uint8Array): Promise<boolean> {
        if (this.sessionId === undefined) throw new Error('no open session');

        return this.nativeAppService.netDispatchRpc(this.sessionId, rpcData);
    }

    private onNetRpcResult = ({ sessionId, rpcData }: { sessionId: number; rpcData: Uint8Array }) =>
        sessionId === this.sessionId && this.rpcResultEvent.dispatch(rpcData);

    rpcResultEvent = new Event<Uint8Array>();
    unexpectedCloseEvent = new Event<void>();

    private sessionId: number | undefined;
}
