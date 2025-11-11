import { EngineSettings } from '@common/engine/EngineSettings';

export const enum RcpMethod {
    initialize = 'initialize',
    openSession = 'opemSession',
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

export interface RpcError extends RpcPayload<RcpMethod> {
    error: string;
}

export type RpcRequest = RpcRequestInitialize | RpcRequestOpenSession;

export type RpcResponse = RpcResponseInitialize | RpcResponseOpenSession;

export type RpcRequestForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcRequestInitialize
    : M extends RcpMethod.openSession
      ? RpcRequestOpenSession
      : never;

export type RpcResponseForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcResponseInitialize
    : M extends RcpMethod.openSession
      ? RpcResponseOpenSession
      : never;

export type RpcArgsForMethod<M extends RcpMethod> = RpcRequestForMethod<M>['args'];

export type RpcResultForMethod<M extends RcpMethod> = RpcResponseForMethod<M>['result'];
