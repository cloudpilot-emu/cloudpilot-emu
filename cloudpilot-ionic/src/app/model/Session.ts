import { DeviceId } from './DeviceId';

export interface Session {
    id: string;
    name: string;
    device: DeviceId;
    ram: number;
    osVersion?: string;
}
