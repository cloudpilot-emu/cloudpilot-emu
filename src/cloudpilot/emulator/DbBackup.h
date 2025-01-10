#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

#include <memory>

#include "DbExporter.h"
#include "EmCommon.h"
#include "Miscellaneous.h"

struct zip_t;

class DbBackup {
   public:
    DbBackup() = default;
    ~DbBackup();

    bool Init(bool includeRomDatabases);

    bool IsInProgress() const;
    bool IsDone() const;

    const char* GetCurrentDatabase();

    bool Save();
    void Skip();

    pair<ssize_t, uint8*> GetArchive();
    uint8* GetArchivePtr();
    ssize_t GetArchiveSize();

   private:
    enum class State { created, inProgress, done };

   private:
    State state{State::created};
    zip_t* zip{nullptr};

    DatabaseInfoList databases;

    DatabaseInfoList::iterator currentDb;

    uint8* archive{nullptr};
    ssize_t archiveSize{0};

    string currentDatabase;

    unique_ptr<DbExporter> exporter;

   private:
    DbBackup(const DbBackup&) = delete;
    DbBackup(DbBackup&&) = delete;
    DbBackup& operator=(const DbBackup&) = delete;
    DbBackup& operator=(DbBackup&&) = delete;
};

#endif  //  _DB_BACKUP_H_
