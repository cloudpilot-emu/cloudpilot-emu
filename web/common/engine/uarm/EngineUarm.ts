/* eslint-disable @typescript-eslint/no-unused-vars */
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { Executor } from '@common/service/AbstractEmulationService';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';

import { BackupResult, EngineUarm, FullState, StorageCardProvider } from '../Engine';
import { EngineSettings } from '../EngineSettings';
import { SnapshotContainer } from '../Snapshot';
import { RpcHost } from './RpcHost';
import { SnapshotContainerImpl } from './Snapshot';
import { ClientMessage, ClientMessageType } from './worker/ClientMessage';
import { HostMessage, HostMessageType } from './worker/HostMessage';

const enum CardState {
    none,
    allocated,
    mounted,
}

interface CardNone {
    state: CardState.none;
}

interface CardAllocated {
    state: CardState.allocated;
    key: string;
    data: Uint32Array;
}

interface CardMounted {
    state: CardState.mounted;
    key: string;
}

type Card = CardNone | CardAllocated | CardMounted;

export class EngineUarmImpl implements EngineUarm {
    readonly type = 'uarm';

    private constructor(
        private worker: Worker,
        private settings: EngineSettings,
        clandestineExecute: Executor,
    ) {
        this.rpcHost = new RpcHost(this.dispatchMessage);

        clandestineExecute(() => worker.addEventListener('message', this.onMessage));
    }

    static async create(
        uarmModule: WebAssembly.Module,
        settings: EngineSettings,
        clandestineExecute: Executor,
    ): Promise<EngineUarmImpl> {
        const worker = new Worker(new URL('./worker/main.worker', import.meta.url));

        const engine = await new Promise<EngineUarmImpl>((resolve, reject) => {
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            const handlers: Array<[string, any]> = [];
            const cleanupHandlers = () =>
                handlers.forEach(([evt, handler]) => worker.removeEventListener(evt, handler));

            const onError = (e: ErrorEvent) => {
                cleanupHandlers();
                reject(e.error);
            };

            const onMessage = (e: MessageEvent) => {
                cleanupHandlers();

                if ((e.data as ClientMessage).type === ClientMessageType.ready) {
                    resolve(new EngineUarmImpl(worker, settings, clandestineExecute));
                } else {
                    reject(new Error('invalid message from worker'));
                }
            };

            worker.addEventListener('error', onError);
            worker.addEventListener('message', onMessage, { once: true });

            handlers.push(['error', onError], ['message', onMessage]);
        });

        return engine.initialize(uarmModule);
    }

    shutdown(): void {
        this.worker.removeEventListener('message', this.onMessage);
        this.worker.terminate();

        document.removeEventListener('visibilitychange', this.onVisibilityChange);
    }

    getStatistics(): EmulationStatisticsUarm {
        return {
            type: 'uarm',
            currentSpeedMips: (this.currentIps / 1000000) | 0,
            currentMaxSpeedMips: (this.currentIpsMax / 1000000) | 0,
        };
    }

    getPcmPort(): MessagePort {
        throw new Error('Method not implemented.');
    }

    penDown(x: number, y: number): void {
        this.dispatchMessage({ type: HostMessageType.penDown, x, y });
    }

    penUp(): void {
        this.dispatchMessage({ type: HostMessageType.penUp });
    }

    buttonDown(button: PalmButton): void {
        this.dispatchMessage({ type: HostMessageType.buttonDown, button });
    }

    buttonUp(button: PalmButton): void {
        this.dispatchMessage({ type: HostMessageType.buttonUp, button });
    }

    keyStroke(key: number, ctrl?: boolean): void {}

    reset(): Promise<void> {
        throw new Error('Method not implemented.');
    }

    resetNoExtensions(): Promise<void> {
        throw new Error('Method not implemented.');
    }

    resetHard(): Promise<void> {
        throw new Error('Method not implemented.');
    }

    isRunning(): boolean {
        return this.running;
    }

    isPowerOff(): boolean {
        return false;
    }

    isUIInitialized(): boolean {
        return true;
    }

    isSlowdown(): boolean {
        return this.currentIps / 1000000 < this.settings.targetMips * this.settings.warnSlowdownThreshold;
    }

    getDeviceId(): DeviceId {
        if (this.deviceId === undefined) throw new Error('not initialized');

        return this.deviceId;
    }

    async openSession(
        rom: Uint8Array,
        device: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        cardProvider?: StorageCardProvider,
    ): Promise<boolean> {
        if (this.deviceId !== undefined) throw new Error('already initialized');
        this.deviceId = device;

        if (cardProvider) await cardProvider.load(this);

        const card: [Uint8Array, string] | undefined =
            this.card.state === CardState.allocated
                ? [new Uint8Array(this.card.data.buffer), this.card.key]
                : undefined;

        if (
            !(await this.rpcHost.call(
                'openSession',
                { rom, nand, memory, state, card },
                [
                    rom.buffer,
                    nand?.buffer,
                    memory?.buffer,
                    state?.buffer,
                    this.card.state === CardState.allocated ? this.card.data.buffer : undefined,
                ].filter((x) => x !== undefined),
            ))
        ) {
            return false;
        }

        const sizes = await this.rpcHost.call('getMemorySize', undefined);

        this.snapshotContainer = new SnapshotContainerImpl(
            sizes.memory,
            sizes.nand,
            this.rpcHost,
            this.dispatchMessage,
        );
        this.snapshotContainer.snapshotSuccessEvent.addHandler((statistics) =>
            this.snapshotSuccessEvent.dispatch(statistics),
        );

        return true;
    }

