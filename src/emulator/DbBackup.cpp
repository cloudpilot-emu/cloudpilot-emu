#include "DbBackup.h"

#include <functional>

#include "CallbackManager.h"
#include "Marshal.h"
#include "Miscellaneous.h"
#include "ROMStubs.h"
#include "zip.h"

namespace {
    constexpr int COMPRESSION_LEVEL = 1;
}

DbBackup::~DbBackup() {
    if (callbackPtr) CallbackManager::ReleaseCallback(callbackPtr);
    if (zip) zip_stream_close(zip);
    if (archive) free(archive);
}

bool DbBackup::Init() {
    EmAssert(state == State::created);

    if (!GetDatabases(databases, GetDatabaseFlags::kOnlyRamDatabases)) {
        return false;
    }

    callbackPtr = CallbackManager::RegisterCallback(bind(&DbBackup::Callback, this));

    currentDb = databases.begin();

    state = currentDb == databases.end() ? State::done : State::inProgress;

    zip = zip_stream_open(nullptr, 0, COMPRESSION_LEVEL, 'w');

    return true;
}

DbBackup::State DbBackup::GetState() const { return state; }

string DbBackup::GetCurentDatabase() const {
    EmAssert(state == State::inProgress);
    EmAssert(currentDb != databases.end());

    return string(currentDb->dbName) + (IsExecutable(*currentDb) ? ".prc" : ".pdb");
}

bool DbBackup::Save() {
    EmAssert(state == State::inProgress);
    EmAssert(currentDb != databases.end());
    EmAssert(zip);

    zip_entry_open(zip, GetCurentDatabase().c_str());

    bool success =
        ExgDBWrite(callbackPtr, 0, currentDb->dbName, currentDb->dbID, currentDb->cardNo) == 0;

    zip_entry_close(zip);

    currentDb++;

    if (currentDb == databases.end()) state = State::done;

    return success;
}

void DbBackup::Skip() {
    EmAssert(state == State::inProgress);
    EmAssert(currentDb != databases.end());

    currentDb++;

    if (currentDb == databases.end()) state = State::done;
}

pair<ssize_t, uint8*> DbBackup::GetArchive() {
    if (!archive) zip_stream_copy(zip, (void**)&archive, &archiveSize);

    return pair(archiveSize, archive);
}

void DbBackup::Callback() {
    CALLED_SETUP_STDARG("Err", "const void* dataP, UInt32* sizeP, void* userDataP");

    CALLED_GET_PARAM_REF(UInt32, sizeP, Marshal::kInput);
    CALLED_GET_PARAM_PTR(void, dataP, *sizeP, Marshal::kInput);

    if (zip_entry_write(zip, dataP, *sizeP) == 0) {
        PUT_RESULT_VAL(Err, 0);
    } else {
        PUT_RESULT_VAL(Err, 1);
    }

    PUT_RESULT_VAL(Err, 0);
}
