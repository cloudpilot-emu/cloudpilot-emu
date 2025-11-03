import { Inject, Injectable } from '@angular/core';
import { Cloudpilot, SessionImage } from '@common/bridge/Cloudpilot';
import { ramSize } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';

import { CLOUDPILOT_INSTANCE_TOKEN } from './token';

@Injectable({ providedIn: 'root' })
export class NativeSupportService {
    constructor(@Inject(CLOUDPILOT_INSTANCE_TOKEN) private cloudpilot: Promise<Cloudpilot>) {}

    async ramSizeForDevice(device: DeviceId): Promise<number> {
        return ramSize(device) ?? (await this.cloudpilot).minRamForDevice(device);
    }

    serializeSessionImage<T>(sessionImage: Omit<SessionImage<T>, 'version'>): Promise<Uint8Array | undefined> {
        return this.cloudpilot.then((cp) => cp.serializeSessionImage(sessionImage));
    }

    deserializeSessionImage<T>(buffer: Uint8Array | undefined): Promise<SessionImage<T> | undefined> {
        return this.cloudpilot.then((cp) => cp.deserializeSessionImage(buffer));
    }
}
