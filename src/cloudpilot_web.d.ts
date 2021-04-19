import { DbBackup } from './web/binding/binding.d';
import 'emscripten';

import { Cloudpilot, RomInfo, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;

    Cloudpilot: { new (): Cloudpilot };
    RomInfo: { new (buffer: VoidPtr, size: number): RomInfo };

    destroy(cloudpilot: Cloudpilot): void;
    destroy(romInfo: RomInfo): void;
    destroy(dbBackup: DbBackup): void;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
