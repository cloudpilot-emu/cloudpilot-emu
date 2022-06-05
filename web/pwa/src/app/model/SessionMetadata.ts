import { DeviceOrientation } from '../../../../common/model/DeviceOrientation';
export interface SessionMetadata {
    name?: string;
    hotsyncName?: string;
    osVersion?: number;
    dontManageHotsyncName?: boolean;
    speed?: number;
    deviceOrientation?: DeviceOrientation;
}
