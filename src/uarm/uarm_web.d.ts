import 'emscripten';
import { Bridge, DbBackup, Uarm, VoidPtr } from './web/binding/binding';

export { DeviceType5 } from '../common/web/common';

export * from './web/binding/binding';

export interface Module extends EmscriptenModule {
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString: typeof UTF8ToString;

    Uarm: { new (): Uarm };
    Bridge: { new (): Bridge };

    destroy(uarm: Uarm): void;
    destroy(bridge: Bridge): void;
    destroy(backup: DbBackup): void;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
