import { ClientMessage, ClientMessageType } from './ClientMessage';
import { RcpMethod, RpcArgsForMethod, RpcRequest, RpcResultForMethod } from './rpc';

export type RpcHandler<M extends RcpMethod> = (
    args: RpcArgsForMethod<M>,
    rpcComplete: Promise<void>,
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
            const result = await callback(args, rpcComplete);
            this.dispatchMessage({ type: ClientMessageType.rpcSuccess, method, id, result });

            onRpcSuccess!();
        } catch (e) {
            console.error(e);
            this.dispatchMessage({ type: ClientMessageType.rpcError, method, id, error: `${e}` });

            onRpcError!(e);
        }
    }

    register<M extends RcpMethod>(method: M, handler: RpcHandler<M>) {
        this.registeredMethods.set(method, handler);

        return this;
    }

    private registeredMethods = new Map<string, RpcHandler<RcpMethod>>();
}
