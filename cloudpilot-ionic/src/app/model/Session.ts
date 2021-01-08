import { Device } from './Device';

export interface Session {
    name: string;
    device: Device;
    ram: number;
    osVersion?: string;
    description?: string;
}
