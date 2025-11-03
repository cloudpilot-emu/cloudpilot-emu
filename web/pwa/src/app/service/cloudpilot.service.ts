import { Inject, Injectable } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';

import { CLOUDPILOT_INSTANCE_TOKEN } from './token';

@Injectable({ providedIn: 'root' })
export class CloudpilotService {
    constructor(@Inject(CLOUDPILOT_INSTANCE_TOKEN) public readonly cloudpilot: Promise<Cloudpilot>) {}
}
