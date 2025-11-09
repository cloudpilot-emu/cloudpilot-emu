import { RpcRequest } from './rpc';

export const enum HostMessageType {
    rpcCall = 'rpcCall',
}

export type HostMessageRpcCall = RpcRequest & {
    type: HostMessageType.rpcCall;
};

export type HostMessage = HostMessageRpcCall;
