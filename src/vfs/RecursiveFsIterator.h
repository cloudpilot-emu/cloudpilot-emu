#ifndef _RECURSIVE_FS_ITERATOR_H_
#define _RECURSIVE_FS_ITERATOR_H_

#include <functional>
#include <string>
#include <vector>

#include "FatfsDelegate.h"
#include "VfsIterator.h"
#include "fatfs/ff.h"

class RecursiveFsIterator : public VfsIterator {
   public:
    using skipDirectoryCb = std::function<bool(const std::string&)>;

   public:
    RecursiveFsIterator(FatfsDelegate& fatfsDelegate, const std::string& prefix = "/");
    ~RecursiveFsIterator();

    RecursiveFsIterator& AddFile(const std::string& path);
    RecursiveFsIterator& SetSkipDirectory(skipDirectoryCb skipDirectory);

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
    skipDirectoryCb skipDirectory;

   private:
    RecursiveFsIterator(const RecursiveFsIterator&) = delete;
    RecursiveFsIterator(RecursiveFsIterator&&) = delete;
    RecursiveFsIterator& operator=(const RecursiveFsIterator&) = delete;
    RecursiveFsIterator& operator=(RecursiveFsIterator&&) = delete;
};

#endif  // _RECURSIVE_FS_ITERATOR_H_
