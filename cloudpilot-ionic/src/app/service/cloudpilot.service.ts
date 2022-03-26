import { Cloudpilot } from '../helper/Cloudpilot';
import { Injectable } from '@angular/core';

@Injectable({ providedIn: 'root' })
export class CloudpilotService {
    readonly cloudpilot = Cloudpilot.create();
}
