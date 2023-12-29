#ifndef _CREATE_ZIP_CONTEXT_H_
#define _CREATE_ZIP_CONTEXT_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "fatfs/ff.h"

struct zip_t;

class ExportZipContext {
   public:
    enum class State { initial = 0, more = 1, done = 2, errorFile = -1, errorDirectory = -2 };

   public:
    ExportZipContext(const std::string& prefix, uint32_t timesliceMilliseconds);
    ~ExportZipContext();

    ExportZipContext& AddFile(const std::string& path);
    ExportZipContext& AddDirectory(const std::string& path);

    int Continue();
    int GetState() const;

    uint8_t* GetZipContent() const;
    ssize_t GetZipSize();

    const char* GetErrorItem() const;

   private:
    void ExecuteSlice();
    void ExecuteStep();
    void AddFileToArchive(const std::string& name);
    void IncrementalReadCurrentFile();

    FRESULT OpenCurrentDir();
    void CloseCurrentDir();

   private:
    std::string prefix;

    State state{State::initial};
    const uint32_t timesliceMilliseconds;

    std::vector<std::string> files;
    std::vector<std::string> directories;

    uint64_t timesliceStart{0};

    zip_t* zip{nullptr};
    DIR dir;
    FIL file;

    bool scanning{false};
    bool reading{false};

    std::string currentFile;
    std::string currentDirectory;

    uint8_t* archive{nullptr};
    ssize_t archiveSize{0};

    std::unique_ptr<uint8_t[]> readBuffer;

   private:
    ExportZipContext(const ExportZipContext&) = delete;
    ExportZipContext(ExportZipContext&&) = delete;
    ExportZipContext& operator=(const ExportZipContext&) = delete;
    ExportZipContext operator=(ExportZipContext&&) = delete;
};

#endif  // _CREATE_ZIP_CONTEXT_H_
