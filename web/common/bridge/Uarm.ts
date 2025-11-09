// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>
import { Module, Uarm as UarmNative, default as createModule } from '@native-uarm/index';
import { Event } from 'microevent.ts';

let nextId = 0;

function guard(): MethodDecorator {
    return (target: unknown, propertyKey: string | symbol, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;

        desc.value = function (this: Uarm, p1: unknown, p2: unknown) {
            return this.guard(() => oldMethod.call(this, p1, p2));
        };

        return desc;
    };
}

export class Uarm {
    private constructor(private module: Module) {
        this.uarm = new module.Uarm();
    }

    static async create(wasmModule: WebAssembly.Module): Promise<Uarm> {
        const id = nextId++;

        return new Uarm(
            await createModule({
                print: (x: string) => console.log(`${id}: ${x}`),
                printErr: (x: string) => console.error(`${id}: ${x}`),
                instantiateWasm: (imports, callback) =>
                    void WebAssembly.instantiate(wasmModule, imports).then((instance) => callback(instance)),
            }),
        );
    }

    @guard()
    foo(): void {}

    guard<T>(fn: () => T) {
        if (this.amIdead) throw new Error('uarm instance is dead');

        try {
            return fn();
        } catch (e) {
            this.amIdead = true;
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            this.uarm = undefined as any;

            this.fatalErrorEvent.dispatch(e instanceof Error ? e : new Error('unknown error'));

            throw e;
        }
    }

    private uarm: UarmNative;

    fatalErrorEvent = new Event<Error>();
    private amIdead = false;
}
