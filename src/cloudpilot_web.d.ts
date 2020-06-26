import 'emscripten';

import { CloudpilotNative, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    Cloudpilot: { new (): CloudpilotNative };
    getPointer(ptr: VoidPtr): number;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
