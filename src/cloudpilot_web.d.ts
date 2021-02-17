import 'emscripten';

import { Cloudpilot, RomInfo, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    ccall: typeof ccall;
    getPointer(ptr: VoidPtr): number;

    Cloudpilot: { new (): Cloudpilot };
    RomInfo: { new (buffer: VoidPtr, size: number): RomInfo };

    destroy(cloudpilot: Cloudpilot): void;
    destroy(romInfo: RomInfo): void;
}

declare const createModule: EmscriptenModuleFactory<Module>;

export default createModule;
