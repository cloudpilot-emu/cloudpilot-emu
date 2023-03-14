#ifndef FATFS_DELEGATE
#define FATFS_DELEGATE

#include "fatfs/ff.h"

class FatfsDelegate {
   public:
    FatfsDelegate() = default;

    virtual FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode);
    virtual FRESULT f_close(FIL* fp);
    virtual FRESULT f_write(FIL* fp, const void* buff, UINT btw, UINT* bw);
    virtual FRESULT f_mkdir(const TCHAR* path);
    virtual FRESULT f_unlink(const TCHAR* path);
    virtual FRESULT f_stat(const TCHAR* path, FILINFO* fno);
    virtual FRESULT f_readdir(DIR* dp, FILINFO* fno);
    virtual FRESULT f_closedir(DIR* dp);
    virtual FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br);
    virtual FRESULT f_opendir(DIR* dp, const TCHAR* path);
};

#endif  // FATFS_DELEGATE
