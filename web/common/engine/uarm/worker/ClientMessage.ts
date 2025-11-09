import { RpcError, RpcResponse } from './rpc';

export const enum ClientMessageType {
    ready = 'ready',
    rpcError = 'rpcError',
    rpcSuccess = 'rpcSuccess',
    fatalError = 'fatalErro',
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

export interface ClientMessageFatalError {
    type: ClientMessageType.fatalError;
    error: string;
}

export type ClientMessage =
    | ClientMessageReady
    | ClientMessageRpcError
    | ClientMessageRpcSuccess
    | ClientMessageFatalError;
