#ifndef _DELETE_RECURSIVE_CONTEXT_
#define _DELETE_RECURSIVE_CONTEXT_

#include <cstdint>
#include <string>
#include <vector>

#include "RecursiveFsIterator.h"
#include "fatfs/ff.h"

class DeleteRecursiveContext {
   public:
    enum class State { initial = 0, more = 1, done = 2, error = -1 };

   public:
    DeleteRecursiveContext(uint32_t timesliceMilliseconds, FatfsDelegate& fatfsDelegate);
    explicit DeleteRecursiveContext(uint32_t timesliceMilliseconds);

    DeleteRecursiveContext& AddFile(const std::string& path);
    DeleteRecursiveContext& AddDirectory(const std::string& path);

    int Continue();
    int GetState();

    const char* GetFailingPath();

   private:
    void ExecuteSlice();
    void ExecuteStep();

    bool More();

   private:
    uint32_t timesliceMilliseconds;
    RecursiveFsIterator iterator;
    bool failed{false};

   private:
    DeleteRecursiveContext(const DeleteRecursiveContext&) = delete;
    DeleteRecursiveContext(DeleteRecursiveContext&&) = delete;
    DeleteRecursiveContext& operator=(const DeleteRecursiveContext&) = delete;
    DeleteRecursiveContext operator=(DeleteRecursiveContext&&) = delete;
};

#endif  // _DELETE_RECURSIVE_CONTEXT_
