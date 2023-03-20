import {
    ReaddirContext,
    CreateZipContext,
    DeleteRecursiveContext,
    UnzipContext,
    PasteContext,
} from './web/binding/binding.d';
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
    DeleteRecursiveContext: { new (timesliceMilliseconds: number): DeleteRecursiveContext };
    ReaddirContext: { new (path: string): ReaddirContext };
    UnzipContext: {
        new (timesliceMilliseconds: number, destination: string, data: VoidPtr, size: number): UnzipContext;
    };
    PasteContest: {
        new (timesliceMilliseconds: number, destination: string, prefix: string): PasteContext;
    };

    destroy(vfs: Vfs): void;
    destroy(context: ReaddirContext): void;
    destroy(context: CreateZipContext): void;
    destroy(context: DeleteRecursiveContext): void;
    destroy(context: UnzipContext): void;
    destroy(context: PasteContext): void;

    instantiateWasm(
        imports: WebAssembly.Imports,
        successCallback: (instance: WebAssembly.Instance) => void
    ): void | Promise<void>;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
