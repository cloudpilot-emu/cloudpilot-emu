// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// tslint:disable-next-line: no-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

import createModule, {
    FsckContext as FsckNativeContext,
    FsckResult,
    GunzipContext as GunzipContextNative,
    GunzipState,
    MkfsContext as MkfsNativeContext,
    Module,
} from '@native-fstools/index';

import { cachedInstantiate } from '@common/helper/wasm';
import { dirtyPagesSize } from './util';

export { FsckResult } from '@native-fstools/index';

const WASM_BINARY = 'fstools_web.wasm';
const instantiateWasm = cachedInstantiate(WASM_BINARY);

const GUNZIP_SLICE_SIZE = 512 * 1024;

export async function mkfs(size: number): Promise<Uint32Array | undefined> {
    const context = await MkfsContext.create();

    if (!context.mkfs(size)) return undefined;

    return context.getImage();
}

export async function gunzip(gzippedData: Uint8Array): Promise<Uint8Array | undefined> {
    const context = await GunzipContext.create(gzippedData);

    if (!(await context.gunzip())) return undefined;

    return context.getDecompressedData();
}

export class MkfsContext {
    private constructor(private module: Module) {
        this.nativeContext = new module.MkfsContext();
    }

    static async create(): Promise<MkfsContext> {
        return new MkfsContext(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
                instantiateWasm,
            })
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
            instantiateWasm,
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
        const bufferSize = dirtyPagesSize(this.nativeContext.GetImageSize());

        return this.module.HEAPU8.subarray(dirtyPagesPtr, dirtyPagesPtr + bufferSize);
    }

    fsck(): FsckResult {
        if (this.result !== FsckResult.pending) return this.result;

        try {
            this.result = this.nativeContext.Fsck();
            return this.result;
        } catch (e) {
            console.warn('fsck failed', e);

            this.result = FsckResult.unfixable;
            return this.result;
        }
    }

    getResult(): FsckResult {
        return this.result;
    }

    private nativeContext: FsckNativeContext;
    private result: FsckResult = FsckResult.pending;
}

export class GunzipContext {
    private constructor(private module: Module, gzippedData: Uint8Array) {
        const fsTools = new module.FSTools();
        const buffer = fsTools.Malloc(gzippedData.length);
        const bufferPtr = module.getPointer(buffer);

        module.HEAPU8.subarray(bufferPtr, bufferPtr + gzippedData.length).set(gzippedData);
        module.destroy(fsTools);

        this.nativeContext = new module.GunzipContext(buffer, gzippedData.length, GUNZIP_SLICE_SIZE);
    }

    static async create(gzippedData: Uint8Array): Promise<GunzipContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm,
        });

        return new GunzipContext(module, gzippedData);
    }

    async gunzip(): Promise<boolean> {
        while (this.nativeContext.Continue() === GunzipState.more) {
            await new Promise((r) => setTimeout(r, 0));
        }

        if (this.nativeContext.GetState() === GunzipState.error) {
            console.error(`gunzip failed: ${this.nativeContext.GetError()}`);
            return false;
        }

        return true;
    }

    getDecompressedData(): Uint8Array {
        const uncompressedSize = this.nativeContext.GetUncompressedSize();
        const buffer = this.nativeContext.GetUncompressedData();
        const bufferPtr = this.module.getPointer(buffer);

        return this.module.HEAPU8.subarray(bufferPtr, bufferPtr + uncompressedSize);
    }

    private nativeContext: GunzipContextNative;
}
