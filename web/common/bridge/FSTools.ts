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
        gzippedData?: Uint8Array,
    ) {
        if (gzippedData) this.initialize(gzippedData);
    }

    static async create(instantiateWasm: InstantiateFunction, gzippedData?: Uint8Array): Promise<GunzipContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm: instantiateWasm,
        });

        return new GunzipContext(module, gzippedData);
    }

    initialize(gzippedData: Uint8Array): void {
        if (this.nativeContext) this.module.destroy(this.nativeContext);

        const fsTools = new this.module.FSTools();

        if (this.bufferPtr) fsTools.Free(this.bufferPtr);
        this.bufferPtr = fsTools.Malloc(gzippedData.length);

        const buffer = this.module.getPointer(this.bufferPtr);

        this.module.HEAPU8.subarray(buffer, buffer + gzippedData.length).set(gzippedData);
        this.module.destroy(fsTools);

        this.nativeContext = new this.module.GunzipContext(this.bufferPtr, gzippedData.length, GUNZIP_SLICE_SIZE);
    }

    async gunzip(): Promise<boolean> {
        const nativeContext = this.getNativeContext();

        while (nativeContext.Continue() === GunzipState.more) {
            await new Promise((r) => setTimeout(r, 0));
        }

        if (nativeContext.GetState() === GunzipState.error) {
            console.error(`gunzip failed: ${nativeContext.GetError()}`);
            return false;
        }

        return true;
    }

    getDecompressedData(): Uint8Array {
        const nativeContext = this.getNativeContext();

        const size = nativeContext.GetUncompressedSize();
        const buffer = nativeContext.GetUncompressedData();
        const bufferPtr = this.module.getPointer(buffer);

        return this.module.HEAPU8.subarray(bufferPtr, bufferPtr + size);
    }

    private getNativeContext(): GunzipContextNative {
        if (!this.nativeContext) throw new Error('gunzip context not initialized');

        return this.nativeContext;
    }

    private nativeContext: GunzipContextNative | undefined;
    private bufferPtr: VoidPtr | undefined;
}

export class GzipContext {
    private constructor(
        private module: Module,
        uncompressedDataSize?: number,
    ) {
        if (uncompressedDataSize !== undefined) this.initialize(uncompressedDataSize);
    }

    static async create(instantiateWasm: InstantiateFunction, uncompressedDataSize?: number): Promise<GzipContext> {
        const module = await createModule({
            print: (x: string) => console.log(x),
            printErr: (x: string) => console.error(x),
            instantiateWasm,
        });

        return new GzipContext(module, uncompressedDataSize);
    }

    initialize(uncompressedDataSize: number): void {
        if (this.nativeContext) this.module.destroy(this.nativeContext);

        const fsTools = new this.module.FSTools();

        if (this.bufferPtr) fsTools.Free(this.bufferPtr);
        this.bufferPtr = fsTools.Malloc(uncompressedDataSize);
        this.uncompressedDataSize = uncompressedDataSize;

        this.module.destroy(fsTools);

        this.nativeContext = new this.module.GzipContext(this.bufferPtr, uncompressedDataSize, GZIP_SLICE_SIZE);
    }

    getBuffer(): Uint8Array {
        if (!this.bufferPtr || this.uncompressedDataSize === undefined) throw new Error('gzip context not initialized');
        const ptr = this.module.getPointer(this.bufferPtr);

        return this.module.HEAPU8.subarray(ptr, ptr + this.uncompressedDataSize);
    }

    setFilename(name: string): this {
        this.getNativeContext().SetFilename(name);

        return this;
    }

    setMtime(mtime: number): this {
        this.getNativeContext().SetMtime(mtime);

        return this;
    }

    async gzip(): Promise<boolean> {
        const nativeContext = this.getNativeContext();

        nativeContext.Continue();

        while (nativeContext.Continue() === GzipState.more) {
            await new Promise((r) => setTimeout(r, 0));
        }

        if (nativeContext.GetState() === GzipState.error) {
            console.error(`gzip failed: ${nativeContext.GetError()}`);
            return false;
        }

        return true;
    }

    getGzipData(): Uint8Array {
        const nativeContext = this.getNativeContext();

        const uncompressedSize = nativeContext.GetGzipSize();
        const buffer = nativeContext.GetGzipData();
        const bufferPtr = this.module.getPointer(buffer);

        return this.module.HEAPU8.subarray(bufferPtr, bufferPtr + uncompressedSize);
    }

    private getNativeContext(): GzipContextNative {
        if (!this.nativeContext) throw new Error('gzip context not initialized');

        return this.nativeContext;
    }

    private nativeContext: GzipContextNative | undefined;
    private uncompressedDataSize: number | undefined;
    private bufferPtr: VoidPtr | undefined;
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
        const context = await GunzipContext.create(this.instantiante, gzippedData);

        if (!(await context.gunzip())) return undefined;

        return context.getDecompressedData();
    }

    createGunzipContext(gzippedData?: Uint8Array): Promise<GunzipContext> {
        return GunzipContext.create(this.instantiante, gzippedData);
    }

    createGzipContext(uncompressedDataSize?: number): Promise<GzipContext> {
        return GzipContext.create(this.instantiante, uncompressedDataSize);
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
