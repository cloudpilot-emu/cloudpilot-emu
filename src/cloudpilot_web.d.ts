import 'emscripten';

export { CloudpilotNative } from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    Cloudpilot: { new (): CloudpilotNative };
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
