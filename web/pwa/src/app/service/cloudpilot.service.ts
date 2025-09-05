import { Injectable } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import wasmModule from '@native/cloudpilot_web.wasm';

declare global {
    interface Window {
        __cp_enableLogging?: (logging: boolean) => void;
    }
}

@Injectable({ providedIn: 'root' })
export class CloudpilotService {
    constructor() {
        void this.cloudpilot.then((instance) => (this.cloudpilotInstance = instance));
        window.__cp_enableLogging = (logging) => this.cloudpilotInstance?.enableLogging(logging);
    }

    readonly cloudpilot = Cloudpilot.create(wasmModule);

    private cloudpilotInstance?: Cloudpilot;
}
