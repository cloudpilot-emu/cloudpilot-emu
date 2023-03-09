#ifndef _GENERIC_COPY_CONTEXT_H_
#define _GENERIC_COPY_CONTEXT_H_

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>

#include "DeleteRecursiveContext.h"
#include "FatfsDelegate.h"
#include "VfsIterator.h"

class GenericCopyContext {
   public:
    enum class State : int {
        more = 0,
        done = 1,
        collision = 2,
        invalidEntry = 3,
        collisionWithDirectory = 4,
        ioError = -1,
        iteratorError = -2,
        cardFull = -3
    };

   public:
    GenericCopyContext(uint32_t timesliceMilliseconds, const char* destination,
                       FatfsDelegate& fatfsDelegate);

    GenericCopyContext(uint32_t timesliceMilliseconds, const char* destination);

    std::string GetCurrentEntry() const;
    std::string GetCollisionPath() const;
    uint32_t GetEntriesSuccess() const;

   protected:
    void Initialize(VfsIterator* iterator);

    State GetState() const;
    State Continue();
    State ContinueWithOverwrite();

   private:
    void ExecuteSlice();

    void RemoveConflictingFile();
    void ExtractCurrentEntry();
    void MkdirRecursive(std::string path);

   private:
    FatfsDelegate& fatfsDelegate;
    uint32_t timesliceMilliseconds;

    VfsIterator* iterator{nullptr};
    std::string destination;

    uint32_t entriesSuccess{0};

    State state{State::more};

    std::string currentEntry;
    std::string currentPath;
    std::string collisionPath;

    std::map<std::string, bool> visitedDirectories;

    std::unique_ptr<DeleteRecursiveContext> deleteRecursiveContext;
};

#endif  // _GENERIC_COPY_CONTEXT_H_
