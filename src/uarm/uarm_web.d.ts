import 'emscripten';
import { Bridge, Uarm, VoidPtr } from './web/binding/binding';

export * from './web/binding/binding';

export interface Module extends Omit<EmscriptenModule, 'instantiateWasm'> {
    addFunction: typeof addFunction;
    getPointer(ptr: VoidPtr): number;
    UTF8ToString: typeof UTF8ToString;

    Uarm: { new (): Uarm };
    Bridge: { new (): Bridge };

    destroy(uarm: Uarm): void;
    destroy(bridge: Bridge): void;
}

declare const createModule: (moduleOverrides: Partial<Module>) => Promise<Module>;

export default createModule;
