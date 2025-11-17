/// <reference lib="webworker" />
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="./audioworklet.d.ts"/>
//
import {
    ControlMessageHost,
    ControlMessageHostType,
    StreamMessageClient,
    StreamMessageClientType,
    StreamMessageHost,
    StreamMessageHostType,
} from './messages';
import { PcmProcessorOptions, ProcessorNodeOptions } from './options';
import { WorkletType } from './worklet-types';

class SampleQueue {
    constructor(
        private capacity: number,
        private sampleRateFrom: number,
        private sampleRateTo: number,
    ) {
        this.channelLeftData = new Float32Array(this.capacity);
        this.channelRightData = new Float32Array(this.capacity);
    }

    push(sample: number) {
        this.channelLeftData[this.nextSample] = ((sample << 16) >> 16) / 0x7fff;
        this.channelRightData[this.nextSample] = (sample >> 16) / 0x7fff;

        if (this.length < this.capacity) {
            this.length++;
        }

        this.nextSample = (this.nextSample + 1) % this.capacity;
    }

    fill(channelLeft: Float32Array, channelRight: Float32Array) {
        const length = channelLeft.length;
        if (length !== channelRight.length) return;

        let iIn = (this.nextSample - this.length + this.capacity) % this.capacity;

        if (this.sampleRateFrom === this.sampleRateTo) {
            for (let iOut = 0; iOut < length && this.length > 0; iOut++) {
                channelLeft[iOut] = this.channelLeftData[iIn];
                channelRight[iOut] = this.channelRightData[iIn];

                iIn = (iIn + 1) % this.capacity;
                this.length--;
            }
        } else {
            for (let iOut = 0; iOut < length && this.length > 0; iOut++) {
                this.currentSampleIndex += this.sampleRateFrom;

                if (this.currentSampleIndex > this.sampleRateTo) {
                    this.currentSampleLeft = this.channelLeftData[iIn];
                    this.currentSampleRight = this.channelRightData[iIn];

                    let increment = (this.currentSampleIndex / this.sampleRateTo) | 0;
                    if (increment > this.length) increment = this.length;

                    iIn = (iIn + increment) % this.capacity;
                    this.currentSampleIndex %= this.sampleRateTo;
                    this.length -= increment;
                }

                channelLeft[iOut] = this.currentSampleLeft;
                channelRight[iOut] = this.currentSampleRight;
            }
        }
    }

    clear() {
        this.length = 0;
    }

    length = 0;
    private nextSample = 0;

    private channelLeftData: Float32Array;
    private channelRightData: Float32Array;

    private currentSampleIndex = 0;
    private currentSampleLeft = 0;
    private currentSampleRight = 0;
}

class PcmProcessor extends AudioWorkletProcessor implements AudioWorkletProcessorImpl {
    constructor(options: ProcessorNodeOptions<PcmProcessorOptions>) {
        super();

        const sampleRateTo = options.processorOptions.sampleRateTo;

        this.sampleQueue = new SampleQueue((44100 / 60) * 10, 44100, sampleRateTo);
        this.buffering = true;
        this.backpressure = false;

        this.port.addEventListener('message', this.onControlMessage);
        this.port.start();

        console.log(`pcm worklet initialized at ${sampleRateTo}Hz`);
    }

    process(inputs: Float32Array[][], outputs: Float32Array[][]): boolean {
        if (outputs.length !== 1 || outputs[0].length !== 2) return false;
        const len = outputs[0][0].length;

        if (this.buffering && this.sampleQueue.length > (44100 / 60) * 4) {
            this.buffering = false;
        }

        if (!this.buffering && this.sampleQueue.length < len) {
            this.buffering = true;
        }

        if (this.backpressure && this.sampleQueue.length < (44100 / 60) * 7) {
            this.backpressure = false;
            this.dispatchStreamMessage({ type: StreamMessageClientType.resumePcm });
        }

        if (!this.backpressure && this.sampleQueue.length > (44100 / 60) * 8) {
            this.backpressure = true;
            this.dispatchStreamMessage({ type: StreamMessageClientType.suspendPcm });
        }

        if (!this.buffering) this.sampleQueue.fill(outputs[0][0], outputs[0][1]);

        return true;
    }

    private dispatchStreamMessage(message: StreamMessageClient, transferables?: Array<Transferable>) {
        this.streamPort?.postMessage(message, transferables as Array<Transferable>);
    }

    private onStreamMessage = (e: MessageEvent) => {
        const message: StreamMessageHost = e.data;

        switch (message.type) {
            case StreamMessageHostType.sampleData: {
                const count = message.count;
                const samples = new Uint32Array(message.buffer);

                for (let i = 0; i < count; i++) this.sampleQueue.push(samples[i]);

                this.dispatchStreamMessage({ type: StreamMessageClientType.returnBuffer, buffer: message.buffer }, [
                    message.buffer,
                ]);

                break;
            }

            case StreamMessageHostType.flush:
                this.flush();
                this.dispatchStreamMessage({ type: StreamMessageClientType.resumePcm });

                break;

            default:
                message satisfies never;
        }
    };

    private onControlMessage = (e: MessageEvent) => {
        const message: ControlMessageHost = e.data;

        switch (message.type) {
            case ControlMessageHostType.setStreamMessagePort: {
                this.setStreamPort(message.port);
                break;
            }

            case ControlMessageHostType.flush:
                this.flush();
                this.dispatchStreamMessage({ type: StreamMessageClientType.resumePcm });

                break;

            case ControlMessageHostType.reset:
                this.clearStreamPort();

                break;

            default:
                message satisfies never;
        }
    };

    private flush(): void {
        this.buffering = true;
        this.backpressure = false;

        this.sampleQueue.clear();
    }

    private clearStreamPort(): void {
        if (this.streamPort) this.streamPort.removeEventListener('message', this.onStreamMessage);
        this.streamPort = undefined;

        this.flush();
    }

    private setStreamPort(port: MessagePort): void {
        this.clearStreamPort();

        this.streamPort = port;
        this.streamPort.addEventListener('message', this.onStreamMessage);
        this.streamPort.start();
    }

    private sampleQueue: SampleQueue;
    private buffering = true;
    private backpressure = false;

    private streamPort: MessagePort | undefined;
}

registerProcessor(WorkletType.pcmProcessor, PcmProcessor);
