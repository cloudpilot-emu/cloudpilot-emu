import { EngineSettings } from '@common/engine/EngineSettings';
import { DbInstallResult } from '@native/cloudpilot_web';

import { UarmSnapshot } from './Snapshot';

interface RpcSchema {
    initialize: {
        args: { module: WebAssembly.Module; settings: EngineSettings };
        result: void;
    };

    openSession: {
        args: {
            rom: Uint8Array;
            nand?: Uint8Array;
            memory?: Uint8Array;
            state?: Uint8Array;
            card?: [Uint8Array, string];
        };
        result: boolean;
    };

    start: {
        args: void;
        result: void;
    };

    stop: {
        args: void;
        result: void;
    };

    takeSnapshot: {
        args: void;
        result: UarmSnapshot;
    };

    refreshSnapshot: {
        args: UarmSnapshot;
        result: UarmSnapshot;
    };

    waitForPendingSnapshot: {
        args: void;
        result: void;
    };

    getMemorySize: {
        args: void;
        result: {
            memory: number;
            nand: number;
        };
    };

    sdCardInsert: {
        args: {
            data: Uint32Array;
            key: string;
        };
        result: boolean;
    };

    sdCardEject: {
        args: void;
        result: void;
    };

    getSdCardState: {
        args: void;
        result: { inserted: false } | { inserted: true; key: string };
    };

    installDb: {
        args: Uint8Array;
        result: DbInstallResult;
    };
}

export type RpcMethod = keyof RpcSchema;

export interface RpcError {
    method: RpcMethod;
    id: number;
    error: string;
}

export type RpcRequestForMethod<M extends RpcMethod> = {
    [K in RpcMethod]: { method: K; id: number; args: RpcSchema[K]['args'] };
}[M];

export type RpcResponseForMethod<M extends RpcMethod> = {
    [K in RpcMethod]: { method: K; id: number; result: RpcSchema[K]['result'] };
}[M];

export type RpcRequest = RpcRequestForMethod<RpcMethod>;

export type RpcResponse = RpcResponseForMethod<RpcMethod>;

export type RpcArgsForMethod<M extends RpcMethod> = RpcRequestForMethod<M>['args'];

export type RpcResultForMethod<M extends RpcMethod> = RpcResponseForMethod<M>['result'];
