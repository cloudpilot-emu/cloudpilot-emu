import { DbBackup, ZipfileWalker, SessionImage, SkinLoader } from './web/binding/binding.d';
import 'emscripten';

import { Cloudpilot, RomInfo, VoidPtr } from './web/binding/binding';
import { GunzipContext as GunzipContextImpl, ModuleWithGunzipContext } from '../common/web/common';

export * from './web/binding/binding';
export { GunzipState } from '../common/web/common';

export type GunzipContext = GunzipContextImpl<VoidPtr>;

export interface Module extends EmscriptenModule, ModuleWithGunzipContext<VoidPtr> {
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString(charPtr: number): string;

    Cloudpilot: { new (): Cloudpilot };
    RomInfo: { new (buffer: VoidPtr, size: number): RomInfo };
    ZipfileWalker: {
        new (bufferSize: number, buffer: VoidPtr): ZipfileWalker;
    };
    SessionImage: { new (): SessionImage };
    SkinLoader: { new (name: string): SkinLoader };

    destroy(cloudpilot: Cloudpilot): void;
    destroy(romInfo: RomInfo): void;
    destroy(dbBackup: DbBackup): void;
    destroy(zipfileWalker: ZipfileWalker): void;
    destroy(sessionImage: SessionImage): void;
    destroy(skinLoader: SkinLoader): void;
    destroy(gunzipContext: GunzipContext): void;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
