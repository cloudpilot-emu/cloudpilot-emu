#ifndef _RECURSIVE_FS_ITERATOR_H_
#define _RECURSIVE_FS_ITERATOR_H_

#include <string>
#include <vector>

#include "FatfsDelegate.h"
#include "VfsIterator.h"
#include "fatfs/ff.h"

class RecursiveFsIterator : public VfsIterator {
   public:
    RecursiveFsIterator(FatfsDelegate& fatfsDelegate, const std::string& prefix = "/");
    ~RecursiveFsIterator();

    RecursiveFsIterator& AddFile(const std::string& path);
    RecursiveFsIterator& AddDirectory(const std::string& path);

    State GetState() override;
    State Next() override;
    const std::string& GetCurrentEntry() override;
    bool IsDirectory() override;
    void ReadCurrent(read_callback cb) override;

    std::string GetFullPath();

   private:
    void CloseDir();
    FRESULT OpenDir(const std::string& path);

    std::string AmputatePrefix(const std::string& path);

   private:
    State state{State::initial};
    std::string prefix;

    std::vector<std::string> files;
    std::vector<std::string> directories;

    uint32_t directoryIndex{0};

    bool scanning{false};
    bool cleanup{false};

    std::string currentEntry;
    std::string entryFullPath;
    std::string failingPath;

    DIR dir;
    FILINFO filinfo;

    FatfsDelegate& fatfsDelegate;

   private:
    RecursiveFsIterator(const RecursiveFsIterator&) = delete;
    RecursiveFsIterator(RecursiveFsIterator&&) = delete;
    RecursiveFsIterator& operator=(const RecursiveFsIterator&) = delete;
    RecursiveFsIterator& operator=(RecursiveFsIterator&&) = delete;
};

#endif  // _RECURSIVE_FS_ITERATOR_H_
