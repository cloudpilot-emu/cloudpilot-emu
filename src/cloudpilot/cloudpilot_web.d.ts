import { DbBackup, SessionImage, SkinLoader } from './web/binding/binding.d';
import 'emscripten';

import { Cloudpilot, RomInfo, VoidPtr } from './web/binding/binding';
import {
    GunzipContext as GunzipContextImpl,
    ZipfileWalker as ZipfileWalkerImpl,
    SessionFile5 as SessionFile5Impl,
    RomInfo5 as RomInfo5Impl,
    ModuleWithGunzipContext,
    ModuleWithZipfileWalker,
    ModuleWithSessionFile5,
    ModuleWithRomInfo5,
} from '../common/web/common';

export * from './web/binding/binding';
export { GunzipState, ZipfileWalkerState } from '../common/web/common';

export type GunzipContext = GunzipContextImpl<VoidPtr>;
export type ZipfileWalker = ZipfileWalkerImpl<VoidPtr>;
export type SessionFile5 = SessionFile5Impl<VoidPtr>;
export type RomInfo5 = RomInfo5Impl<VoidPtr>;

export interface Module
    extends EmscriptenModule,
        ModuleWithGunzipContext<VoidPtr>,
        ModuleWithZipfileWalker<VoidPtr>,
        ModuleWithSessionFile5<VoidPtr>,
        ModuleWithRomInfo5<VoidPtr> {
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString: typeof UTF8ToString;

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
    destroy(sessionFile5: SessionFile5): void;
    destroy(romInfo5: RomInfo5): void;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
