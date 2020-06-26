import 'emscripten';

import { CloudpilotNative, RomInfoNative, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    getPointer(ptr: VoidPtr): number;

    Cloudpilot: { new (): CloudpilotNative };
    RomInfo: { new (): RomInfoNative };

    destroy(cloudpilot: CloudpilotNative);
    destroy(romInfo: RomInfoNative);
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
