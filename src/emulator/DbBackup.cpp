#include "DbBackup.h"

#include "Miscellaneous.h"

void DbBackup::init() {
    DatabaseInfoList databases;

    if (!GetDatabases(databases, GetDatabaseFlags::kOnlyRamDatabases)) {
        cout << "unable to retrieve database list" << endl;
        return;
    }

    for (auto& dbInfo : databases) {
        cout << dbInfo.dbName << (IsExecutable(dbInfo) ? ".prc" : ".pdb") << endl;
    }
}
