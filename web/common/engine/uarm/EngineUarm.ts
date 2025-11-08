/* eslint-disable @typescript-eslint/no-unused-vars */
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { Event } from 'microevent.ts';

import { BackupResult, EngineUarm, FullState, StorageCardProvider } from '../Engine';
import { EngineSettings } from '../EngineSettings';
import { SnapshotContainer } from '../Snapshot';
import { Message, MessageType } from './worker/message';

export class EngineUarmImpl implements EngineUarm {
    readonly type = 'uarm';

    private constructor(private worker: Worker) {}

    static create(uarmModule: WebAssembly.Module): Promise<EngineUarmImpl> {
        const worker = new Worker(new URL('./worker/main.worker', import.meta.url));

        return new Promise<EngineUarmImpl>((resolve, reject) => {
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            const handlers: Array<[string, any]> = [];

            const onError = (e: ErrorEvent) => {
                handlers.forEach(([evt, handler]) => worker.removeEventListener(evt, handler));

                reject(e.error);
            };

            const onMessage = (e: MessageEvent) => {
                handlers.forEach(([evt, handler]) => worker.removeEventListener(evt, handler));

                if ((e.data as Message).type === MessageType.ready) {
                    resolve(new EngineUarmImpl(worker));
                } else {
                    reject(new Error('invalid message from worker'));
                }
            };

            worker.addEventListener('error', onError);
            worker.addEventListener('message', onMessage, { once: true });

            handlers.push(['error', onError], ['message', onMessage]);
        });
    }

    shutdown(): void {
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
        throw new Error('Method not implemented.');
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

    newFrameEvent = new Event<void>();
    timesliceEvent = new Event<number>();
    suspendEvent = new Event<void>();
    snapshotEvent = new Event<SnapshotContainer>();
    palmosStateChangeEvent = new Event<void>();
    fatalError = new Event<Error>();
    snapshotSuccessEvent = new Event<SnapshotStatistics>();
}
