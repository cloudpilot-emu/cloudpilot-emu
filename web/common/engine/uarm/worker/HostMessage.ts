import { EngineSettings } from '@common/engine/EngineSettings';
import { PalmButton } from '@native/cloudpilot_web';

import { RpcRequest } from './rpc';

export const enum HostMessageType {
    rpcCall = 'rpcCall',
    updateSettings = 'updateSettings',
    buttonUp = 'buttonUp',
    buttonDown = 'buttonDown',
    penUp = 'penUp',
    penDown = 'penDown',
    returnFrame = 'returnFrame',
}

export type HostMessageRpcCall = RpcRequest & {
    type: HostMessageType.rpcCall;
};

export interface HostMessageUpdateSettings {
    type: HostMessageType.updateSettings;
    settings: EngineSettings;
}

export interface HostMessageButtonUp {
    type: HostMessageType.buttonUp;
    button: PalmButton;
}

export interface HostMessageButtonDown {
    type: HostMessageType.buttonDown;
    button: PalmButton;
}

export interface HostMessagePenUp {
    type: HostMessageType.penUp;
}

export interface HostMessagePenDown {
    type: HostMessageType.penDown;
    x: number;
    y: number;
}

export interface HostMessageReturnFrame {
    type: HostMessageType.returnFrame;
    frame: ArrayBuffer;
}

export type HostMessage =
    | HostMessageRpcCall
    | HostMessageUpdateSettings
    | HostMessageButtonUp
    | HostMessageButtonDown
    | HostMessagePenUp
    | HostMessagePenDown
    | HostMessageReturnFrame;
