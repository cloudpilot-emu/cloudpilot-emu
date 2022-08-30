#ifndef _DB_BACKUP_FALLBACK_H_
#define _DB_BACKUP_FALLBACK_H_

#include <algorithm>

#include "DbBackup.h"
#include "EmCommon.h"

struct PrcHeader;

class DbBackupFallback : public DbBackup {
   public:
    DbBackupFallback() = default;

    bool Init(bool includeRomDatabases) override;

   protected:
    bool DoSave(const DatabaseInfo& dbInfo) override;

   private:
    bool Write8(uint8 value);
    bool Write16(uint16 value);
    bool Write32(uint32 value);
    bool WriteHeader(const PrcHeader& hdr);
    bool WriteChildList(uint16 numChildren);
    bool WriteResourceEntry(uint32 type, uint16 id, uint32 offset);
    bool WriteRecordEntry(uint8 attr, uint32 id, uint32 offset);
    bool WriteChunk(uint16 cardNo, LocalID lid);

   private:
    vector<LocalID> children;
    vector<uint8> chunkTemp;

   private:
    DbBackupFallback(const DbBackupFallback&) = delete;
    DbBackupFallback(DbBackupFallback&&) = delete;
    DbBackupFallback& operator=(const DbBackupFallback&) = delete;
    DbBackupFallback& operator=(DbBackupFallback&&) = delete;
};

#endif  // _DB_BACKUP_FALLBACK_H_
