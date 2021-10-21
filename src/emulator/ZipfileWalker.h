#ifndef _ZIPFILE_WALKER_H_
#define _ZIPFILE_WALKER_H_

#include "EmCommon.h"

struct zip_t;

class ZipfileWalker {
   public:
    enum class State : int8 { error = -1, open = 0, done = 1 };

   public:
    ZipfileWalker(size_t bufferSize, uint8* buffer);

    ~ZipfileWalker();

    State GetState() const;
    State Next();

    size_t GetCurrentEntrySize();
    const char* GetCurrentEntryName();
    uint8* GetEntryContent();

   private:
    unique_ptr<char[]> buffer;
    zip_t* zip{nullptr};
    size_t iNextEntry{0};
    ssize_t entriesTotal{0};
    bool done{false};

    uint8* currentEntryContent{nullptr};
};

#endif  // _ZIPFILE_WALKER_H_
