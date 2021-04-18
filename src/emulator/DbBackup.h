#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

#include "CallbackManager.h"
#include "EmCommon.h"
#include "Miscellaneous.h"

struct zip_t;

class DbBackup {
   public:
    DbBackup();

    ~DbBackup();

    bool Init();

    bool IsInProgress() const;
    bool IsDone() const;

    const char* GetCurentDatabase();

    bool Save();
    void Skip();

    pair<ssize_t, uint8*> GetArchive();
    uint8* GetArchivePtr();
    ssize_t GetArchiveSize();

   private:
    void Callback();

   private:
    enum class State { created, inProgress, done };

   private:
    State state{State::created};

    DatabaseInfoList databases;
    CallbackWrapper callback;

    DatabaseInfoList::iterator currentDb;

    zip_t* zip{nullptr};

    uint8* archive{nullptr};
    ssize_t archiveSize{0};

    string currentDatabase;

   private:
    DbBackup(const DbBackup&) = delete;
    DbBackup(DbBackup&&) = delete;
    DbBackup& operator=(const DbBackup&) = delete;
    DbBackup& operator=(DbBackup&&) = delete;
};

#endif  // _DB_BACKUP_H_
