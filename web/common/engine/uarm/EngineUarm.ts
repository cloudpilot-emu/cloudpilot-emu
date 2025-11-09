/* eslint-disable @typescript-eslint/no-unused-vars */
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { Mutex } from 'async-mutex';
import { Event } from 'microevent.ts';

import { BackupResult, EngineUarm, FullState, StorageCardProvider } from '../Engine';
import { EngineSettings } from '../EngineSettings';
import { SnapshotContainer } from '../Snapshot';
import { RpcHost } from './RpcHost';
import { ClientMessage, ClientMessageType } from './worker/ClientMessage';
import { HostMessage } from './worker/HostMessage';
import { RcpMethod } from './worker/rpc';

export class EngineUarmImpl implements EngineUarm {
    readonly type = 'uarm';

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
        throw new Error('Method not implemented.');
    }

    penUp(): void {
        throw new Error('Method not implemented.');
    }

    buttonDown(button: PalmButton): void {
        throw new Error('Method not implemented.');
    }

    buttonUp(button: PalmButton): void {
        throw new Error('Method not implemented.');
    }

    keyStroke(key: number, ctrl?: boolean): void {
        throw new Error('Method not implemented.');
    }

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
        throw new Error('Method not implemented.');
    }

    openSession(
        rom: Uint8Array,
        device: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: StorageCardProvider,
    ): Promise<boolean> {
        throw new Error('Method not implemented.');
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
        void this.rpcMutex.runExclusive(() => this.rpcHost.call(RcpMethod.updateSettings, settings));
    }

    allocateCard(key: string, size: number): Uint32Array {
        throw new Error('Method not implemented.');
    }

    releaseCard(key: string): Promise<void> {
        throw new Error('Method not implemented.');
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

    private constructor(private worker: Worker) {
        this.rpcHost = new RpcHost(this.dispatchMessage);

        worker.addEventListener('message', this.onMessage);
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

    private rpcHost: RpcHost;
    private rpcMutex = new Mutex();
}
