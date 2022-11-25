#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

#include <memory>

#include "EmCommon.h"
#include "Miscellaneous.h"

struct zip_t;

class DbBackup {
   public:
    static unique_ptr<DbBackup> create();

    virtual ~DbBackup();

    virtual bool Init(bool includeRomDatabases);

    bool IsInProgress() const;
    bool IsDone() const;

    const char* GetCurrentDatabase();

    bool Save();
    void Skip();

    pair<ssize_t, uint8*> GetArchive();
    uint8* GetArchivePtr();
    ssize_t GetArchiveSize();

   protected:
    virtual bool DoSave(const DatabaseInfo& dbInfo) = 0;

   protected:
    zip_t* zip{nullptr};

   private:
    enum class State { created, inProgress, done };

   private:
    State state{State::created};

    DatabaseInfoList databases;

    DatabaseInfoList::iterator currentDb;

    uint8* archive{nullptr};
    ssize_t archiveSize{0};

    string currentDatabase;
};

#endif  //  _DB_BACKUP_H_
