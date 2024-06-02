class SampleQueue {
    constructor(capacity, sampleRateFrom, sampleRateTo) {
        this.capacity = capacity;
        this.sampleRateFrom = sampleRateFrom;
        this.sampleRateTo = sampleRateTo;

        this.channelLeftData = new Float32Array(this.capacity);
        this.channelRightData = new Float32Array(this.capacity);

        this.length = 0;
        this.nextSample = 0;

        this.currentSampleIndex = 0;
        this.currentSampleLeft = 0;
        this.currentSampleRight = 0;
    }

    push(sample) {
        this.channelLeftData[this.nextSample] = ((sample << 16) >> 16) / 0x7fff;
        this.channelRightData[this.nextSample] = (sample >> 16) / 0x7fff;

        if (this.length < this.capacity) {
            this.length++;
        }

        this.nextSample = (this.nextSample + 1) % this.capacity;
    }

    fill(channelLeft, channelRight) {
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

                    const increment = (this.currentSampleIndex / this.sampleRateTo) | 0;
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
}

class PcmProcessor extends AudioWorkletProcessor {
    constructor(parameters) {
        super(parameters);

        const sampleRateTo = parameters.processorOptions.sampleRateTo;

        this.sampleQueue = new SampleQueue((44100 / 60) * 10, 44100, sampleRateTo);
        this.buffering = true;
        this.backpressure = false;

        this.port.onmessage = (evt) => this.handleHostMessage(evt.data);
        this.onWorkerMessage = (evt) => this.handleWorkerMessage(evt.data);

        this.port.postMessage({
            type: 'log',
            message: `audio thread running at ${sampleRateTo} Hz`,
        });
    }

    handleHostMessage(message) {
        switch (message.type) {
            case 'configure-worker-port':
                this.setupWorkerPort(message.port);
                break;

            default:
                console.error(`audio processor: unknown host message ${message.type}`);
                break;
        }
    }

    handleWorkerMessage(message) {
        switch (message.type) {
            case 'sample-data': {
                const count = message.count;
                const samples = new Uint32Array(message.buffer);

                for (let i = 0; i < count; i++) this.sampleQueue.push(samples[i]);

                this.workerPort.postMessage({ type: 'return-buffer', buffer: message.buffer }, [message.buffer]);
                break;
            }

            default:
                console.error(`audio processor: unknown worker message ${message.type}`);
        }
    }

    setupWorkerPort(port) {
        if (this.workerPort) this.workerPort.removeEventListener('message', this.onWorkerMessage);

        this.workerPort = port;
        this.workerPort.addEventListener('message', this.onWorkerMessage);
        this.workerPort.start();
    }

    process(inputs, outputs) {
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
            this.workerPort?.postMessage({ type: 'resume-pcm' });
        }

        if (!this.backpressure && this.sampleQueue.length > (44100 / 60) * 8) {
            this.backpressure = true;
            this.workerPort?.postMessage({ type: 'suspend-pcm' });
        }

        if (!this.buffering) this.sampleQueue.fill(outputs[0][0], outputs[0][1]);

        return true;
    }
}

registerProcessor('pcm-processor', PcmProcessor);
