import { DbBackup, SessionImage, SkinLoader } from './web/binding/binding.d';
import 'emscripten';

import { Cloudpilot, RomInfo, VoidPtr } from './web/binding/binding';
import {
    GunzipContext as GunzipContextImpl,
    ZipfileWalker as ZipfileWalkerImpl,
    ModuleWithGunzipContext,
    ModuleWithZipfileWalker,
    SessionFile5Impl,
    ModuleWithSessionFile5Impl,
} from '../common/web/common';

export * from './web/binding/binding';
export { GunzipState, ZipfileWalkerState } from '../common/web/common';

export type GunzipContext = GunzipContextImpl<VoidPtr>;
export type ZipfileWalker = ZipfileWalkerImpl<VoidPtr>;
export type SessionFile5 = SessionFile5Impl<VoidPtr>;

export interface Module
    extends Omit<EmscriptenModule, 'instantiateWasm'>,
        ModuleWithGunzipContext<VoidPtr>,
        ModuleWithZipfileWalker<VoidPtr>,
        ModuleWithSessionFile5Impl<VoidPtr> {
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    Cloudpilot: { new (): Cloudpilot };
    RomInfo: { new (buffer: VoidPtr, size: number): RomInfo };
    SessionImage: { new (): SessionImage };
    SkinLoader: { new (name: string): SkinLoader };

    destroy(cloudpilot: Cloudpilot): void;
    destroy(romInfo: RomInfo): void;
    destroy(dbBackup: DbBackup): void;
    destroy(zipfileWalker: ZipfileWalker): void;
    destroy(sessionImage: SessionImage): void;
    destroy(skinLoader: SkinLoader): void;
    destroy(gunzipContext: GunzipContext): void;
    destroy(session_file5: SessionFile5): void;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
