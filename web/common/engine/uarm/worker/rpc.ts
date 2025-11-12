import { EngineSettings } from '@common/engine/EngineSettings';

export const enum RcpMethod {
    initialize = 'initialize',
    openSession = 'openSession',
    start = 'start',
    stop = 'stop',
}

interface RpcPayload<M extends RcpMethod> {
    id: number;
    method: M;
}

export interface RpcRequestInitialize extends RpcPayload<RcpMethod.initialize> {
    args: { module: WebAssembly.Module; settings: EngineSettings };
}

export interface RpcResponseInitialize extends RpcPayload<RcpMethod.initialize> {
    result: void;
}

export interface RpcRequestOpenSession extends RpcPayload<RcpMethod.openSession> {
    args: {
        rom: Uint8Array;
        nand?: Uint8Array;
        memory?: Uint8Array;
        state?: Uint8Array;
        card?: [Uint8Array, string];
    };
}

export interface RpcResponseOpenSession extends RpcPayload<RcpMethod.openSession> {
    result: boolean;
}

export interface RpcRequestStart extends RpcPayload<RcpMethod.start> {
    args: void;
}

export interface RpcResponseStart extends RpcPayload<RcpMethod.start> {
    result: boolean;
}

export interface RpcRequestStop extends RpcPayload<RcpMethod.stop> {
    args: void;
}

export interface RpcResponseStop extends RpcPayload<RcpMethod.stop> {
    result: boolean;
}

export interface RpcError extends RpcPayload<RcpMethod> {
    error: string;
}

export type RpcRequest = RpcRequestInitialize | RpcRequestOpenSession | RpcRequestStart | RpcRequestStop;

export type RpcResponse = RpcResponseInitialize | RpcResponseOpenSession | RpcResponseStart | RpcResponseStop;

export type RpcRequestForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcRequestInitialize
    : M extends RcpMethod.openSession
      ? RpcRequestOpenSession
      : M extends RcpMethod.start
        ? RpcRequestStart
        : M extends RcpMethod.stop
          ? RpcRequestStop
          : never;

export type RpcResponseForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcResponseInitialize
    : M extends RcpMethod.openSession
      ? RpcResponseOpenSession
      : M extends RcpMethod.start
        ? RpcResponseStart
        : M extends RcpMethod.stop
          ? RpcResponseStop
          : never;

export type RpcArgsForMethod<M extends RcpMethod> = RpcRequestForMethod<M>['args'];

export type RpcResultForMethod<M extends RcpMethod> = RpcResponseForMethod<M>['result'];
