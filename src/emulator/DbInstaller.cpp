#include "DbInstaller.h"

#include "CallbackManager.h"
#include "EmHAL.h"
#include "EmMemory.h"
#include "Marshal.h"
#include "ROMStubs.h"

DbInstaller::Result DbInstaller::Install(size_t bufferSize, uint8* buffer) {
    if (EmHAL::GetAsleep()) return Result::failure;

    size_t bytesRead = 0;
    bool failedToOverwrite = false;

    CallbackWrapper readProcP([&]() {
        CALLED_SETUP_STDARG("Err", "void* dataP, UInt32* sizeP, void* userDataP");

        CALLED_GET_PARAM_VAL(emuptr, dataP);
        CALLED_GET_PARAM_REF(UInt32, sizeP, Marshal::kInOut);

        ssize_t bytesToCopy =
            min(static_cast<ssize_t>(*sizeP), static_cast<ssize_t>(bufferSize - bytesRead));

        EmAssert(bytesToCopy >= 0);

        EmMem_memcpy<emuptr, void*>(dataP, buffer + bytesRead, bytesToCopy);

        bytesRead += bytesToCopy;
        *sizeP = bytesToCopy;

        CALLED_PUT_PARAM_REF(sizeP);

        PUT_RESULT_VAL(Err, 0);
    });

    CallbackWrapper deleteProcP([&]() {
        CALLED_SETUP_STDARG(
            "Boolean",
            "const char* nameP, UInt16 version, UInt16 cardNo, LocalID dbID, void* userDataP");

        CALLED_GET_PARAM_VAL(UInt16, cardNo);
        CALLED_GET_PARAM_VAL(LocalID, dbID);

        UInt16 attributes;

        DmDatabaseInfo(cardNo, dbID, NULL, &attributes, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                       NULL, NULL);

        if (DmDeleteDatabase(cardNo, dbID) == 0) {
            PUT_RESULT_VAL(Boolean, 1);
        } else {
            failedToOverwrite = true;
            PUT_RESULT_VAL(Boolean, 0);
        }
    });

    LocalID localId;
    Boolean needsReset = false;

    if (ExgDBRead(readProcP, deleteProcP, 0, &localId, 0, &needsReset, true) == 0 &&
        !failedToOverwrite)
        return needsReset ? Result::needsReboot : Result::success;

    return Result::failure;
}
