#include "DbBackupNative.h"

#include <functional>

#include "CallbackManager.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Marshal.h"
#include "ROMStubs.h"
#include "zip.h"

DbBackupNative::DbBackupNative() : callback(bind(&DbBackupNative::Callback, this)) {}

bool DbBackupNative::Init(bool includeRomDatabases) {
    if (gSession->IsCpuStopped()) {
        return false;
    }

    if (gSystemState.OSMajorVersion() < 3) return false;

    return DbBackup::Init(includeRomDatabases);
}

bool DbBackupNative::DoSave(const DatabaseInfo& dbInfo) {
    return ExgDBWrite(callback, 0, dbInfo.dbName, dbInfo.dbID, dbInfo.cardNo) == 0;
}

void DbBackupNative::Callback() {
    CALLED_SETUP_STDARG("Err", "const void* dataP, UInt32* sizeP, void* userDataP");

    CALLED_GET_PARAM_REF(UInt32, sizeP, Marshal::kInput);
    CALLED_GET_PARAM_PTR(void, dataP, *sizeP, Marshal::kInput);

    if (zip_entry_write(zip, dataP, *sizeP) == 0) {
        PUT_RESULT_VAL(Err, 0);
    } else {
        PUT_RESULT_VAL(Err, 1);
    }

    PUT_RESULT_VAL(Err, 0);

    gSession->TriggerDeadMansSwitch();
}
