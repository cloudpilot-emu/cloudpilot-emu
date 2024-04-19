class SampleQueue {
    constructor(capacity) {
        this.capacity = capacity;

        this.channelLeftData = new Float32Array(this.capacity);
        this.channelRightData = new Float32Array(this.capacity);

        this.length = 0;
        this.nextSample = 0;
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

        for (let iOut = 0; iOut < length && this.length > 0; iOut++) {
            if (this.length > 0) {
                channelLeft[iOut] = this.channelLeftData[iIn];
                channelRight[iOut] = this.channelRightData[iIn];

                iIn = (iIn + 1) % this.capacity;
                this.length--;
            }
        }
    }

    clear() {
        this.length = 0;
    }
}

class PcmProcessor extends AudioWorkletProcessor {
    constructor() {
        super();

        this.sampleQueue = new SampleQueue((44100 / 60) * 10);
        this.buffering = true;
        this.backpressure = false;

        this.port.onmessage = (evt) => this.handleHostMessage(evt.data);
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
        this.workerPort = port;
        port.onmessage = (evt) => this.handleWorkerMessage(evt.data);
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
