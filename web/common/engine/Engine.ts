import { PwmUpdate } from '@common/bridge/Cloudpilot';
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsCloudpilot, EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { EngineType } from '@common/model/EngineType';
import { SnapshotStatistics } from '@common/model/SnapshotStatistics';
import { SerialPort } from '@common/serial/SerialPort';
import { DbInstallResult, PalmButton } from '@native/cloudpilot_web';
import { EventInterface } from 'microevent.ts';

import { EngineSettings } from './EngineSettings';
import { SnapshotContainer } from './Snapshot';

export interface StorageCardProvider {
    storageKey: string;
    load(engine: Engine): Promise<void>;
}

export interface BackupResult {
    archive: Uint8Array;
    failedDatabases: Array<string>;
}

export interface FullState {
    rom: Uint8Array;
    memory: Uint8Array;
    savestate: Uint8Array;
    nand?: Uint8Array;
}

export interface EngineCommon {
    readonly type: EngineType;

    supportsKeyboardIO(): boolean;

    penDown(x: number, y: number): void;
    penUp(): void;

    buttonDown(button: PalmButton): void;
    buttonUp(button: PalmButton): void;

    keyStroke(key: number): void;

    reset(): Promise<void>;
    resetNoExtensions(): Promise<void>;
    resetHard(): Promise<void>;

    isRunning(): boolean;
    isPowerOff(): boolean;
    isLcdEnabled(): boolean;
    isUIInitialized(): boolean;
    isSlowdown(): boolean;
    getOSVersion(): number;

    getDeviceId(): DeviceId;

    openSession(
        rom: Uint8Array,
        device: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: StorageCardProvider,
    ): Promise<boolean>;

    resume(): Promise<void>;
    stop(): Promise<void>;
    processTimesliceNow(): void;

    requestSnapshot(): Promise<SnapshotContainer>;
    waitForPendingSnapshot(): Promise<void>;

    blitFrame(canvas: HTMLCanvasElement): void;

    updateSettings(settings: EngineSettings): void;

    shutdown(): void;

    allocateCard(key: string, size: number): Uint32Array;
    releaseCard(key: string): Promise<void>;
    mountCard(key: string): Promise<boolean>;
    getMountedKey(): Promise<string | undefined>;
    getCardData(key: string): Promise<Uint32Array | undefined>;

    getFullState(): Promise<FullState | undefined>;

    installDb(data: Uint8Array): Promise<DbInstallResult>;
    backup(includeRomDatabases: boolean): Promise<BackupResult | undefined>;

    launchAppByName(name: string): Promise<boolean>;
    launchAppByHeader(header: Uint8Array): Promise<boolean>;

    readonly newFrameEvent: EventInterface<void>;
    readonly timesliceEvent: EventInterface<number>;
    readonly suspendEvent: EventInterface<void>;
    readonly snapshotEvent: EventInterface<SnapshotContainer>;
    readonly palmosStateChangeEvent: EventInterface<void>;
    readonly fatalError: EventInterface<Error>;
    readonly snapshotDoneEvent: EventInterface<SnapshotStatistics>;
}

export interface EngineCloudpilot extends EngineCommon {
    type: 'cloudpilot';

    getStatistics(): EmulationStatisticsCloudpilot;

    isSuspended(): boolean;

    getSerialPortSerial(): SerialPort;
    getSerialPortIR(): SerialPort;

    readonly configuredHotsyncNameUpdateEvent: EventInterface<string>;
    readonly pwmUpdateEvent: EventInterface<PwmUpdate>;
}

export interface EngineUarm extends EngineCommon {
    type: 'uarm';

    getStatistics(): EmulationStatisticsUarm;

    getPcmPort(): MessagePort;
    enablePcmStreaming(): void;
    disablePcmStreaming(): void;
}

export type Engine = EngineCloudpilot | EngineUarm;
