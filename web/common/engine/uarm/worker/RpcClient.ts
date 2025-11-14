import { ClientMessage, ClientMessageRpcSuccess, ClientMessageType } from './ClientMessage';
import { RpcArgsForMethod, RpcMethod, RpcRequest, RpcResultForMethod } from './rpc';

export type RpcHandler<M extends RpcMethod> = (
    args: RpcArgsForMethod<M>,
    rpcComplete: Promise<void>,
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

        let onRpcSuccess: () => void, onRpcError: (e: unknown) => void;
        const rpcComplete = new Promise<void>((resolve, reject) => {
            onRpcSuccess = resolve;
            onRpcError = reject;
        });

        try {
            let transferables: Array<Transferable> | undefined = undefined;
            const result = await callback(
                args,
                rpcComplete,
                (t) => void (transferables = [...(transferables ?? []), ...t]),
            );

            this.dispatchMessage(
                { type: ClientMessageType.rpcSuccess, method, id, result } as ClientMessageRpcSuccess,
                transferables,
            );

            onRpcSuccess!();
        } catch (e) {
            console.error(e);
            this.dispatchMessage({ type: ClientMessageType.rpcError, method, id, error: `${e}` });

            onRpcError!(e);
        }
    }

    register<M extends RpcMethod>(method: M, handler: RpcHandler<M>) {
        this.registeredMethods.set(method, handler as unknown as RpcHandler<RpcMethod>);

        return this;
    }

    private registeredMethods = new Map<string, RpcHandler<RpcMethod>>();
}
