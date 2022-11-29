import 'emscripten';

import { FSTools, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    FSTools: { new (): FSTools };

    destroy(fsTools: FSTools): void;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
