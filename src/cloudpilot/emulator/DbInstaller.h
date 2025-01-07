#ifndef _DB_INSTALLER_H_
#define _DB_INSTALLER_H_

#include <memory>

#include "EmCommon.h"
#include "EmPalmStructs.h"

class DbInstaller {
   public:
    enum class Result : int {
        failedCouldNotOverwrite = -6,
        failureInternal = -5,
        failureDbIsCorrupt = -4,
        failureDbIsOpen = -3,
        failureNotEnoughMemory = -2,
        failureUnknownReason = -1,
        success = 1,
        needsReboot = 2
    };

   public:
    DbInstaller(size_t bufferSize, uint8* buffer);

    Result Install();

   private:
    void PreprocessDb();

    bool InspectDb();
    void FixupGap();

    template <typename T>
    bool ValidateEntries(const EmAliasDatabaseHdrType<LAS>& header, uint32& offset,
                         uint32& watermark, uint32& firstRecord);

    template <typename T>
    void FixupEntries(const EmAliasDatabaseHdrType<LAS>& header, uint32& offset, int32 delta);

   private:
    size_t bufferSize{0};
    uint8* buffer{nullptr};

    bool isEmptyDb{false};
    bool isResourceDb{false};
    uint32 entrySize;
    uint32 gap{2};

    unique_ptr<uint8[]> preprocessedDb;
};

#endif  // _DB_INSTALLER_H_
