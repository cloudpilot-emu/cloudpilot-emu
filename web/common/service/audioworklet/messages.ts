export const enum StreamMessageHostType {
    flush = 'flush',
    sampleData = 'sampleData',
}

export interface StreamMessageHostReset {
    type: StreamMessageHostType.flush;
}

export interface StreamMessageHostSampleData {
    type: StreamMessageHostType.sampleData;
    buffer: ArrayBufferLike;
    count: number;
}

export type StreamMessageHost = StreamMessageHostReset | StreamMessageHostSampleData;

export const enum StreamMessageClientType {
    suspendPcm = 'suspendPcm',
    resumePcm = 'resumePcm',
    returnBuffer = 'returnBuffer',
}

export interface StreamMessageClientSuspendPcm {
    type: StreamMessageClientType.suspendPcm;
}

export interface StreamMessageClientResumePcm {
    type: StreamMessageClientType.resumePcm;
}

export interface StreamMessageClientReturnBuffer {
    type: StreamMessageClientType.returnBuffer;
    buffer: ArrayBufferLike;
}

export type StreamMessageClient =
    | StreamMessageClientResumePcm
    | StreamMessageClientSuspendPcm
    | StreamMessageClientReturnBuffer;

export const enum ControlMessageHostType {
    setStreamMessagePort = 'setStreamMessagePort',
    flush = 'flush',
    reset = 'reset',
}

export interface ControlMessageHostSetStreamMessagePort {
    type: ControlMessageHostType.setStreamMessagePort;
    port: MessagePort;
}

export interface ControlMessageHostFlush {
    type: ControlMessageHostType.flush;
}

export interface ControlMessageHostReset {
    type: ControlMessageHostType.reset;
}

export type ControlMessageHost =
    | ControlMessageHostSetStreamMessagePort
    | ControlMessageHostFlush
    | ControlMessageHostReset;
