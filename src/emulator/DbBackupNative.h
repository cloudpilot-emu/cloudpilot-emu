#ifndef _DB_BACKUP_NATIVE_H_
#define _DB_BACKUP_NATIVE_H_

#include "CallbackManager.h"
#include "DbBackup.h"
#include "EmCommon.h"

class DbBackupNative : public DbBackup {
   public:
    DbBackupNative();

    bool Init(bool includeRomDatabases) override;

   protected:
    bool DoSave(const DatabaseInfo& dbInfo) override;

   private:
    void Callback();

   private:
    CallbackWrapper callback;

   private:
    DbBackupNative(const DbBackupNative&) = delete;
    DbBackupNative(DbBackupNative&&) = delete;
    DbBackupNative& operator=(const DbBackupNative&) = delete;
    DbBackupNative& operator=(DbBackupNative&&) = delete;
};

#endif  // _DB_BACKUP_NATIVE_H_
