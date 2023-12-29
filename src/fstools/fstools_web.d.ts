import { FsckContext } from './web/binding/binding.d';
import 'emscripten';

import { FSTools, VoidPtr, MkfsContext } from './web/binding/binding';
import {
    GunzipContext as GunzipContextImpl,
    GzipContext as GzipContextImpl,
    CreateZipContext as CreateZipContextImpl,
    ModuleWithGunzipContext,
    ModuleWithGzipContext,
    ModuleWithCreateZipContext,
} from '../common/web/common';

export * from './web/binding/binding';
export { GunzipState, GzipState } from '../common/web/common';

export type GunzipContext = GunzipContextImpl<VoidPtr>;
export type GzipContext = GzipContextImpl<VoidPtr>;
export type CreateZipContext = CreateZipContextImpl<VoidPtr>;

export interface Module
    extends Omit<EmscriptenModule, 'instantiateWasm'>,
        ModuleWithGunzipContext<VoidPtr>,
        ModuleWithGzipContext<VoidPtr>,
        ModuleWithCreateZipContext<VoidPtr> {
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
    destroy(gunzipContext: GunzipContext): void;
    destroy(gzipContext: GzipContext): void;

    instantiateWasm(
        imports: WebAssembly.Imports,
        successCallback: (instance: WebAssembly.Instance) => void
    ): void | Promise<void>;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
