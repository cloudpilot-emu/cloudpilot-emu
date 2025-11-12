import { RpcError, RpcResponse } from './rpc';

export const enum ClientMessageType {
    ready = 'ready',
    rpcError = 'rpcError',
    rpcSuccess = 'rpcSuccess',
    timeslice = 'timeslice',
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

export interface ClientMessageTimeslice {
    type: ClientMessageType.timeslice;
    sizeSeconds: number;
    frame: ArrayBuffer | undefined;
}

export interface ClientMessageFatalError {
    type: ClientMessageType.fatalError;
    error: string;
}

export type ClientMessage =
    | ClientMessageReady
    | ClientMessageRpcError
    | ClientMessageRpcSuccess
    | ClientMessageTimeslice
    | ClientMessageFatalError;
