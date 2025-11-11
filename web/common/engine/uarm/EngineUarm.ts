/* eslint-disable @typescript-eslint/no-unused-vars */
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';

import { BackupResult, EngineUarm, FullState, StorageCardProvider } from '../Engine';
import { EngineSettings } from '../EngineSettings';
import { SnapshotContainer } from '../Snapshot';
import { RpcHost } from './RpcHost';
import { ClientMessage, ClientMessageType } from './worker/ClientMessage';
import { HostMessage, HostMessageType } from './worker/HostMessage';
import { RcpMethod } from './worker/rpc';

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

    private constructor(private worker: Worker) {
        this.rpcHost = new RpcHost(this.dispatchMessage);

        worker.addEventListener('message', this.onMessage);
    }

    static async create(uarmModule: WebAssembly.Module, settings: EngineSettings): Promise<EngineUarmImpl> {
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
                    resolve(new EngineUarmImpl(worker));
                } else {
                    reject(new Error('invalid message from worker'));
                }
            };

            worker.addEventListener('error', onError);
            worker.addEventListener('message', onMessage, { once: true });

            handlers.push(['error', onError], ['message', onMessage]);
        });

        return engine.initialize(uarmModule, settings);
    }

    shutdown(): void {
        this.worker.removeEventListener('message', this.onMessage);
        this.worker.terminate();
    }

    getStatistics(): EmulationStatisticsUarm {
        throw new Error('Method not implemented.');
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
        throw new Error('Method not implemented.');
    }

    isPowerOff(): boolean {
        throw new Error('Method not implemented.');
    }

    isUIInitialized(): boolean {
        throw new Error('Method not implemented.');
    }

    isSlowdown(): boolean {
        throw new Error('Method not implemented.');
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

        return this.rpcHost.call(
            RcpMethod.openSession,
            { rom, nand, memory, state, card },
            [
                rom.buffer,
                nand?.buffer,
                memory?.buffer,
                state?.buffer,
                this.card.state === CardState.allocated ? this.card.data.buffer : undefined,
            ].filter((x) => x !== undefined),
        );
    }

    resume(): Promise<void> {
        throw new Error('Method not implemented.');
    }

    stop(): Promise<void> {
        throw new Error('Method not implemented.');
    }

    processTimesliceNow(): void {
        throw new Error('Method not implemented.');
    }

    requestSnapshot(): Promise<SnapshotContainer> {
        throw new Error('Method not implemented.');
    }

    blitFrame(canvas: HTMLCanvasElement): void {
        throw new Error('Method not implemented.');
    }

    updateSettings(settings: EngineSettings): void {
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

    private async initialize(module: WebAssembly.Module, settings: EngineSettings): Promise<this> {
        await this.rpcHost.call(RcpMethod.initialize, { module, settings });

        return this;
    }

    private fatal(error: Error | string): void {
        this.fatalError.dispatch(error instanceof Error ? error : new Error(error));
    }

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

    deviceId: DeviceId | undefined;

    card: Card = { state: CardState.none };

    private rpcHost: RpcHost;
}
