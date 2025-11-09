import { EngineSettings } from '@common/engine/EngineSettings';

export const enum RcpMethod {
    initialize = 'initialize',
    updateSettings = 'updateSettings',
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

export interface RpcRequestUpdateSettings extends RpcPayload<RcpMethod.updateSettings> {
    args: EngineSettings;
}

export interface RpcResponseUpdateSettings extends RpcPayload<RcpMethod.updateSettings> {
    result: void;
}

export interface RpcError extends RpcPayload<RcpMethod> {
    error: string;
}

export type RpcRequest = RpcRequestInitialize | RpcRequestUpdateSettings;

export type RpcResponse = RpcResponseInitialize | RpcResponseUpdateSettings;

export type RpcRequestForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcRequestInitialize
    : RpcRequestUpdateSettings;

export type RpcResponseForMethod<M extends RcpMethod> = M extends RcpMethod.initialize
    ? RpcResponseInitialize
    : RpcResponseUpdateSettings;

export type RpcArgsForMethod<M extends RcpMethod> = RpcRequestForMethod<M>['args'];

export type RpcResultForMethod<M extends RcpMethod> = RpcResponseForMethod<M>['result'];
