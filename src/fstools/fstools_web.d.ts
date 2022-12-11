import 'emscripten';

import { FSTools, VoidPtr, MkfsContext } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    FSTools: { new (): FSTools };
    MkfsContext: { new (): MkfsContext };

    destroy(fsTools: FSTools): void;
    destroy(mkfsContext: MkfsContext): void;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
