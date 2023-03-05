#ifndef _UNZIP_CONTEXT_H_
#define _UNZIP_CONTEXT_H_

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "DeleteRecursiveContext.h"
#include "FatfsDelegate.h"
#include "zip.h"

class UnzipContext {
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

    ~UnzipContext();

    int GetState() const;

    int Continue();
    int ContinueWithOverwrite();

    const char* GetCurrentEntry() const;
    const char* GetCollisionPath() const;
    uint32_t GetEntriesTotal() const;
    uint32_t GetEntriesSuccess() const;

   private:
    void ExecuteSlice();

    void RemoveConflictingFile();
    void ExtractCurrentEntry();
    void MkdirRecursive(std::string path);

   private:
    FatfsDelegate& fatfsDelegate;
    uint32_t timesliceMilliseconds;

    zip_t* zip{nullptr};
    std::string destination;

    uint32_t entriesTotal{0};
    uint32_t entriesSuccess{0};
    uint32_t currentEntryIndex{0};

    State state{State::more};

    std::string currentEntry;
    std::string currentPath;
    std::string collisionPath;

    std::map<std::string, bool> visitedDirectories;

    std::unique_ptr<DeleteRecursiveContext> deleteRecursiveContext;
};

#endif  // _UNZIP_CONTEXT_H_
