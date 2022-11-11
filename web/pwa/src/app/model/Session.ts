import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';

export interface Session {
    id: number;
    rom: string;
    name: string;
    device: DeviceId;
    ram: number;
    osVersion?: number;
    hotsyncName?: string;
    dontManageHotsyncName?: boolean;
    wasResetForcefully?: boolean;
    speed?: number;
    deviceOrientation?: DeviceOrientation;
    mountedCard?: number;
}
