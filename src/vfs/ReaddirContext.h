#ifndef _READDIR_CONTEXT_H_
#define _READDIR_CONTEXT_H_

#include <cstdint>
#include <string>

#include "FileEntry.h"
#include "fatfs/ff.h"

class ReaddirContext {
   public:
    enum class Status : uint8_t { more = 0, done = 1, error = 2 };
    enum class Error : uint8_t { none = 0, no_such_directory = 1, unknown = 2 };

   public:
    explicit ReaddirContext(const char* path);
    ~ReaddirContext();

    int Next();

    const char* GetPath() const;
    const FileEntry& GetEntry() const;

    int GetStatus() const;
    int GetError() const;
    const char* GetErrorDescription() const;

   private:
    bool IsFilenameValid() const;

   private:
    const std::string path;

    DIR dir;
    FileEntry fileEntry;

    Status status{Status::error};
    int err{FR_NOT_READY};

   private:
    ReaddirContext(const ReaddirContext&);
    ReaddirContext(ReaddirContext&&);
    ReaddirContext& operator=(const ReaddirContext);
    ReaddirContext& operator=(ReaddirContext&&);
};

#endif  //  _READDIR_CONTEXT_H_
