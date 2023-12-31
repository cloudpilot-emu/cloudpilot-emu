// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

import createModule, {
    FsckContext as FsckNativeContext,
    FsckResult,
    GunzipContext as GunzipContextNative,
    GunzipState,
    GzipContext as GzipContextNative,
    GzipState,
    MkfsContext as MkfsNativeContext,
    CreateZipContext as CreateZipNativeContext,
    Module,
    VoidPtr,
} from '@native-fstools/index';

import { InstantiateFunction, cachedInstantiate } from '@common/helper/wasm';
import { dirtyPagesSize } from './util';
import { decorateZipfileWalker, ZipfileWalker } from './ZipfileWalker';

export { FsckResult } from '@native-fstools/index';

const WASM_BINARY = 'fstools_web.wasm';

const GUNZIP_SLICE_SIZE = 512 * 1024;
const GZIP_SLICE_SIZE = 512 * 1024;
const ZIP_SLICE_SIZE = 512 * 1024;

export class CreateZipContext {
    private constructor(
        private module: Module,
        compressionLevel: number,
    ) {
        this.nativeContext = new module.CreateZipContext();
        if (!this.nativeContext.Initialize(compressionLevel)) {
            throw new Error(`failed to initialize context: ${this.nativeContext.GetLastError()}`);
        }

        const fsTools = new module.FSTools();
        this.chunkBufferPtr = fsTools.Malloc(ZIP_SLICE_SIZE);

        module.destroy(fsTools);
    }

    static async create(compressionLevel: number, instantiateWasm: InstantiateFunction): Promise<CreateZipContext> {
        return new CreateZipContext(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
                instantiateWasm,
            }),
            compressionLevel,
        );
    }

    async addEntry(name: string, content: Uint8Array): Promise<void> {
        if (!this.nativeContext.AddEntry(name)) {
            throw new Error(`failed to add entry: ${this.nativeContext.GetLastError()}`);
        }

        const bufferPtr = this.module.getPointer(this.chunkBufferPtr);

        for (let i = 0; i < content.length; i += ZIP_SLICE_SIZE) {
            if (i > 0) await new Promise((r) => setTimeout(r, 0));

            const len = Math.min(content.length - i, ZIP_SLICE_SIZE);
            const chunkBuffer = this.module.HEAPU8.subarray(bufferPtr, bufferPtr + len);

            chunkBuffer.set(content.subarray(i, i + len));

            if (!this.nativeContext.WriteData(this.chunkBufferPtr, len)) {
                throw new Error(`failed to add ${len} bytes to zip: ${this.nativeContext.GetLastError()}`);
            }
        }
    }

    getContent(): Uint8Array {
        const ptr = this.module.getPointer(this.nativeContext.GetZipData());
        if (!ptr) throw new Error(`unable to obtain zip data: ${this.nativeContext.GetLastError()}`);

        return this.module.HEAPU8.subarray(ptr, ptr + this.nativeContext.GetZipDataSize());
    }

    private nativeContext: CreateZipNativeContext;
    private chunkBufferPtr: VoidPtr;
}

export class MkfsContext {
    private constructor(private module: Module) {
        this.nativeContext = new module.MkfsContext();
    }

    static async create(instantiateWasm: InstantiateFunction): Promise<MkfsContext> {
        return new MkfsContext(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
                instantiateWasm,
            }),
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
    private constructor(
        private module: Module,
        size: number,
    ) {
        this.nativeContext = new module.FsckContext(size >>> 9);
    }

