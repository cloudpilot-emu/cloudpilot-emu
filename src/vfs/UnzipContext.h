#ifndef _UNZIP_CONTEXT_H_
#define _UNZIP_CONTEXT_H_

#include <cstddef>
#include <cstdint>

#include "FatfsDelegate.h"
#include "GenericCopyContext.h"
#include "ZipfileIterator.h"

class UnzipContext : public GenericCopyContext {
   public:
    enum class State : int {
        more = 0,
        done = 1,
        collision = 2,
        invalidEntry = 3,
        collisionWithDirectory = 4,
        ioError = -1,
        zipfileError = -2,
        cardFull = -3
    };

   public:
    UnzipContext(uint32_t timesliceMilliseconds, const char* destination, void* data, size_t size,
                 FatfsDelegate& fatfsDelegate);

    UnzipContext(uint32_t timesliceMilliseconds, const char* destination, void* data, size_t size);

    int GetState() const;
    int Continue();
    int ContinueWithOverwrite();

    uint32_t GetEntriesTotal() const;

   private:
    ZipfileIterator iterator;
};

#endif  // _UNZIP_CONTEXT_H_
