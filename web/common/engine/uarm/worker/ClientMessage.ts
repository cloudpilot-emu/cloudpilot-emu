import { UarmSnapshot } from './Snapshot';
import { RpcError, RpcResponse } from './rpc';

export const enum ClientMessageType {
    ready = 'ready',
    rpcError = 'rpcError',
    rpcSuccess = 'rpcSuccess',
    timeslice = 'timeslice',
    fatalError = 'fatalError',
    snapshot = 'snapshot',
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

    currentIps: number;
    currentIpsMax: number;
}

export interface ClientMessageFatalError {
    type: ClientMessageType.fatalError;
    error: string;
}

export interface ClientMessageSnapshot {
    type: ClientMessageType.snapshot;
    snapshot: UarmSnapshot;
}

export type ClientMessage =
    | ClientMessageReady
    | ClientMessageRpcError
    | ClientMessageRpcSuccess
    | ClientMessageTimeslice
    | ClientMessageFatalError
    | ClientMessageSnapshot;
