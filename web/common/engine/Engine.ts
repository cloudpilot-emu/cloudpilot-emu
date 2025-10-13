import { PwmUpdate } from '@common/bridge/Cloudpilot';
import { DeviceId } from '@common/model/DeviceId';
import { EmulationStatisticsCloudpilot, EmulationStatisticsUarm } from '@common/model/EmulationStatistics';
import { EngineType } from '@common/model/EngineType';
import { SerialPort } from '@common/serial/SerialPort';
import { PalmButton } from '@native/cloudpilot_web';
import { EventInterface } from 'microevent.ts';

import { EngineSettings } from './EngineSettings';
import { SnapshotContainer } from './Snapshot';

export interface StorageCardProvider {
    load(engine: Engine): Promise<void>;
}

export interface EngineCommon {
    readonly type: EngineType;

    penDown(x: number, y: number): void;
    penUp(): void;

    buttonDown(button: PalmButton): void;
    buttonUp(button: PalmButton): void;

    keyStroke(key: number, ctrl?: boolean): void;

    reset(): Promise<void>;
    resetNoExtensions(): Promise<void>;
    resetHard(): Promise<void>;

    isRunning(): boolean;
    isPowerOff(): boolean;
    isUIInitialized(): boolean;
    isSlowdown(): boolean;

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

    blitFrame(canvas: HTMLCanvasElement): void;

    updateSettings(settings: EngineSettings): void;

    shutdown(): void;

    allocateCard(id: string, size: number): Uint32Array;
    releaseCard(id: string): void;
    insertCard(id: string): Promise<void>;

    readonly newFrameEvent: EventInterface<void>;
    readonly timesliceEvent: EventInterface<number>;
    readonly suspendEvent: EventInterface<void>;
    readonly snapshotEvent: EventInterface<SnapshotContainer>;
    readonly palmosStateChangeEvent: EventInterface<void>;
    readonly fatalError: EventInterface<Error>;
}

export interface EngineCloudpilot extends EngineCommon {
    type: 'cloudpilot';

    getStatistics(): EmulationStatisticsCloudpilot;

    isSuspended(): boolean;
    getOSVersion(): number;

    getSerialPortSerial(): SerialPort;
    getSerialPortIR(): SerialPort;

    readonly configuredHotsyncNameUpdateEvent: EventInterface<string>;
    readonly pwmUpdateEvent: EventInterface<PwmUpdate>;
}

export interface EngineUarm extends EngineCommon {
    type: 'uarm';

    getStatistics(): EmulationStatisticsUarm;
    getPcmPort(): MessagePort;
}

export type Engine = EngineCloudpilot | EngineUarm;
