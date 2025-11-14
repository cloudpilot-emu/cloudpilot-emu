import { ClientMessageRpcError, ClientMessageRpcSuccess } from './worker/ClientMessage';
import { HostMessage, HostMessageRpcCall, HostMessageType } from './worker/HostMessage';
import { RpcArgsForMethod, RpcMethod, RpcResultForMethod } from './worker/rpc';

interface RpcContext<T, E> {
    resolve(result: T): void;
    reject(e: E): void;
}

export class RpcHost {
    constructor(private dispatchMessage: (message: HostMessage, transferables?: Array<Transferable>) => void) {}

    call<M extends RpcMethod>(
        method: M,
        args: RpcArgsForMethod<M>,
        transferables?: Array<Transferable>,
    ): Promise<RpcResultForMethod<M>> {
        return new Promise((resolve, reject) => {
            const id = this.nextRpcId++;

            this.pendingCalls.set(id, { resolve, reject });

            this.dispatchMessage(
                { type: HostMessageType.rpcCall, id, method, args } as HostMessageRpcCall,
                transferables,
            );
        });
    }

    dispatchRpcMessage(message: ClientMessageRpcError | ClientMessageRpcSuccess): void {
        switch (message.type) {
            case 'rpcSuccess':
                this.getContext(message.id).resolve(message.result);
                break;

            case 'rpcError':
                this.getContext(message.id).reject(message.error);
                break;

            default:
                break;
        }
    }

    private getContext(id: number): RpcContext<unknown, unknown> {
        const ctx = this.pendingCalls.get(id);
        if (!ctx) throw new Error(`RPC failed: no context with ID ${id}`);

        this.pendingCalls.delete(id);
        return ctx;
    }

    private nextRpcId = 0;
    private pendingCalls = new Map<number, RpcContext<unknown, unknown>>();
}
