#ifndef _DB_LIST_H_
#define _DB_LIST_H_

#include <cstdint>
#include <vector>

struct SyscallDispatch;

struct DbMetadata {
    char name[32];

    uint16_t cardNo;
    uint16_t index;
    uint32_t localID;

    uint32_t creator;
    uint32_t type;
    uint32_t attr;

    bool isRom;
};

using DbMetadataList = std::vector<DbMetadata>;

bool dbListGet(struct SyscallDispatch* sd, DbMetadataList& metadataList);

#endif  // _DB_LIST_H_