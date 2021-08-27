import { DeviceId } from './DeviceId';

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
}
