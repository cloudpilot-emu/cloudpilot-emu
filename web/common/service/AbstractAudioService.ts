import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { Mutex } from 'async-mutex';
import { PwmUpdate } from '@common/bridge/Cloudpilot';
import { isIOS } from '@common/helper/browser';

type AudioContextType = typeof AudioContext;

declare global {
    interface Window {
        webkitAudioContext: AudioContextType;
        AudioContext: AudioContextType;
    }
}

const audioContextCtor = window.AudioContext || window.webkitAudioContext;

function withTimeout<T>(v: Promise<T>, timeout = 200): Promise<T> {
    return new Promise((resolve, reject) => {
        setTimeout(() => reject(new Error('timeout')), timeout);

        v.then(resolve, reject);
    });
}
export abstract class AbstractAudioService {
    constructor(private emulationService: AbstractEmulationService) {
        this.emulationService.pwmUpdateEvent.addHandler(this.onPwmUpdate);

        this.emulationService.emulationStateChangeEvent.addHandler(() => this.updateState());
        this.emulationService.powerOffChangeEvent.addHandler(() => this.updateState());

        if (!isIOS) document.addEventListener('visibilitychange', () => this.updateState());
    }

    initialize = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (this.initialized) return;

            try {
                this.context = new audioContextCtor();
            } catch (e) {
                console.error(e);
                console.error('web audio not available');

                return;
            }

            try {
                this.context.destination.channelCount = 1;
            } catch (e) {
                console.warn('audio driver: failed to set channel count');
            }

            try {
                await this.start();
            } catch (e) {
                console.error(e);
                console.error('failed to initialize audio context');

                return;
            }

            this.context.addEventListener('statechange', () => {
                if ((this.context?.state as string) === 'interrupted') void this.updateState();
            });

            this.gainNode = this.context.createGain();
            this.gainNode.channelCount = 1;
            this.gainNode.channelInterpretation = 'speakers';
            this.gainNode.gain.value = this.gain();

            this.gainNode.connect(this.context.destination);

            this.applyPwmUpdate();

            this.initialized = true;
        });

    isInitialized(): boolean {
        return this.initialized;
    }

    mute(muted: boolean): void {
        this.muted = muted;

        void this.updateState();
    }

    isMuted(): boolean {
        return this.muted;
    }

    protected abstract getVolume(): number;

    protected abstract runHidden(): boolean;

    protected shouldRun(): boolean {
        return (
            this.emulationService.isRunning() &&
            !this.emulationService.isPowerOff() &&
            !this.muted &&
            this.gain() > 0 &&
            (document.visibilityState !== 'hidden' || isIOS || this.runHidden())
        );
    }

    protected updateState = (skipPwmUpdate = false) =>
        this.mutex.runExclusive(async () => {
            if (!this.context) return;

            if (this.isRunning() === this.shouldRun()) return;
            if (!skipPwmUpdate && !this.isRunning() && this.shouldRun()) this.applyPwmUpdate();

            const oldState = this.context.state;

            if (this.shouldRun()) {
                try {
                    this.updateGain();

                    await withTimeout(this.context.resume());

                    console.log('resume audio context');
                } catch (e) {
                    console.error(`failed to resume audio from state ${oldState}`);
                }
            } else {
                try {
                    await withTimeout(this.context.suspend());

                    console.log('suspend audio context');
                } catch (e) {
                    console.error(`failed to suspend audio from state ${oldState}`);
                }
            }

            console.log(`audio context state change ${oldState} -> ${this.context.state}`);
        });

    protected updateGain(): void {
        if (this.gainNode) {
            this.gainNode.gain.value = this.gain();
        }
    }

    private async start(): Promise<void> {
        if (!this.context) return;

        await withTimeout(this.context.resume());
        void this.updateState();

        console.log('audio context initialized');
    }

    private isRunning(): boolean {
        return this.context?.state === 'running';
    }

    private onPwmUpdate = async (pwmUpdate: PwmUpdate): Promise<void> => {
        this.pendingPwmUpdate = pwmUpdate;

        if (!this.initialized) return;

        if (this.shouldRun() !== this.isRunning()) void this.updateState(true);

        if (!this.shouldRun()) return;

        this.applyPwmUpdate();
    };

    private applyPwmUpdate(): void {
        if (!this.context || !this.pendingPwmUpdate) return;

        const { frequency, dutyCycle } = this.pendingPwmUpdate;
        const sampleRate = this.context.sampleRate;

        this.pendingPwmUpdate = undefined;

        if (frequency <= 0 || dutyCycle <= 0 || frequency >= sampleRate) {
            this.disconnectSource();

            return;
        }

        const buffer = this.context.createBuffer(1, Math.round(sampleRate / frequency), sampleRate);
        const data = buffer.getChannelData(0);

        for (let i = 0; i < data.length; i++) {
            data[i] = i / data.length < dutyCycle ? 1 : 0;
        }

        const bufferSourceNode = this.context.createBufferSource();
        bufferSourceNode.channelCount = 1;
        bufferSourceNode.channelInterpretation = 'speakers';
        bufferSourceNode.loop = true;
        bufferSourceNode.buffer = buffer;

        this.disconnectSource();

        bufferSourceNode.connect(this.gainNode);
        bufferSourceNode.start();
        this.bufferSourceNode = bufferSourceNode;

        this.pendingPwmUpdate = undefined;
    }

    private disconnectSource(): void {
        if (this.bufferSourceNode) {
            this.bufferSourceNode.stop();
            this.bufferSourceNode.disconnect();
            this.bufferSourceNode = undefined;
        }
    }

    private gain(): number {
        return this.getVolume() / 2;
    }

    private mutex = new Mutex();
    private context: AudioContext | undefined;

    private bufferSourceNode: AudioBufferSourceNode | undefined;
    private gainNode!: GainNode;

    private initialized = false;
    private muted = false;

    private pendingPwmUpdate: PwmUpdate | undefined;
}
