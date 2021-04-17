#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

#include "EmCommon.h"
#include "Miscellaneous.h"

class DbBackup {
   public:
    enum class State { created, inProgress, done };

   public:
    DbBackup() = default;

    ~DbBackup();

    bool Init();

    State GetState() const;

    string GetCurentDatabase() const;

    bool Save();
    void Skip();

   private:
    void Callback();

   private:
    State state{State::created};

    DatabaseInfoList databases;
    emuptr callbackPtr{0};

    DatabaseInfoList::iterator currentDb;

   private:
    DbBackup(const DbBackup&) = delete;
    DbBackup(DbBackup&&) = delete;
    DbBackup& operator=(const DbBackup&) = delete;
    DbBackup& operator=(DbBackup&&) = delete;
};

#endif  // _DB_BACKUP_H_
