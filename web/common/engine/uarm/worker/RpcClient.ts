import { ClientMessage, ClientMessageRpcSuccess, ClientMessageType } from './ClientMessage';
import { RpcArgsForMethod, RpcMethod, RpcRequest, RpcResultForMethod } from './rpc';

export type RpcHandler<M extends RpcMethod> = (
    args: RpcArgsForMethod<M>,
    addTransferables: (transferables: Array<Transferable>) => void,
) => Promise<RpcResultForMethod<M>> | RpcResultForMethod<M>;

export class RpcClient {
    constructor(private dispatchMessage: (message: ClientMessage, transferable?: Array<Transferable>) => void) {
        this.registeredMethods = new Map();
    }

    async dispatch(message: RpcRequest): Promise<void> {
        const { id, method, args } = message;

        const callback = this.registeredMethods.get(method);
        if (!callback) {
            this.dispatchMessage({
                type: ClientMessageType.rpcError,
                id,
                method,
                error: `method not registered: ${method}`,
            });
            return;
        }

        try {
            let transferables: Array<Transferable> | undefined = undefined;
            const result = await callback(args, (t) => void (transferables = [...(transferables ?? []), ...t]));

            this.dispatchMessage(
                { type: ClientMessageType.rpcSuccess, method, id, result } as ClientMessageRpcSuccess,
                transferables,
            );
        } catch (e) {
            console.error(e);
            this.dispatchMessage({ type: ClientMessageType.rpcError, method, id, error: `${e}` });
        }
    }

    register<M extends RpcMethod>(method: M, handler: RpcHandler<M>) {
        this.registeredMethods.set(method, handler as unknown as RpcHandler<RpcMethod>);

        return this;
    }

    private registeredMethods = new Map<string, RpcHandler<RpcMethod>>();
}
