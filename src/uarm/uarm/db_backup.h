#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

#include <sys/types.h>

#include <cstddef>
#include <cstdint>
#include <memory>

#include "db_list.h"

struct SyscallDispatch;
struct zip_t;

class DbBackup {
   public:
    enum class BackupType { ram = 0, ramRom = 1, everything = 2 };
    enum class State { created = 0, inProgress = 1, done = 2, error = -1 };

   public:
    DbBackup(SyscallDispatch* syscallDispatch, BackupType backupType);
    ~DbBackup();

    bool Init();
    State GetState() const;
    bool Continue();
    const char* GetLastProcessedDb();

    const void* GetArchiveData();
    size_t GetArchiveSize();

   private:
    bool Include(DbMetadata& db);
    void CopyArchiveData();

   private:
   private:
    SyscallDispatch* sd{nullptr};

    BackupType backupType{BackupType::ram};
    State state{State::created};

    zip_t* zip{nullptr};
    void* archiveData{nullptr};
    ssize_t archiveSize{0};

    DbMetadataList dbList;
    DbMetadataList::iterator dbIterator;

    std::unique_ptr<uint8_t[]> chunkBuffer;
    char lastProcessedDb[32];

   private:
    DbBackup(const DbBackup&) = delete;
    DbBackup(DbBackup&&) = delete;
    DbBackup& operator=(const DbBackup&) = delete;
    DbBackup& operator=(DbBackup&&) = delete;
};

#endif  // _DB_BACKUP_H_