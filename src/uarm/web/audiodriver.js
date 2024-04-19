const GRACE_TIME = 250;

function withTimeout(timeout, fn) {
    return new Promise((resolve, reject) => {
        const handle = setTimeout(() => reject(new Error('timeout')), timeout);

        fn()
            .then(resolve, reject)
            .finally(() => clearTimeout(handle));
    });
}

export class AudioDriver {
    constructor(log, onRunStateChange) {
        this.initialized = false;
        this.log = log;
        this.shouldBeRunning = true;
        this.updatingRunState = false;
        this.onRunStateChange = onRunStateChange;

        window.document.addEventListener('visibilitychange', () => {
            console.log(document.visibilityState);
            this.updateRunState();
        });
    }

    async initialize(emulator) {
        const audioContextCtor = window.AudioContext || window.webkitAudioContext;

        if (this.initialized) return;

        this.context = new audioContextCtor({ sampleRate: 44100, latencyHint: 'interactive' });

        if (this.context.destination.maxChannelCount > 0)
            this.context.destination.channelCount = Math.min(this.context.destination.maxChannelCount, 2);

        this.context.destination.channelInterpretation = 'speakers';

        this.gainNode = this.context.createGain();
        this.gainNode.gain.value = 0.8;
        this.gainNode.connect(this.context.destination);

        await this.context.audioWorklet.addModule(`web/pcmaudioprocessor.js?cb=${Date.now()}}`);

        this.workletNode = new AudioWorkletNode(this.context, 'pcm-processor', {
            channelCount: 2,
            numberOfOutputs: 1,
            outputChannelCount: [2],
        });

        this.workletNode.connect(this.gainNode);

        if (!(await this.updateRunState(true))) return;

        this.workerMessageChannel = new MessageChannel();
        this.workletNode.port.onmessage = (evt) => this.handleWorkletMessage(evt.data);
        this.workletNode.port.postMessage({ type: 'configure-worker-port', port: this.workerMessageChannel.port1 }, [
            this.workerMessageChannel.port1,
        ]);

        emulator.setupAudio(this.workerMessageChannel.port2);

        this.emulator = emulator;
        this.initialized = true;
    }

    async pause() {
        if (!this.initialized) return;
        this.shouldBeRunning = false;

        await this.updateRunState();
    }

    async resume() {
        if (!this.initialized) return;
        this.shouldBeRunning = true;

        await this.updateRunState();
    }

    isRunning() {
        return this.context?.state === 'running';
    }

    async updateRunState(force) {
        if (!this.initialized && !force) return true;

        if (this.updatingRunState) return;
        this.updatingRunState = true;

        try {
            if (this.shouldBeRunning && document.visibilityState === 'visible' && this.context.state !== 'running') {
                try {
                    await withTimeout(GRACE_TIME, () => this.context.resume());
                    console.log('audio context resumed');
                } catch (e) {
                    console.log('failed to resume audio context:', GRACE_TIME);
                    return false;
                }
            }

            if (
                (!this.shouldBeRunning || document.visibilityState === 'hidden') &&
                this.context.state !== 'suspended'
            ) {
                try {
                    await withTimeout(GRACE_TIME, () => this.context.suspend());
                    console.log('audio context paused');
                } catch (e) {
                    console.log('failed to suspend audio context:', GRACE_TIME);
                    return false;
                }
            }

            return true;
        } finally {
            this.onRunStateChange();

            if (this.context.state === 'running') this.emulator?.enablePcm();
            else this.emulator?.disablePcm();

            this.updatingRunState = false;
        }
    }

    handleWorkletMessage(message) {
        switch (message.type) {
            case 'log':
                this.log(message.message);
                break;

            default:
                console.log(`invalid message from audio worket: ${message.type}`);
                break;
        }
    }
}
