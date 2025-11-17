declare class AudioWorkletProcessor {
    port: MessagePort;
}

interface AudioWorkletProcessorImpl extends AudioWorkletProcessor {
    process(inputs: Float32Array[][], outputs: Float32Array[][], parameters: Record<string, Float32Array>): boolean;
}

interface AudioWorkletProcessorConstructor<T> {
    new (options: T): AudioWorkletProcessorImpl;
}

declare function registerProcessor<T = unknown>(name: string, processorCtor: AudioWorkletProcessorConstructor<T>): void;
