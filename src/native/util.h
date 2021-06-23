#include <memory>

#include "EmCommon.h"
#include "EmROMReader.h"

namespace util {
    bool readFile(string file, unique_ptr<uint8[]>& buffer, size_t& len);

    bool initializeSession(string file, string deviceId = "");

    void analyzeRom(EmROMReader& reader);
}  // namespace util