    async resume(): Promise<void> {
        await this.rpcHost.call('start', undefined);
        this.running = true;
    }

    async stop(): Promise<void> {
        await this.rpcHost.call('stop', undefined);
        this.running = false;

        this.returnPendingFrame();
    }

    processTimesliceNow(): void {
        throw new Error('Method not implemented.');
    }

    async requestSnapshot(): Promise<SnapshotContainer> {
        if (!this.snapshotContainer) throw new Error('emulator not initialized');

        return this.snapshotContainer.schedule(await this.rpcHost.call('takeSnapshot', undefined));
    }

    waitForPendingSnapshot(): Promise<void> {
        return this.rpcHost.call('waitForPendingSnapshot', undefined);
    }

    blitFrame(canvas: HTMLCanvasElement): void {
        if (!this.pendingFrame) return;

        const imageData = new ImageData(new Uint8ClampedArray(this.pendingFrame), 320);

        canvas.getContext('2d')?.putImageData(imageData, 0, 0);

        this.returnPendingFrame();
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
        this.dispatchMessage({ type: HostMessageType.updateSettings, settings });
    }

    allocateCard(key: string, size: number): Uint32Array {
        if (this.card.state !== CardState.none) throw new Error(`bad card state ${this.card.state}`);
        if (size & 0x03) throw new Error(`invalid card size ${size}`);

        this.card = {
            state: CardState.allocated,
            key,
            data: new Uint32Array(size >>> 2),
        };

        return this.card.data;
    }

    async releaseCard(key: string): Promise<void> {
        if (this.card.state === CardState.none) return;

        // TODO: unmount

        this.card = { state: CardState.none };
    }

    mountCard(key: string): Promise<boolean> {
        throw new Error('Method not implemented.');
    }

    getMountedKey(): Promise<string | undefined> {
        throw new Error('Method not implemented.');
    }

    getCardData(key: string): Promise<Uint32Array | undefined> {
        throw new Error('Method not implemented.');
    }

    getFullState(): Promise<FullState | undefined> {
        throw new Error('Method not implemented.');
    }

    installDb(data: Uint8Array): Promise<DbInstallResult> {
        throw new Error('Method not implemented.');
    }

    backup(includeRomDatabases: boolean): Promise<BackupResult | undefined> {
        throw new Error('Method not implemented.');
    }

    private dispatchMessage = (mesage: HostMessage, transferables?: Array<Transferable>) => {
        this.worker.postMessage(mesage, transferables as Array<Transferable>);
    };

    private async initialize(module: WebAssembly.Module): Promise<this> {
        await this.rpcHost.call('initialize', { module, settings: this.settings });
        document.addEventListener('visibilitychange', this.onVisibilityChange);

        return this;
    }

    private fatal(error: Error | string): void {
        this.fatalError.dispatch(error instanceof Error ? error : new Error(error));
    }

    private returnPendingFrame(): void {
        if (this.pendingFrame) {
            this.dispatchMessage({ type: HostMessageType.returnFrame, frame: this.pendingFrame }, [this.pendingFrame]);
        }

        this.pendingFrame = undefined;
    }

    private onVisibilityChange = (): void => {
        if (document.visibilityState === 'hidden') {
            this.dispatchMessage({ type: HostMessageType.setBackgrounded, backgrounded: true });
        } else {
            this.dispatchMessage({ type: HostMessageType.setBackgrounded, backgrounded: false });
        }
    };

    private onMessage = (e: MessageEvent): void => {
        const message = e.data as ClientMessage;

        switch (message.type) {
            case ClientMessageType.ready:
                return this.fatal('duplicate ready event');

            case ClientMessageType.rpcError:
            case ClientMessageType.rpcSuccess:
                return this.rpcHost.dispatchRpcMessage(message);

            case ClientMessageType.fatalError:
                return this.fatal(message.error);

            case ClientMessageType.timeslice:
                this.timesliceEvent.dispatch(message.sizeSeconds);
                this.currentIps = message.currentIps;
                this.currentIpsMax = message.currentIpsMax;

                if (message.frame) {
                    this.returnPendingFrame();
                    this.pendingFrame = message.frame;

                    this.newFrameEvent.dispatch();
                }

                break;

            case ClientMessageType.snapshot:
                try {
                    if (!this.snapshotContainer) {
                        throw new Error('Unreachable: emulator runs, but snapshots are not initialized');
                    }

                    this.snapshotEvent.dispatch(this.snapshotContainer.schedule(message.snapshot));
                } catch (e) {
                    this.fatal(`${e}`);

                    throw e;
                }

                break;

            default:
                message satisfies never;
        }
    };

    newFrameEvent = new Event<void>();
    timesliceEvent = new Event<number>();
    suspendEvent = new Event<void>();
    snapshotEvent = new Event<SnapshotContainer>();
    palmosStateChangeEvent = new Event<void>();
    fatalError = new Event<Error>();
    snapshotSuccessEvent = new Event<SnapshotStatistics>();

    private deviceId: DeviceId | undefined;
    private running = false;

    private currentIps = 0;
    private currentIpsMax = 0;

    private pendingFrame: ArrayBuffer | undefined;

    private card: Card = { state: CardState.none };

    private snapshotContainer: SnapshotContainerImpl | undefined;

    private rpcHost: RpcHost;
}
