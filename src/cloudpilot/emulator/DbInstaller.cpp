#include "DbInstaller.h"

#include <cstring>

#include "CallbackManager.h"
#include "EmFileImport.h"
#include "EmMemory.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "LogDomain.h"
#include "Logging.h"
#include "Marshal.h"
#include "ROMStubs.h"

namespace {
    DbInstaller::Result mapErr(Err err) {
        switch (err) {
            case 0:
                return DbInstaller::Result::success;

            case dmErrMemError:
                return DbInstaller::Result::failureNotEnoughMemory;

            case dmErrCorruptDatabase:
                return DbInstaller::Result::failureDbIsCorrupt;

            case dmErrDatabaseOpen:
                return DbInstaller::Result::failureDbIsOpen;

            default:
                return DbInstaller::Result::failureUnknownReason;
        }
    }

    DbInstaller::Result exmgrInstall(size_t bufferSize, uint8* buffer) {
        if (gSession->IsCpuStopped()) return DbInstaller::Result::failureInternal;

        size_t bytesRead = 0;
        bool failedToOverwrite = false;

        CallbackWrapper deleteProcP([&]() {
            CALLED_SETUP_STDARG(
                "Boolean",
                "const char* nameP, UInt16 version, UInt16 cardNo, LocalID dbID, void* userDataP");

            CALLED_GET_PARAM_VAL(UInt16, cardNo);
            CALLED_GET_PARAM_VAL(LocalID, dbID);

            if ((dbID & 0x01) == 0) {
                PUT_RESULT_VAL(Boolean, 1);
                return;
            }

            UInt16 attributes;

            DmDatabaseInfo(cardNo, dbID, NULL, &attributes, NULL, NULL, NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL);

            if (DmDeleteDatabase(cardNo, dbID) == 0) {
                PUT_RESULT_VAL(Boolean, 1);
            } else {
                failedToOverwrite = true;
                PUT_RESULT_VAL(Boolean, 0);
            }
        });

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

            gSession->TriggerDeadMansSwitch();
        });

        LocalID localId;
        Boolean needsReset = false;

        Err err = ExgDBRead(readProcP, deleteProcP, 0, &localId, 0, &needsReset, true);

        if (err == 0 && !failedToOverwrite)
            return needsReset ? DbInstaller::Result::needsReboot : DbInstaller::Result::success;
        if (failedToOverwrite) return DbInstaller::Result::failedCouldNotOverwrite;

        return mapErr(err);
    }
}  // namespace

DbInstaller::DbInstaller(size_t bufferSize, uint8* buffer)
    : bufferSize(bufferSize), buffer(buffer) {
    PreprocessDb();
}

DbInstaller::Result DbInstaller::Install() {
    if (!gSystemState.IsUIInitialized()) return Result::failureInternal;

    if (gSystemState.OSMajorVersion() < 3) {
        logPrintfDom(logging::domainInstaller, "manually installing database");

        return EmFileImport::LoadPalmFile(buffer, bufferSize, kMethodHomebrew) == kError_NoError
                   ? Result::success
                   : Result::failureUnknownReason;
    }

    if (gSystemState.OSMajorVersion() == 3) {
        logPrintfDom(logging::domainInstaller, "attempting to install database via exgmgr");
        Result result = exmgrInstall(bufferSize, buffer);

        // Some versions of PalmOS 3.x fail to install empty DBs
        if (result == Result::success || !isEmptyDb) return result;

        logPrintfDom(logging::domainInstaller, "installation via exgmgr failed, retrying manually");
        return EmFileImport::LoadPalmFile(buffer, bufferSize, kMethodHomebrew) == kError_NoError
                   ? Result::success
                   : Result::failureUnknownReason;
    }

    logPrintfDom(logging::domainInstaller, "installing database via exgmgr");
    return exmgrInstall(bufferSize, buffer);
}

void DbInstaller::PreprocessDb() {
    if (!gSystemState.IsUIInitialized() || gSystemState.OSMajorVersion() != 3) return;

    if (!InspectDb()) {
        logPrintfDom(logging::domainInstaller,
                     "database seems to be invalid, aborting analysis...");
        return;
    }

    // PalmOS 3 cannot handle gaps != 2
    if (gap != 2 && gSystemState.OSMajorVersion() == 3) {
        logPrintfDom(logging::domainInstaller,
                     "fixing up database with nonstandard gap of %u bytes", gap);

        FixupGap();
    }
}

