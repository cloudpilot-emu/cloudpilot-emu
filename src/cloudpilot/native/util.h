#include <memory>
#include <optional>

#include "EmCommon.h"
#include "EmHAL.h"
#include "EmROMReader.h"

namespace util {
    constexpr const char* SUPPORTED_DEVICES[] = {
        "PalmPilot", "Pilot",           "PalmIIIc", "PalmV",           "PalmIII",   "PalmIIIe",
        "PalmIIIx",  "PalmIIIxe",       "PalmVx",   "PalmVII",         "PalmVIIEZ", "PalmVIIx",
        "PalmM500",  "PalmM505",        "PalmM515", "PalmM105",        "PalmM100",  "PalmM125",
        "PalmM130",  "PalmI705",        "PalmI710", "HandEra330",      "PEG-S300",  "PEG-S320",
        "PEG-T400",  "PEG-N600C/N610C", "PEG-T600", "PEG-N700C/N710C", "YSX1230",   "NR70",
        "PEG-S500C", "HandEra330c",     "PalmM520", "Acer-S65",        "Acer-S11",  "Legend-P168"};

    bool readFile(string file, unique_ptr<uint8[]>& buffer, size_t& len);

    bool initializeSession(string file, optional<string> deviceId = optional<string>());

    void analyzeRom(EmROMReader& reader);

    bool mountImage(const string& image);
    string registerImage(const string& image);
    bool mountKey(const string& key);

    EmHAL::Slot mountedSlot();
}  // namespace util
