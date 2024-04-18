function withTimeout(timeout, fn) {
    return new Promise((resolve, reject) => {
        const handle = setTimeout(() => reject(new Error('timeout')), timeout);

        fn()
            .then(resolve, reject)
            .finally(() => clearTimeout(handle));
    });
}

export class AudioDriver {
    constructor(log) {
        this.initialized = false;
        this.log = log;
    }

    async initialize(emulator) {
        const audioContextCtor = window.AudioContext || window.webkitAudioContext;

        if (this.initialized) return;

        this.context = new audioContextCtor({ sampleRate: 44100, latencyHint: 'interactive' });
        this.context.destination.channelCount = 2;
        this.context.destination.channelInterpretation = 'speakers';

        this.gainNode = this.context.createGain();
        this.gainNode.gain.value = 1;
        this.gainNode.connect(this.context.destination);

        await this.context.audioWorklet.addModule('web/pcmaudioprocessor.js');

        this.workletNode = new AudioWorkletNode(this.context, 'pcm-processor', {
            channelCount: 2,
            numberOfOutputs: 1,
            numberOfInputs: 1,
            outputChannelCount: [2],
        });

        this.workletNode.connect(this.gainNode);

        await withTimeout(100, () => this.context.resume());

        this.workerMessageChannel = new MessageChannel();
        this.workletNode.port.onmessage = (evt) => this.handleWorkletMessage(evt.data);
        this.workletNode.port.postMessage({ type: 'configure-worker-port', port: this.workerMessageChannel.port1 }, [
            this.workerMessageChannel.port1,
        ]);

        emulator.setupAudio(this.workerMessageChannel.port2);

        this.initialized = true;
    }

    handleWorkletMessage(message) {
        switch (message.type) {
            case 'underrun':
                this.log('audio underrun');
                break;

            default:
                console.log(`invalid message from audio worket: ${message.type}`);
                break;
        }
    }
}
