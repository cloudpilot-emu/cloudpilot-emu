#include "DbBackup.h"

#include "Miscellaneous.h"

void DbBackup::init() {
    DatabaseInfoList databases;

    if (!GetDatabases(databases, false)) {
        cout << "unable to retrieve database list" << endl;
        return;
    }

    for (auto& dbInfo : databases) {
        if (dbInfo.dbAttrs & dmHdrAttrReadOnly) continue;

        cout << dbInfo.dbName << endl;
    }
}
