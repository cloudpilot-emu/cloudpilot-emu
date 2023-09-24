import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { Injectable } from '@angular/core';
import wasmModule from '@native/cloudpilot_web.wasm';

@Injectable({ providedIn: 'root' })
export class CloudpilotService {
    readonly cloudpilot = Cloudpilot.create(wasmModule);
}
