import { VoidPtr } from './../../vfs/web/binding/binding.d';
export const enum GunzipState {
    more = 0,
    done = 1,
    error = -1,
}

interface GunzipContext<VoidPtr> {
    GetState(): GunzipState;
    Continue(): GunzipState;

    GetUncompressedData(): VoidPtr;
    ReleaseUncompressedData(): VoidPtr;
    GetUncompressedSize(): number;

    GetError(): string;
}

interface ModuleWithGunzipContext<VoidPtr> {
    GunzipContext: {
        new (data: VoidPtr, size: number, slizeSize: number): GunzipContext<VoidPtr>;
    };
}