    static async create(size: number, instantiateWasm: InstantiateFunction): Promise<FsckContext> {
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
    private constructor(
        private module: Module,
        gzippedData: Uint8Array,
    ) {
        const fsTools = new module.FSTools();
        const buffer = fsTools.Malloc(gzippedData.length);
        const bufferPtr = module.getPointer(buffer);

        module.HEAPU8.subarray(bufferPtr, bufferPtr + gzippedData.length).set(gzippedData);
        module.destroy(fsTools);

        this.nativeContext = new module.GunzipContext(buffer, gzippedData.length, GUNZIP_SLICE_SIZE);
    }

    static async create(gzippedData: Uint8Array, instantiateWasm: InstantiateFunction): Promise<GunzipContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm: instantiateWasm,
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

export class GzipContext {
    private constructor(
        private module: Module,
        private uncompressedDataSize: number,
    ) {
        const fsTools = new module.FSTools();
        const buffer = fsTools.Malloc(uncompressedDataSize);
        this.uncompressedDataPtr = module.getPointer(buffer);

        module.destroy(fsTools);

        this.nativeContext = new module.GzipContext(buffer, uncompressedDataSize, GZIP_SLICE_SIZE);
    }

    static async create(uncompressedDataSize: number, instantiateWasm: InstantiateFunction): Promise<GzipContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm,
        });

        return new GzipContext(module, uncompressedDataSize);
    }

    getBuffer(): Uint8Array {
        return this.module.HEAPU8.subarray(
            this.uncompressedDataPtr,
            this.uncompressedDataPtr + this.uncompressedDataSize,
        );
    }

    setFilename(name: string): this {
        this.nativeContext.SetFilename(name);

        return this;
    }

    setMtime(mtime: number): this {
        this.nativeContext.SetMtime(mtime);

        return this;
    }

    async gzip(): Promise<boolean> {
        this.nativeContext.Continue();

        while (this.nativeContext.Continue() === GzipState.more) {
            await new Promise((r) => setTimeout(r, 0));
        }

        if (this.nativeContext.GetState() === GzipState.error) {
            console.error(`gzip failed: ${this.nativeContext.GetError()}`);
            return false;
        }

        return true;
    }

    getGzipData(): Uint8Array {
        const uncompressedSize = this.nativeContext.GetGzipSize();
        const buffer = this.nativeContext.GetGzipData();
        const bufferPtr = this.module.getPointer(buffer);

        return this.module.HEAPU8.subarray(bufferPtr, bufferPtr + uncompressedSize);
    }

    private nativeContext: GzipContextNative;
    private uncompressedDataPtr: number;
}

const cachedInstantiateByUrl = new Map<string, InstantiateFunction>();

export class FsTools {
    constructor(wasmModuleUrl = WASM_BINARY) {
        let instantiate = cachedInstantiateByUrl.get(wasmModuleUrl);

        if (!instantiate) {
            instantiate = cachedInstantiate(wasmModuleUrl);
            cachedInstantiateByUrl.set(wasmModuleUrl, instantiate);
        }
        this.instantiante = instantiate;
    }

    async mkfs(size: number): Promise<Uint32Array | undefined> {
        const context = await MkfsContext.create(this.instantiante);

        if (!context.mkfs(size)) return undefined;

        return context.getImage();
    }

    async gunzip(gzippedData: Uint8Array): Promise<Uint8Array | undefined> {
        const context = await GunzipContext.create(gzippedData, this.instantiante);

        if (!(await context.gunzip())) return undefined;

        return context.getDecompressedData();
    }

    createGzipContext(uncompressedDataSize: number): Promise<GzipContext> {
        return GzipContext.create(uncompressedDataSize, this.instantiante);
    }

    createFsckContext(size: number): Promise<FsckContext> {
        return FsckContext.create(size, this.instantiante);
    }

    createZipContext(compressionLevel: number): Promise<CreateZipContext> {
        return CreateZipContext.create(compressionLevel, this.instantiante);
    }

    async createZipfileWalker(zipfile: Uint8Array): Promise<ZipfileWalker> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm: this.instantiante,
        });

        const fstools = new module.FSTools();
        const buffer = fstools.Malloc(zipfile.length);
        const bufferPtr = module.getPointer(buffer);

        module.HEAPU8.subarray(bufferPtr, bufferPtr + zipfile.length).set(zipfile);

        module.destroy(fstools);

        return decorateZipfileWalker(new module.ZipfileWalker(zipfile.length, buffer), module);
    }

    private instantiante: InstantiateFunction;
}
