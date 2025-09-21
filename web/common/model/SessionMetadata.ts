import { DeviceOrientation } from '@common/model/DeviceOrientation';

export interface SessionMetadata {
    name?: string;
    osVersion?: number;
    deviceOrientation?: DeviceOrientation;

    // cloudpilot only
    dontManageHotsyncName?: boolean;
    speed?: number;
    hotsyncName?: string;

    // uarm only
    disableAudio?: boolean;
    targetMips?: number;
    warnSlowdownThreshold?: number;
    maxHostLoad?: number;
}
