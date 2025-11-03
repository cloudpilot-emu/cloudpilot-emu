import { Inject, Injectable } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';

import { TOKEN_CLOUDPILOT_INSTANCE } from './token';

@Injectable({ providedIn: 'root' })
export class CloudpilotService {
    constructor(@Inject(TOKEN_CLOUDPILOT_INSTANCE) public readonly cloudpilot: Promise<Cloudpilot>) {}
}
