#ifndef _DB_EXPORTER_
#define _DB_EXPORTER_

#include <vector>

#include "EmCommon.h"
#include "EmStructs.h"

struct PrcHeader;
struct zip_t;

class DbExporter {
   public:
    DbExporter(zip_t* zip);

    bool Export(const DatabaseInfo& dbInfo);

   private:
    bool Write8(uint8 value);
    bool Write16(uint16 value);
    bool Write32(uint32 value);
    bool WriteHeader(const PrcHeader& hdr);
    bool WriteChildList(uint16 numChildren);
    bool WriteResourceEntry(uint32 type, uint16 id, uint32 offset);
    bool WriteRecordEntry(uint8 attr, uint32 id, uint32 offset);
    bool WriteChunk(uint16 cardNo, LocalID lid);

   private:
    vector<LocalID> children;
    vector<uint8> chunkTemp;

    zip_t* zip;

   private:
    DbExporter(const DbExporter&) = delete;
    DbExporter(DbExporter&&) = delete;
    DbExporter& operator=(const DbExporter&) = delete;
    DbExporter& operator=(DbExporter&&) = delete;
};

#endif  // _DB_EXPORTER_
