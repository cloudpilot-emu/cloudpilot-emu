// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../node_modules/@types/emscripten/index.d.ts"/>

import createModule, { MkfsContext as MkfsNativeContext, Module } from '@native-fstools/index';

export async function mkfs(size: number): Promise<Uint32Array | undefined> {
    const context = await MkfsContext.create();

    if (!context.mkfs(size)) return undefined;

    return context.getImage();
}

export class MkfsContext {
    private constructor(private module: Module) {
        this.nativeContext = new module.MkfsContext();
    }

    static async create(): Promise<MkfsContext> {
        return new MkfsContext(
            await createModule({ print: (x: string) => console.log(x), printErr: (x: string) => console.error(x) })
        );
    }

    mkfs(size: number): boolean {
        return this.nativeContext.Mkfs(size, 'card');
    }

    getImage(): Uint32Array | undefined {
        const imageSize = this.nativeContext.GetImageSize();
        const imagePtr = this.module.getPointer(this.nativeContext.GetImage());
        if (imageSize === 0 || imagePtr === 0) return undefined;

        return this.module.HEAPU32.subarray(imagePtr >>> 2, (imagePtr + imageSize) >>> 2);
    }

    private nativeContext: MkfsNativeContext;
}
