#include "db_backup.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Defer.h"
#include "cputil.h"
#include "pace.h"
#include "syscall_dispatch.h"
#include "zip.h"

using namespace std;

namespace {
    constexpr uint16_t dmHdrAttrResDB = 0x0001;
    constexpr uint16_t dmHdrAttrLaunchableData = 0x0200;

    constexpr int COMPRESSION_LEVEL = 1;
    constexpr uint32_t MAX_CHUNK_SIZE = 64 * 1024;

    constexpr uint32_t DB_CREATOR_A68K = 0x6136386b;  // 'a68k'
    constexpr uint32_t DB_CREATOR_CLPR = 0x636c7072;  // 'clpr

    string getEntryName(DbMetadata& meta) {
        const char* suffix;
        const char* prefix;

        if (meta.creator == DB_CREATOR_CLPR && (meta.attr & dmHdrAttrLaunchableData)) {
            suffix = ".pqa";
        } else if (meta.attr & dmHdrAttrResDB) {
            suffix = ".prc";
        } else {
            suffix = ".pdb";
        }

        if (meta.isRom) {
            prefix = "rom/";
        } else if (meta.creator == DB_CREATOR_A68K) {
            prefix = "cache/";
        } else {
            prefix = "";
        }

        return string(prefix) + meta.name + suffix;
    }
}  // namespace

DbBackup::DbBackup(SyscallDispatch* syscallDispatch, BackupType backupType)
    : sd(syscallDispatch), backupType(backupType) {
    memset(lastProcessedDb, 0, sizeof(lastProcessedDb));
}

DbBackup::~DbBackup() {
    if (zip) zip_stream_close(zip);
    if (archiveData) free(archiveData);
}

bool DbBackup::Init() {
    if (state != State::created) ERR("DB backup: already initialized");

    if (!dbListGet(sd, dbList)) {
        state = State::error;
        return false;
    }

    dbIterator = dbList.begin();
    if (dbIterator == dbList.end()) {
        state = State::error;
        cout << "DB backup: empty DB list" << endl;
        return false;
    }

    zip = zip_stream_open(nullptr, 0, COMPRESSION_LEVEL, 'w');
    chunkBuffer = make_unique<uint8_t[]>(MAX_CHUNK_SIZE);
    state = State::inProgress;

    return true;
}

DbBackup::State DbBackup::GetState() const { return state; }

bool DbBackup::Continue() {
    if (state != State::inProgress) ERR("DB backup: not in progress");

    while (dbIterator != dbList.end() && !Include(*dbIterator)) dbIterator++;
    if (dbIterator == dbList.end()) {
        state = State::done;
        return true;
    }

    strncpy(lastProcessedDb, dbIterator->name, sizeof(lastProcessedDb));

    if (!syscallDispatchM68kSupport(sd)) {
        cerr << "DB backup: m68k syscalls not supported";
        state = State::error;
        return false;
    }

    if (!syscallDispatchPrepare(sd)) {
        cerr << "DB backup: failed to prepare safe environment for syscalls";
        state = State::error;
        return false;
    }

    const string entryName = getEntryName(*dbIterator);
    zip_entry_open(zip, entryName.c_str());
    Defer closeEntry([=]() { zip_entry_close(zip); });

    const uint32_t scratch = syscall68k_MemPtrNew(sd, SC_EXECUTE_PURE, 3);
    Defer freeScratch([=]() { syscall68k_MemPtrFree(sd, SC_EXECUTE_PURE, scratch); });

    const uint32_t writeProcP = scratch;
    const uint32_t nameP = scratch + 2;

    paceSet8(nameP, 0);

    syscallDispatchRegisterM68kStub(
        sd, writeProcP,
        [&](struct ArmCpu*, uint32_t parameterBase, std::function<void()> deadMansSwitch) {
            deadMansSwitch();

            paceSetDreg(0, 1);

            uint32_t dataP = paceGet32(parameterBase);
            const uint32_t sizeP = paceGet32(parameterBase + 4);
            uint32_t size = paceGet32(sizeP);

            while (size > 0) {
                const uint32_t transferSize = min(size, MAX_CHUNK_SIZE);

                if (!syscallDispatch_memcpy_toHost(sd, chunkBuffer.get(), dataP, transferSize)) {
                    cout << "DB backup: failed to copy chunk" << endl;
                    return;
                }

                if (zip_entry_write(zip, chunkBuffer.get(), transferSize) != 0) {
                    cout << "DB backup: failed to write chunk" << endl;
                    return;
                }

                size -= transferSize;
                dataP += transferSize;
            }

            paceSetDreg(0, 0);
        });

    Defer unregisterStub([=]() { syscallDispatchUnregisterM68kStub(sd, writeProcP); });

    const uint16_t dbWriteResult = syscall68k_ExgDBWrite(sd, SC_EXECUTE_FULL, writeProcP, 0, nameP,
                                                         dbIterator->localID, dbIterator->cardNo);

    state = ++dbIterator == dbList.end() ? State::done : State::inProgress;

    return dbWriteResult == 0;
}

const char* DbBackup::GetLastProcessedDb() { return lastProcessedDb; }

const void* DbBackup::GetArchiveData() {
    CopyArchiveData();
    return archiveData;
}

size_t DbBackup::GetArchiveSize() {
    CopyArchiveData();
    return archiveSize >= 0 ? archiveSize : 0;
}

bool DbBackup::Include(DbMetadata& db) {
    switch (backupType) {
        case BackupType::ram:
            return !db.isRom && db.creator != DB_CREATOR_A68K;

        case BackupType::ramRom:
            return db.creator != DB_CREATOR_A68K;

        default:
            return true;
    }
}

void DbBackup::CopyArchiveData() {
    if (state != State::done) return;

    if (!archiveData) {
        zip_stream_copy(zip, &archiveData, &archiveSize);

        zip_stream_close(zip);
        zip = nullptr;
    }
}
