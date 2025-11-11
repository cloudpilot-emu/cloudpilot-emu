// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>
import { Bridge, Module, Uarm as UarmNative, VoidPtr, default as createModule } from '@native-uarm/index';
import { PalmButton } from '@native/cloudpilot_web';
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

export function mapButton(button: PalmButton): number {
    switch (button) {
        case PalmButton.cal:
            return 1;

        case PalmButton.phone:
            return 2;

        case PalmButton.todo:
            return 3;

        case PalmButton.notes:
            return 4;

        case PalmButton.up:
            return 5;

        case PalmButton.down:
            return 6;

        default:
            return -1;
    }
}

export class Uarm {
    private constructor(private module: Module) {
        this.uarm = new module.Uarm();
        this.bridge = new module.Bridge();
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
    setRamSize(ramSize: number): void {
        this.uarm.SetRamSize(ramSize);
    }

    @guard()
    setMemory(data: Uint8Array): void {
        this.uarm.SetMemory(data.length, this.copyIn(data));
    }

    @guard()
    setNand(data: Uint8Array): void {
        this.uarm.SetNand(data.length, this.copyIn(data));
    }

    @guard()
    setSd(data: Uint8Array, key: string): void {
        this.uarm.SetSd(data.length, this.copyIn(data), key);
    }

    @guard()
    setSavestate(data: Uint8Array): void {
        this.uarm.SetSavestate(data.length, this.copyIn(data));
    }

    @guard()
    launch(nor: Uint8Array): boolean {
        return this.uarm.Launch(nor.length, this.copyIn(nor));
    }

    @guard()
    buttonUp(button: PalmButton): void {
        const key = mapButton(button);
        if (key >= 0) this.uarm.KeyUp(key);
    }

    @guard()
    buttonDown(button: PalmButton): void {
        const key = mapButton(button);
        if (key >= 0) this.uarm.KeyDown(key);
    }

    @guard()
    penDown(x: number, y: number): void {
        this.uarm.PenDown(x, y);
    }

    @guard()
    penUp(): void {
        this.uarm.PenUp();
    }

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

    private copyIn(data: Uint8Array): VoidPtr {
        const ptr = this.bridge.Malloc(data.length);
        const offset = this.module.getPointer(ptr);

        this.module.HEAPU8.subarray(offset, offset + data.length).set(data);

        return ptr;
    }

    private uarm: UarmNative;
    private bridge: Bridge;

    fatalErrorEvent = new Event<Error>();
    private amIdead = false;
}
