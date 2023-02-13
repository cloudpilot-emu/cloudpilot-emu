#ifndef _DELETE_RECURSIVE_CONTEXT_
#define _DELETE_RECURSIVE_CONTEXT_

#include <cstdint>
#include <string>
#include <vector>

#include "fatfs/ff.h"

class DeleteRecursiveContext {
   public:
    enum class State { initial = 0, more = 1, done = 2, error = -1 };

   public:
    DeleteRecursiveContext(uint32_t timesliceMilliseconds);
    ~DeleteRecursiveContext();

    DeleteRecursiveContext& AddFile(const std::string& path);
    DeleteRecursiveContext& AddDirectory(const std::string& path);

    int Continue();
    int GetState() const;

    const char* GetFailingPath() const;

   private:
    void ExecuteSlice();
    void ExecuteStep();

    void Unlink(const std::string& path);

    FRESULT OpenDir(const std::string& path);
    void CloseDir();

   private:
    State state{State::initial};
    uint32_t timesliceMilliseconds;

    std::vector<std::string> files;
    std::vector<std::string> directories;

    uint32_t directoryIndex{0};

    bool scanning{false};
    bool cleanup{false};

    std::string failingPath;
    DIR dir;
};

#endif  // _DELETE_RECURSIVE_CONTEXT_
