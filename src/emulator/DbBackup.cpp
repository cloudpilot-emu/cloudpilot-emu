#include "DbBackup.h"

#include <functional>

#include "CallbackManager.h"
#include "EmCPU68K.h"
#include "Marshal.h"
#include "Miscellaneous.h"
#include "ROMStubs.h"
#include "UAE.h"

// extern struct regstruct regs;

DbBackup::~DbBackup() {
    if (callbackPtr) CallbackManager::ReleaseCallback(callbackPtr);
}

bool DbBackup::Init() {
    EmAssert(state == State::created);

    if (!GetDatabases(databases, GetDatabaseFlags::kOnlyRamDatabases)) {
        return false;
    }

    callbackPtr = CallbackManager::RegisterCallback(bind(&DbBackup::Callback, this));

    currentDb = databases.begin();

    state = currentDb == databases.end() ? State::done : State::inProgress;

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

    bool success =
        ExgDBWrite(callbackPtr, 0, currentDb->dbName, currentDb->dbID, currentDb->cardNo) == 0;

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

void DbBackup::Callback() {
    CALLED_SETUP_STDARG("Err", "const void* dataP, UInt32* sizeP, void* userDataP");

    CALLED_GET_PARAM_REF(UInt32, sizeP, Marshal::kInput);
    CALLED_GET_PARAM_PTR(emuptr, dataP, *sizeP, Marshal::kInput);

    PUT_RESULT_VAL(Err, 0);
}
