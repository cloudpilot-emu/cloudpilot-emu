import { FsckContext } from './web/binding/binding.d';
import 'emscripten';

import { FSTools, VoidPtr, MkfsContext } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends Omit<EmscriptenModule, 'instantiateWasm'> {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    FSTools: { new (): FSTools };
    MkfsContext: { new (): MkfsContext };
    FsckContext: { new (blockCount: number): FsckContext };

    destroy(fsTools: FSTools): void;
    destroy(mkfsContext: MkfsContext): void;
    destroy(fsckContext: FsckContext): void;

    instantiateWasm(
        imports: WebAssembly.Imports,
        successCallback: (instance: WebAssembly.Instance) => void
    ): void | Promise<void>;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
