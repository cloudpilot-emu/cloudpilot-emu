#ifndef _AUTOCLOSE_FILE_H_
#define _AUTOCLOSE_FILE_H_

#include "fatfs/ff.h"

class AutocloseFile {
   public:
    AutocloseFile(FIL* file) : file(file) {}
    ~AutocloseFile() { f_close(file); }

   private:
    FIL* file;

   private:
    AutocloseFile(const AutocloseFile&) = delete;
    AutocloseFile(AutocloseFile&&) = delete;
    AutocloseFile& operator=(const AutocloseFile&) = delete;
    AutocloseFile& operator==(AutocloseFile&&) = delete;
};

#endif  //  _AUTOCLOSE_FILE_H_
