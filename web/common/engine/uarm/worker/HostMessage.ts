import { EngineSettings } from '@common/engine/EngineSettings';

import { RpcRequest } from './rpc';

export const enum HostMessageType {
    rpcCall = 'rpcCall',
    updateSettings = 'updateSettings',
}

export type HostMessageRpcCall = RpcRequest & {
    type: HostMessageType.rpcCall;
};

export interface HostMessageUpdateSettings {
    type: HostMessageType.updateSettings;
    settings: EngineSettings;
}

export type HostMessage = HostMessageRpcCall | HostMessageUpdateSettings;
