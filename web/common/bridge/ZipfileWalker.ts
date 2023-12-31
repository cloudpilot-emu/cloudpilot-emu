export { ZipfileWalkerState } from '@native-common/index';
import { ZipfileWalker as ZipfileWalkerNative } from '@native-common/index';

export interface ZipfileWalker extends Omit<ZipfileWalkerNative<unknown>, 'GetCurrentEntryContent'> {
    GetCurrentEntryContent(): Uint8Array | undefined;
}

export interface Module<VoidPtr> {
    getPointer(ptr: VoidPtr): number;

    HEAPU8: Uint8Array;
}

export function decorateZipfileWalker<VoidPtr>(
    nativeWalker: ZipfileWalkerNative<VoidPtr>,
    module: Module<VoidPtr>,
): ZipfileWalker {
    return Object.setPrototypeOf(
        {
            GetCurrentEntryContent: () => {
                const ptr = module.getPointer(nativeWalker.GetCurrentEntryContent());
                const size = nativeWalker.GetCurrentEntrySize();

                return ptr > 0 && size > 0 ? module.HEAPU8.subarray(ptr, ptr + size) : undefined;
            },
        },
        nativeWalker,
    );
}