bool DbInstaller::InspectDb() {
    EmAliasDatabaseHdrType<LAS> header(buffer);

    if (header.GetSize() > bufferSize) {
        logPrintfDom(logging::domainInstaller, "suspicious DB: header does not fit buffer");
        return false;
    }

    if (strnlen(reinterpret_cast<const char*>(header.name.GetPtr()), dmDBNameLength) ==
        dmDBNameLength) {
        logPrintfDom(logging::domainInstaller, "suspicious DB: name unterminated or too long");
        return false;
    }

    for (size_t i = 0; i < dmDBNameLength; i++) {
        const char c = reinterpret_cast<const char*>(header.name.GetPtr())[i];

        if (c == '\0') break;
        if (c < 0x20 || c > 0x7e) {
            logPrintfDom(logging::domainInstaller,
                         "suspicious DB: name contains unprintable characters");

            return false;
        }
    }

    isResourceDb = header.attributes & dmHdrAttrResDB;
    entrySize = isResourceDb ? EmAliasRsrcEntryType<LAS>::GetSize()
                             : EmAliasRecordEntryType<LAS>::GetSize();

    uint32 watermark = header.GetSize();

    if (header.appInfoID > 0) {
        if (header.appInfoID < watermark || header.appInfoID >= bufferSize) {
            logPrintfDom(logging::domainInstaller, "suspicious DB: invalid appinfo");
            return false;
        }

        watermark = header.appInfoID;
    }

    if (header.sortInfoID > 0) {
        if (header.sortInfoID < watermark || header.sortInfoID >= bufferSize) {
            logPrintfDom(logging::domainInstaller, "suspicious DB: invalid appinfo");
            return false;
        }

        watermark = header.sortInfoID;
    }

    uint32 firstRecord = bufferSize;
    uint32 offset = header.GetSize();

    if (!(isResourceDb
              ? ValidateEntries<EmAliasRsrcEntryType<LAS>>(header, offset, watermark, firstRecord)
              : ValidateEntries<EmAliasRecordEntryType<LAS>>(header, offset, watermark,
                                                             firstRecord))) {
        return false;
    }

    if (header.sortInfoID > 0) firstRecord = header.sortInfoID;
    if (header.appInfoID > 0) firstRecord = header.appInfoID;

    const uint32 entrySize = isResourceDb ? EmAliasRsrcEntryType<LAS>::GetSize()
                                          : EmAliasRecordEntryType<LAS>::GetSize();

    gap = firstRecord - header.GetSize() - header.recordList.numRecords * entrySize;
    isEmptyDb = header.recordList.numRecords == 0;

    return true;
}

void DbInstaller::FixupGap() {
    size_t originalBufferSize = bufferSize;
    uint8* originalBuffer = buffer;

    const int32 delta = 2 - gap;
    bufferSize += delta;

    preprocessedDb = make_unique<uint8[]>(originalBufferSize + bufferSize);
    buffer = preprocessedDb.get();

    uint32 offset = 0;

    memcpy(buffer, originalBuffer, EmAliasDatabaseHdrType<LAS>::GetSize());
    offset += EmAliasDatabaseHdrType<LAS>::GetSize();

    EmAliasDatabaseHdrType<LAS> header(buffer);

    if (header.appInfoID > 0) header.appInfoID = header.appInfoID + delta;
    if (header.sortInfoID > 0) header.sortInfoID = header.sortInfoID + delta;

    memcpy(buffer + offset, originalBuffer + offset, header.recordList.numRecords * entrySize);

    if (isResourceDb) {
        FixupEntries<EmAliasRsrcEntryType<LAS>>(header, offset, delta);
    } else {
        FixupEntries<EmAliasRecordEntryType<LAS>>(header, offset, delta);
    }

    memset(buffer + offset, 0, 2);
    memcpy(buffer + offset + 2, originalBuffer + offset + gap, originalBufferSize - offset - gap);
}

template <typename T>
bool DbInstaller::ValidateEntries(const EmAliasDatabaseHdrType<LAS>& header, uint32& offset,
                                  uint32& watermark, uint32& firstRecord) {
    for (uint32 i = 0; i < header.recordList.numRecords; i++) {
        T entry(buffer + offset);

        if (offset + entry.GetSize() > bufferSize) {
            logPrintfDom(logging::domainInstaller, "suspicious DB: entry %i exceeds bounds", i);
            return false;
        }

        if (entry.localChunkID < watermark) {
            logPrintfDom(logging::domainInstaller, "suspicious DB: invalid entry %i", i);
            return false;
        }

        offset += entry.GetSize();
        watermark = entry.localChunkID;

        if (i == 0) firstRecord = entry.localChunkID;
    }

    return true;
}

template <typename T>
void DbInstaller::FixupEntries(const EmAliasDatabaseHdrType<LAS>& header, uint32& offset,
                               int32 delta) {
    for (size_t i = 0; i < header.recordList.numRecords; i++) {
        T entry(buffer + offset);
        offset += entry.GetSize();

        entry.localChunkID = entry.localChunkID + delta;
    }
}
