import { PwmUpdate } from '@common/bridge/Cloudpilot';
import { isIOS, isWebkit } from '@common/helper/browser';
import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { Mutex } from 'async-mutex';
import { EventInterface } from 'microevent.ts';

import { ControlMessageHost, ControlMessageHostType } from './audioworklet/messages';

type AudioContextType = typeof AudioContext;

declare global {
    interface Window {
        webkitAudioContext: AudioContextType;
        AudioContext: AudioContextType;
    }
}

interface Audio {
    context: AudioContext;
    gainNode: GainNode;
    workletNode: AudioWorkletNode;
}

const audioContextCtor = window.AudioContext || window.webkitAudioContext;

function withTimeout<T>(v: Promise<T>, timeout = 500): Promise<T> {
    return new Promise((resolve, reject) => {
        setTimeout(() => reject(new Error('timeout')), timeout);

        v.then(resolve, reject);
    });
}

export abstract class AbstractAudioService {
    constructor(private emulationService: AbstractEmulationService) {
        this.bind();

        this.emulationService.openSessionEvent.addHandler(() => this.bind());

        this.emulationService.emulationStateChangeEvent.addHandler(() => this.updateState());
        this.emulationService.palmosStateChangeEvent.addHandler(() => this.updateState());
        if (!isIOS) document.addEventListener('visibilitychange', () => this.updateState());
    }

    initialize = (): Promise<boolean> =>
        this.mutex.runExclusive(async () => {
            for (let i = 0; i < 3; i++) {
                if (await this.initializeAudioUnguarded()) return true;
            }

            return false;
        });

    isInitialized(): boolean {
        return this.audio !== undefined;
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
        return this.gain() > 0 && (document.visibilityState !== 'hidden' || isIOS || this.runHidden());
    }

    protected shouldMute(): boolean {
        return !this.emulationService.isRunning() || this.emulationService.isPowerOff() || this.muted;
    }

    protected updateState = () =>
        this.mutex.runExclusive(async () => {
            if (!this.audio) return;
            this.audio.gainNode.gain.value = this.shouldMute() ? 0 : this.gain();

            if (this.isRunning() === this.shouldRun()) return;

            const oldState = this.audio.context.state;

            if (this.shouldRun()) {
                try {
                    await withTimeout(this.audio.context.resume());

                    this.emulationService.enablePcmStreaming();
                    this.audio.gainNode.gain.value = this.shouldMute() ? 0 : this.gain();

                    console.log('resume audio context');
                } catch (e) {
                    console.error(`failed to resume audio from state ${oldState}`, e);
                }
            } else {
                try {
                    this.emulationService.disablePcmStreaming();
                    await withTimeout(this.audio.context.suspend());

                    console.log('suspend audio context');
                } catch (e) {
                    console.error(`failed to suspend audio from state ${oldState}`, e);
                }
            }

            console.log(`audio context state change ${oldState} -> ${this.audio.context.state}`);
        });

    private bind(): void {
        this.unbind();

        this.pwmUpdateEvent = this.emulationService.pwmUpdateEvent();
        this.pwmUpdateEvent?.addHandler(this.onPwmUpdate);

        const pcmPort = this.emulationService.pcmPort();
        if (!pcmPort) return;

        this.bindWorkletNode();
    }

    private unbind(): void {
        this.pwmUpdateEvent?.removeHandler(this.onPwmUpdate);
        this.disconnectPwmSource();
        this.pwmUpdateEvent = undefined;

        this.unbindWorkletNode();
    }

    private async registerWorklet(context: AudioContext): Promise<void> {
        // Trick webpack into bundling the worklet as a worker. Abomination.
        const savedWorker = window.Worker;
        window.Worker = class {
            constructor(url: URL) {
                this.ready = context.audioWorklet.addModule(url);
            }

            ready: Promise<void>;
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
        } as any;

        const loader = new Worker(new URL('./audioworklet/pcm-worklet.worker.ts', import.meta.url));
        window.Worker = savedWorker;

        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        await (loader as any).ready;
    }

