#ifndef _DB_BACKUP_H_
#define _DB_BACKUP_H_

class DbBackup {
   public:
    DbBackup() = default;

    void init();

   private:
    DbBackup(const DbBackup&) = delete;
    DbBackup(DbBackup&&) = delete;
    DbBackup& operator=(const DbBackup&) = delete;
    DbBackup& operator=(DbBackup&&) = delete;
};

#endif  // _DB_BACKUP_H_
