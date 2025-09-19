import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';

export interface Session {
    id: number;
    rom: string;
    name: string;
    device: DeviceId;
    ram: number;
    osVersion?: number;
    wasResetForcefully?: boolean;
    deviceOrientation?: DeviceOrientation;
    mountedCard?: number;
    lastLaunch?: number;

    // only OS4
    hotsyncName?: string;
    dontManageHotsyncName?: boolean;
    speed?: number;

    // only uarm
    nand?: number;
    targetMips?: number;
    warnMips?: number;
}
