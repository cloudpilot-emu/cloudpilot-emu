#include "db_list.h"

#include <iostream>

#include "Defer.h"
#include "pace.h"
#include "syscall_dispatch.h"

using namespace std;

bool dbListGet(struct SyscallDispatch* sd, DbMetadataList& metadataList) {
    if (!syscallDispatchM68kSupport(sd)) {
        cout << "failed to get DB list: m68k syscalls not supported" << endl;
        return false;
    }

    if (!syscallDispatchPrepare(sd)) {
        cout << "failed to get DB list: unable to prepare save environment for syscalls" << endl;
        return false;
    }

    size_t numDbsTotal = 0;
    const uint16_t numCards = syscall68k_MemNumCards(sd, SC_EXECUTE_PURE);

    for (uint16_t cardNo = 0; cardNo < numCards; cardNo++)
        numDbsTotal += syscall68k_DmNumDatabases(sd, SC_EXECUTE_FULL, cardNo);

    const uint32_t scratch = syscall68k_MemPtrNew(sd, SC_EXECUTE_PURE, 44);
    if (scratch == 0) {
        cout << "failed to get DB list: failed to allocate scratch space" << endl;
    }

    Defer releaseScratch([=]() { syscall68k_MemPtrFree(sd, SC_EXECUTE_PURE, scratch); });

    const uint32_t nameP = scratch;
    const uint32_t creatorP = scratch + 32;
    const uint32_t typeP = scratch + 36;
    const uint32_t attrP = scratch + 44;

    metadataList.resize(numDbsTotal);

    size_t i = 0;
    for (uint16_t cardNo = 0; cardNo < numCards; cardNo++) {
        const uint16_t numDbs = syscall68k_DmNumDatabases(sd, SC_EXECUTE_FULL, cardNo);

        for (uint16_t index = 0; index < numDbs; index++) {
            DbMetadata& meta = metadataList[i];
            meta.localID = syscall68k_DmGetDatabase(sd, SC_EXECUTE_FULL, cardNo, index);
            meta.cardNo = cardNo;
            meta.index = index;

            if (meta.localID == 0) {
                cout << "get DB list: unable to retrieve local ID for " << cardNo << " / " << index
                     << " , skipping..." << endl;

                continue;
            }

            meta.isRom = (meta.localID & 0x80000000) == 0;

            const uint16_t err =
                syscall68k_DmDatabaseInfo(sd, SC_EXECUTE_FULL, cardNo, meta.localID, nameP, attrP,
                                          0, 0, 0, 0, 0, 0, 0, typeP, creatorP);

            if (err != 0) {
                cout << "get DB list: unable to retrieve local ID for " << cardNo << " / " << index
                     << " , skipping..." << endl;

                continue;
            }

            if (!syscallDispatch_strncpy_toHost(sd, meta.name, nameP, sizeof(meta.name))) {
                cout << "get DB list: failed to get name for " << cardNo << " / " << index
                     << " , skipping..." << endl;

                continue;
            }

            meta.name[sizeof(meta.name) - 1] = '\0';

            meta.creator = paceGet32(creatorP);
            meta.type = paceGet32(typeP);
            meta.attr = paceGet32(attrP);

            i++;
        }
    }

    metadataList.resize(i);

    return true;
}
