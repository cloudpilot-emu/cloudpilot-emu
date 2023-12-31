#ifndef _ZIPFILE_WALKER_H_
#define _ZIPFILE_WALKER_H_

#include <cstddef>
#include <cstdint>
#include <memory>

struct zip_t;

class ZipfileWalker {
   public:
    enum State : int8_t { stateError = -1, stateOpen = 0, stateDone = 1 };

   public:
    ZipfileWalker(size_t bufferSize, void* buffer);

    ~ZipfileWalker();

    State GetState() const;
    State Next();

    uint32_t GetTotalEntries();

    size_t GetCurrentEntrySize();
    const char* GetCurrentEntryName();
    uint8_t* GetCurrentEntryContent();

   private:
    std::unique_ptr<char[]> buffer;
    zip_t* zip{nullptr};
    uint32_t iNextEntry{0};
    int32_t entriesTotal{0};
    bool done{false};

    uint8_t* currentEntryContent{nullptr};
};

#endif  // _ZIPFILE_WALKER_H_
