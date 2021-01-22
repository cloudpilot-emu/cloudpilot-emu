import { Device } from './Device';

export interface Session {
    id: string;
    name: string;
    device: Device;
    ram: number;
    osVersion?: string;
    description?: string;
}
