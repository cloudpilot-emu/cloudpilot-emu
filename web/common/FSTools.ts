// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../node_modules/@types/emscripten/index.d.ts"/>

import createModule, {
    FsckContext as FsckNativeContext,
    FsckResult,
    MkfsContext as MkfsNativeContext,
    Module,
} from '@native-fstools/index';

export { FsckResult } from '@native-fstools/index';

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

export class FsckContext {
    private constructor(private module: Module, size: number) {
        this.nativeContext = new module.FsckContext(size >>> 9);
    }

    static async create(size: number): Promise<FsckContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
        });

        return new FsckContext(module, size);
    }

    getImageData(): Uint32Array {
        const imageSize = this.nativeContext.GetImageSize();
        const imagePtr = this.module.getPointer(this.nativeContext.GetImage());

        return this.module.HEAPU32.subarray(imagePtr >>> 2, (imagePtr + imageSize) >> 2);
    }

    getDirtyPages(): Uint8Array {
        const dirtyPagesPtr = this.module.getPointer(this.nativeContext.GetDirtyPages());

        const imageSize = this.nativeContext.GetImageSize();
        const pages = (imageSize >>> 13) + (imageSize % 8192 > 0 ? 1 : 0);
        const bufferSize = (pages >>> 3) + (pages % 8 > 0 ? 1 : 0);

        return this.module.HEAPU8.subarray(dirtyPagesPtr, dirtyPagesPtr + bufferSize);
    }

    fsck(): FsckResult {
        if (this.result !== FsckResult.pending) return this.result;

        try {
            this.result = this.nativeContext.Fsck();
            return this.result;
        } catch (e) {
            console.warn('fsck failed', e);

            this.result = FsckResult.invalid;
            return this.result;
        }
    }

    getResult(): FsckResult {
        return this.result;
    }

    private nativeContext: FsckNativeContext;
    private result: FsckResult = FsckResult.pending;
}
