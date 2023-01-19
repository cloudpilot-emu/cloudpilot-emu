#ifndef _ZIPFILE_WALKER_H_
#define _ZIPFILE_WALKER_H_

#include <memory>

#include "EmCommon.h"

struct zip_t;

class ZipfileWalker {
   public:
    enum State : int8 { stateError = -1, stateOpen = 0, stateDone = 1 };

   public:
    ZipfileWalker(size_t bufferSize, void* buffer);

    ~ZipfileWalker();

    State GetState() const;
    State Next();

    size_t GetCurrentEntrySize();
    const char* GetCurrentEntryName();
    uint8* GetCurrentEntryContent();

   private:
    unique_ptr<char[]> buffer;
    zip_t* zip{nullptr};
    size_t iNextEntry{0};
    ssize_t entriesTotal{0};
    bool done{false};

    uint8* currentEntryContent{nullptr};
};

#endif  // _ZIPFILE_WALKER_H_
