import {
    ReaddirContext,
    ExportZipContext,
    DeleteRecursiveContext,
    UnzipContext,
    PasteContext,
} from './web/binding/binding.d';
import { Vfs, VoidPtr } from './web/binding/binding';
import 'emscripten';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString: typeof UTF8ToString;

    Vfs: { new (): Vfs };
    ExportZipContext: { new (prefix: string, timesliceMilliseconds: number): ExportZipContext };
    DeleteRecursiveContext: { new (timesliceMilliseconds: number): DeleteRecursiveContext };
    ReaddirContext: { new (path: string): ReaddirContext };
    UnzipContext: {
        new (timesliceMilliseconds: number, destination: string, data: VoidPtr, size: number): UnzipContext;
    };
    PasteContext: {
        new (timesliceMilliseconds: number, destination: string, prefix: string): PasteContext;
    };

    destroy(vfs: Vfs): void;
    destroy(context: ReaddirContext): void;
    destroy(context: ExportZipContext): void;
    destroy(context: DeleteRecursiveContext): void;
    destroy(context: UnzipContext): void;
    destroy(context: PasteContext): void;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
