import { PwmUpdate } from '@common/bridge/Cloudpilot';
import { isIOS } from '@common/helper/browser';
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

const audioContextCtor = window.AudioContext || window.webkitAudioContext;

function withTimeout<T>(v: Promise<T>, timeout = 100): Promise<T> {
    return new Promise((resolve, reject) => {
        setTimeout(() => reject(new Error('timeout')), timeout);

        v.then(resolve, reject);
    });
}

export abstract class AbstractAudioService {
    constructor(private emulationService: AbstractEmulationService) {
        this.bind();

        this.emulationService.openSessionEvent.addHandler(this.onOpenSession);

        this.emulationService.emulationStateChangeEvent.addHandler(() => this.updateState());
        this.emulationService.palmosStateChangeEvent.addHandler(() => this.updateState());

        if (!isIOS) document.addEventListener('visibilitychange', () => this.updateState());
    }

    initialize = (): Promise<boolean> =>
        this.mutex.runExclusive(async () => {
            for (let i = 0; i < 3; i++) {
                if (await this.initializeAudioUnguarded()) return false;
            }

            return true;
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

    protected bind(): void {
        this.unbind();

        this.pwmUpdateEvent = this.emulationService.pwmUpdateEvent();
        this.pwmUpdateEvent?.addHandler(this.onPwmUpdate);

        const pcmPort = this.emulationService.pcmPort();
        if (!pcmPort) return;

        this.bindWorkletNode();
    }

    protected unbind(): void {
        this.pwmUpdateEvent?.removeHandler(this.onPwmUpdate);
        this.disconnectPwmSource();
        this.pwmUpdateEvent = undefined;

        this.unbindWorkletNode();
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
                    this.emulationService.enablePcmStreaming();

                    console.log('resume audio context');
                } catch (e) {
                    console.error(`failed to resume audio from state ${oldState}`, e);
                }
            } else {
                try {
                    this.emulationService.disablePcmStreaming();
                    await withTimeout(this.context.suspend());

                    console.log('suspend audio context');
                } catch (e) {
                    console.error(`failed to suspend audio from state ${oldState}`, e);
                }
            }

            console.log(`audio context state change ${oldState} -> ${this.context.state}`);
        });

    protected updateGain(): void {
        if (this.gainNode) {
            this.gainNode.gain.value = this.gain();
        }
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
        if (this.initialized) return true;

        try {
            this.context = new audioContextCtor();
        } catch (e) {
            console.error('web audio not available', e);

            return false;
        }

        try {
            this.context.destination.channelCount = 1;
        } catch (e) {
            console.warn('audio driver: failed to set channel count', e);
        }

        try {
            await this.start();
        } catch (e) {
            console.error('failed to initialize audio context', e);

            return false;
        }

        await this.registerWorklet(this.context);

        this.context.addEventListener('statechange', () => {
            if ((this.context?.state as string) === 'interrupted') void this.updateState();
        });

        this.workletNode = new AudioWorkletNode(this.context, 'pcm-processor', {
            channelCount: 2,
            numberOfOutputs: 1,
            outputChannelCount: [2],
            processorOptions: {
                sampleRateTo: this.context.sampleRate,
            },
        });

        this.gainNode = this.context.createGain();
        // this.gainNode.channelCount = 1;
        this.gainNode.channelInterpretation = 'speakers';
        this.gainNode.gain.value = this.gain();

        this.gainNode.connect(this.context.destination);

        this.initialized = true;

        this.applyPwmUpdate();
        this.bindWorkletNode();

        return true;
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

    private onOpenSession = () => this.bind();

    private onPwmUpdate = async (pwmUpdate: PwmUpdate): Promise<void> => {
        this.pendingPwmUpdate = pwmUpdate;

        if (!this.initialized) return;

        if (this.shouldRun() !== this.isRunning()) void this.updateState(true);
        if (!this.shouldRun()) return;

        this.applyPwmUpdate();
    };

    private applyPwmUpdate(): void {
        if (!this.context || !this.pendingPwmUpdate) return;
        if (!this.gainNode) throw new Error('unreachable');

        const { frequency, dutyCycle } = this.pendingPwmUpdate;
        const sampleRate = this.context.sampleRate;

        this.pendingPwmUpdate = undefined;

        if (frequency <= 0 || dutyCycle <= 0 || frequency >= sampleRate) {
            this.disconnectPwmSource();

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

        this.disconnectPwmSource();

        bufferSourceNode.connect(this.gainNode);
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
        this.workletNode?.port.postMessage(message, transferables as Array<Transferable>);
    }

    private bindWorkletNode(): void {
        if (this.workletBound || !this.initialized) return;
        if (!this.workletNode || !this.gainNode) throw new Error('unreachable');

        const pcmPort = this.emulationService.pcmPort();
        if (!pcmPort) return;

        this.workletNode.connect(this.gainNode);
        this.dispatchPcmControlMessage({ type: ControlMessageHostType.setStreamMessagePort, port: pcmPort }, [pcmPort]);

        if (this.shouldRun()) this.emulationService.enablePcmStreaming();
        else this.emulationService.disablePcmStreaming();

        this.workletBound = true;
        console.log('bound worklet');
    }

    private unbindWorkletNode(): void {
        if (!this.workletBound) return;
        this.workletNode?.disconnect();

        this.dispatchPcmControlMessage({ type: ControlMessageHostType.reset });
        this.emulationService.disablePcmStreaming();

        this.workletBound = false;
        console.log('unbound worklet');
    }

    private mutex = new Mutex();
    private context: AudioContext | undefined;

    private bufferSourceNode: AudioBufferSourceNode | undefined;
    private gainNode: GainNode | undefined;
    private workletNode: AudioWorkletNode | undefined;

    private initialized = false;
    private muted = false;
    private workletBound = false;

    private pwmUpdateEvent: EventInterface<PwmUpdate> | undefined;
    private pendingPwmUpdate: PwmUpdate | undefined;
}
