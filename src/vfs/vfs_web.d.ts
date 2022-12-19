import { Vfs, VoidPtr } from './web/binding/binding';
import 'emscripten';

export * from './web/binding/binding';

export interface Module extends Omit<EmscriptenModule, 'instantiateWasm'> {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    Vfs: { new (): Vfs };

    destroy(vfs: Vfs): void;

    instantiateWasm(
        imports: WebAssembly.Imports,
        successCallback: (instance: WebAssembly.Instance) => void
    ): void | Promise<void>;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
