#include "DbBackupFallback.h"

#include "EmMemory.h"
#include "EmSession.h"
#include "ROMStubs.h"
#include "zip.h"

struct PrcHeader {
    char name[dmDBNameLength];
    UInt16 attributes;
    UInt16 version;
    UInt32 creationDate;
    UInt32 modificationDate;
    UInt32 lastBackupDate;
    UInt32 modificationNumber;
    UInt32 appInfoOfst;
    UInt32 sortInfoOfst;
    UInt32 type;
    UInt32 crid;
    UInt32 uniqueIDSeed;
};

namespace {
    uint32 localIdSize(uint16 cardNo, LocalID lid) {
        emuptr ptr = MemLocalIDToLockedPtr(lid, cardNo);
        uint32 result = MemPtrSize(ptr);

        MemPtrUnlock(ptr);

        return result;
    }
}  // namespace

bool DbBackupFallback::Init(bool includeRomDatabases) {
    if (gSession->IsCpuStopped()) return false;

    return DbBackup::Init(includeRomDatabases);
}

bool DbBackupFallback::DoSave(const DatabaseInfo& dbInfo) {
    PrcHeader hdr;
    LocalID appInfo, sortInfo;
    uint32 currentOffset = 72 + 6 + 2;

    memset(&hdr, 0, sizeof(hdr));

    if (DmDatabaseInfo(dbInfo.cardNo, dbInfo.dbID, hdr.name, &hdr.attributes, &hdr.version,
                       &hdr.creationDate, &hdr.modificationDate, &hdr.lastBackupDate,
                       &hdr.modificationNumber, &appInfo, &sortInfo, &hdr.type,
                       &hdr.crid) != errNone)
        return false;

    hdr.uniqueIDSeed = 0;
    hdr.attributes &= ~dmHdrAttrOpen;

    emuptr db = DmOpenDatabase(dbInfo.cardNo, dbInfo.dbID, dmModeReadOnly);
    if (!db) return false;

    Defer deferCloseDb([=]() { DmCloseDatabase(db); });

    uint16 numChildren;
    if (hdr.attributes & dmHdrAttrResDB) {
        numChildren = DmNumResources(db);
        currentOffset += 10 * numChildren;
    } else {
        numChildren = DmNumRecords(db);
        currentOffset += 8 * numChildren;
    }

    if (appInfo) {
        hdr.appInfoOfst = currentOffset;
        currentOffset += localIdSize(dbInfo.cardNo, appInfo);
    }

    if (sortInfo) {
        hdr.sortInfoOfst = currentOffset;
        currentOffset += localIdSize(dbInfo.cardNo, sortInfo);
    }

    if (!WriteHeader(hdr)) return false;
    if (!WriteChildList(numChildren)) return false;

    children.reserve(numChildren);
    children.clear();

    if (hdr.attributes & dmHdrAttrResDB) {
        for (uint32 i = 0; i < numChildren; i++) {
            LocalID lid;
            uint32 type;
            uint16 id;

            if (DmResourceInfo(db, i, &type, &id, &lid) != errNone) return false;
            children.push_back(lid);

            if (!WriteResourceEntry(type, id, currentOffset)) return false;
            currentOffset += localIdSize(dbInfo.cardNo, lid);
        }
    } else {
        for (uint32 i = 0; i < numChildren; i++) {
            UInt16 attr = 666;
            UInt32 id = 777;
            LocalID lid = 888;

            if (DmRecordInfo(db, i, &attr, &id, &lid) != errNone) return false;
            children.push_back(lid);

            if (!WriteRecordEntry(attr, id, lid != 0 ? currentOffset : 0)) return false;
            if (lid != 0) currentOffset += localIdSize(dbInfo.cardNo, lid);
        }
    }

    if (!Write16(0)) return false;

    if (appInfo) {
        if (!WriteChunk(dbInfo.cardNo, appInfo)) return false;
    }

    if (sortInfo) {
        if (!WriteChunk(dbInfo.cardNo, sortInfo)) return false;
    }

    for (LocalID lid : children) {
        if (lid == 0) continue;
        if (!WriteChunk(dbInfo.cardNo, lid)) return false;
    }

    return true;
}

bool DbBackupFallback::Write8(uint8 value) { return zip_entry_write(zip, &value, 1) == 0; }

bool DbBackupFallback::Write16(uint16 value) {
    uint8 buffer[2];

    buffer[0] = value >> 8;
    buffer[1] = value;

    return zip_entry_write(zip, buffer, 2) == 0;
}

bool DbBackupFallback::Write32(uint32 value) {
    uint8 buffer[4];

    buffer[0] = value >> 24;
    buffer[1] = value >> 16;
    buffer[2] = value >> 8;
    buffer[3] = value;

    return zip_entry_write(zip, buffer, 4) == 0;
}

bool DbBackupFallback::WriteHeader(const PrcHeader& hdr) {
    if (zip_entry_write(zip, hdr.name, dmDBNameLength) != 0) return false;

    return Write16(hdr.attributes) && Write16(hdr.version) && Write32(hdr.creationDate) &&
           Write32(hdr.modificationDate) && Write32(hdr.lastBackupDate) &&
           Write32(hdr.modificationNumber) && Write32(hdr.appInfoOfst) &&
           Write32(hdr.sortInfoOfst) && Write32(hdr.type) && Write32(hdr.crid) &&
           Write32(hdr.uniqueIDSeed);
}

bool DbBackupFallback::WriteChildList(uint16 numChildren) {
    return Write32(0) && Write16(numChildren);
}

bool DbBackupFallback::WriteResourceEntry(uint32 type, uint16 id, uint32 offset) {
    return Write32(type) && Write16(id) && Write32(offset);
}

bool DbBackupFallback::WriteRecordEntry(uint8 attr, uint32 id, uint32 offset) {
    return Write32(offset) && Write8(attr) && Write8(id >> 16) && Write8(id >> 8) && Write8(id);
}

bool DbBackupFallback::WriteChunk(uint16 cardNo, LocalID lid) {
    emuptr ptr = MemLocalIDToLockedPtr(lid, cardNo);
    if (ptr == 0) return false;

    Defer deferUnlock([=]() { MemPtrUnlock(ptr); });

    size_t len = MemPtrSize(ptr);

    chunkTemp.resize(len);
    EmMem_memcpy(static_cast<void*>(chunkTemp.data()), ptr, len);

    return zip_entry_write(zip, chunkTemp.data(), len) == 0;
}
