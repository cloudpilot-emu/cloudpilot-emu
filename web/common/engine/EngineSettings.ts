import { SchedulerKind } from '@common/helper/scheduler';

export interface EngineSettings {
    automaticSnapshotInterval: number;

    speed: number;
    manageHotsyncName: boolean;
    hotsyncName: string;
    schedulerKind: SchedulerKind;

    clipboardIntegration: boolean;
    networkIntegration: boolean;

    disableAudio: boolean;
    targetMips: number;
    warnSlowdownThreshold: number;
    maxHostLoad: number;
}