    private async initializeAudioUnguarded(): Promise<boolean> {
        if (this.audio) return true;
        let context: AudioContext | undefined;

        try {
            context = new audioContextCtor({ sampleRate: isWebkit ? undefined : 44100, latencyHint: 'interactive' });
        } catch (e) {
            console.error('web audio not available', e);
            return false;
        }

        try {
            context.destination.channelCount = 2;
        } catch (e) {
            console.warn('audio driver: failed to set channel count', e);
        }

        try {
            await withTimeout(context.resume());
        } catch (e) {
            console.error('failed to initialize audio context', e);
            return false;
        }

        await this.registerWorklet(context);

        const workletNode = new AudioWorkletNode(context, 'pcm-processor', {
            channelCount: 2,
            numberOfOutputs: 1,
            outputChannelCount: [2],
            processorOptions: {
                sampleRateTo: context.sampleRate,
            },
        });

        const gainNode = context.createGain();
        gainNode.channelInterpretation = 'speakers';
        gainNode.gain.value = this.gain();

        gainNode.connect(context.destination);

        this.audio = { context, gainNode, workletNode };

        context.addEventListener('statechange', () => {
            if ((context?.state as string) === 'interrupted') void this.updateState();
        });

        this.applyPwmUpdate();
        this.bindWorkletNode();
        void this.updateState();

        console.log(`audio initialised at ${context.sampleRate}Hz`);

        return true;
    }

    private isRunning(): boolean {
        return this.audio?.context.state === 'running';
    }

    private onPwmUpdate = async (pwmUpdate: PwmUpdate): Promise<void> => {
        this.pendingPwmUpdate = pwmUpdate;

        this.applyPwmUpdate();
    };

    private applyPwmUpdate(): void {
        if (!this.audio || !this.pendingPwmUpdate) return;

        const { frequency, dutyCycle } = this.pendingPwmUpdate;
        const sampleRate = this.audio.context.sampleRate;

        this.pendingPwmUpdate = undefined;

        if (frequency <= 0 || dutyCycle <= 0 || frequency >= sampleRate) {
            this.disconnectPwmSource();

            return;
        }

        const buffer = this.audio.context.createBuffer(1, Math.round(sampleRate / frequency), sampleRate);
        const data = buffer.getChannelData(0);

        for (let i = 0; i < data.length; i++) {
            data[i] = i / data.length < dutyCycle ? 1 : 0;
        }

        const bufferSourceNode = this.audio.context.createBufferSource();
        bufferSourceNode.channelCount = 1;
        bufferSourceNode.channelInterpretation = 'speakers';
        bufferSourceNode.loop = true;
        bufferSourceNode.buffer = buffer;

        this.disconnectPwmSource();

        bufferSourceNode.connect(this.audio.gainNode);
        bufferSourceNode.start();
        this.bufferSourceNode = bufferSourceNode;

        this.pendingPwmUpdate = undefined;
    }

    private disconnectPwmSource(): void {
        if (this.bufferSourceNode) {
            this.bufferSourceNode.stop();
            this.bufferSourceNode.disconnect();
            this.bufferSourceNode = undefined;
        }
    }

    private gain(): number {
        return this.getVolume() / 2;
    }

    private dispatchPcmControlMessage(message: ControlMessageHost, transferables?: Array<Transferable>): void {
        this.audio?.workletNode.port.postMessage(message, transferables as Array<Transferable>);
    }

    private bindWorkletNode(): void {
        if (this.workletBound || !this.audio) return;

        const pcmPort = this.emulationService.pcmPort();
        if (!pcmPort) return;

        this.audio.workletNode.connect(this.audio.gainNode);
        this.dispatchPcmControlMessage({ type: ControlMessageHostType.setStreamMessagePort, port: pcmPort }, [pcmPort]);

        if (this.shouldRun()) this.emulationService.enablePcmStreaming();
        else this.emulationService.disablePcmStreaming();

        this.workletBound = true;
        console.log('bound worklet');
    }

    private unbindWorkletNode(): void {
        if (!this.workletBound) return;
        this.audio?.workletNode.disconnect();

        this.dispatchPcmControlMessage({ type: ControlMessageHostType.reset });
        this.emulationService.disablePcmStreaming();

        this.workletBound = false;
        console.log('unbound worklet');
    }

    private mutex = new Mutex();

    private bufferSourceNode: AudioBufferSourceNode | undefined;
    private audio: Audio | undefined;

    private muted = false;
    private workletBound = false;

    private pwmUpdateEvent: EventInterface<PwmUpdate> | undefined;
    private pendingPwmUpdate: PwmUpdate | undefined;
}
