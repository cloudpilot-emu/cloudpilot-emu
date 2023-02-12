import { ReaddirContext, CreateZipContext } from './web/binding/binding.d';
import { Vfs, VoidPtr } from './web/binding/binding';
import 'emscripten';

export * from './web/binding/binding';

export interface Module extends Omit<EmscriptenModule, 'instantiateWasm'> {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    Vfs: { new (): Vfs };
    CreateZipContext: { new (prefix: string, timesliceMilliseconds: number): CreateZipContext };
    ReaddirContext: { new (path: string): ReaddirContext };

    destroy(vfs: Vfs): void;
    destroy(context: ReaddirContext): void;
    destroy(context: CreateZipContext): void;

    instantiateWasm(
        imports: WebAssembly.Imports,
        successCallback: (instance: WebAssembly.Instance) => void
    ): void | Promise<void>;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
