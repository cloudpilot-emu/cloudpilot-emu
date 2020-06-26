import 'emscripten';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
}

const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
