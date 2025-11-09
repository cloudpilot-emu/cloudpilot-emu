import { RpcError, RpcResponse } from './rpc';

export const enum ClientMessageType {
    ready = 'ready',
    rpcError = 'rpcError',
    rpcSuccess = 'rpcSuccess',
}

export interface ClientMessageReady {
    type: ClientMessageType.ready;
}

export type ClientMessageRpcError = RpcError & {
    type: ClientMessageType.rpcError;
};

export type ClientMessageRpcSuccess = RpcResponse & {
    type: ClientMessageType.rpcSuccess;
};

export type ClientMessage = ClientMessageReady | ClientMessageRpcError | ClientMessageRpcSuccess;
