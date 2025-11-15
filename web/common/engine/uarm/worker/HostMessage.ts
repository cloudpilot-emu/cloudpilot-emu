import { EngineSettings } from '@common/engine/EngineSettings';
import { PalmButton } from '@native/cloudpilot_web';

import { UarmSnapshot } from './Snapshot';
import { RpcRequest } from './rpc';

export const enum HostMessageType {
    rpcCall = 'rpcCall',
    updateSettings = 'updateSettings',
    buttonUp = 'buttonUp',
    buttonDown = 'buttonDown',
    penUp = 'penUp',
    penDown = 'penDown',
    returnFrame = 'returnFrame',
    returnSnapshot = 'returnSnapshot',
    setBackgrounded = 'setBackgrounded',
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

export interface HostMessageReturnSnapshot {
    type: HostMessageType.returnSnapshot;
    snapshot: UarmSnapshot;
    success: boolean;
}

export interface HostMessageSetBackgrounded {
    type: HostMessageType.setBackgrounded;
    backgrounded: boolean;
}

export type HostMessage =
    | HostMessageRpcCall
    | HostMessageUpdateSettings
    | HostMessageButtonUp
    | HostMessageButtonDown
    | HostMessagePenUp
    | HostMessagePenDown
    | HostMessageReturnFrame
    | HostMessageReturnSnapshot
    | HostMessageSetBackgrounded;
