#include "FatfsDelegate.h"

FRESULT FatfsDelegate::f_open(FIL* fp, const TCHAR* path, BYTE mode) {
    return ::f_open(fp, path, mode);
}

FRESULT FatfsDelegate::f_close(FIL* fp) { return ::f_close(fp); }

FRESULT FatfsDelegate::f_write(FIL* fp, const void* buff, UINT btw, UINT* bw) {
    return ::f_write(fp, buff, btw, bw);
}

FRESULT FatfsDelegate::f_mkdir(const TCHAR* path) { return ::f_mkdir(path); }

FRESULT FatfsDelegate::f_unlink(const TCHAR* path) { return ::f_unlink(path); }

FRESULT FatfsDelegate::f_stat(const TCHAR* path, FILINFO* fno) { return ::f_stat(path, fno); }

FRESULT FatfsDelegate::f_readdir(DIR* dp, FILINFO* fno) { return ::f_readdir(dp, fno); }
