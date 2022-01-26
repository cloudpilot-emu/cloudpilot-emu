#include <memory>

#include "EmCommon.h"
#include "EmROMReader.h"

namespace util {
    constexpr const char* SUPPORTED_DEVICES[] = {
        "PalmPilot", "Pilot",           "PalmIIIc", "PalmV",          "PalmIII",   "PalmIIIe",
        "PalmIIIx",  "PalmIIIxe",       "PalmVx",   "PalmVII",        "PalmVIIEZ", "PalmVIIx",
        "PalmM500",  "PalmM505",        "PalmM515", "PalmM105",       "PalmM100",  "PalmM125",
        "PalmM130",  "PalmI705",        "PalmI710", "HandEra330",     "PEG-S300",  "PEG-S320",
        "PEG-T400",  "PEG-N600C/N610C", "PEG-T600", "PEG-N700C/N710C"};

    bool readFile(string file, unique_ptr<uint8[]>& buffer, size_t& len);

    bool initializeSession(string file, optional<string> deviceId = optional<string>());

    void analyzeRom(EmROMReader& reader);
}  // namespace util
