#include "RomInfo.h"

#include "EmDevice.h"

RomInfo::RomInfo(void* buffer, size_t size) : romReader(buffer, size) {
    valid = romReader.Read();

    if (valid) {
        cardName = romReader.GetCardName();
        romVersionString = romReader.GetRomVersionString();
    }
}

bool RomInfo::IsValid() const { return valid; }

int RomInfo::CardVersion() { return valid ? romReader.GetCardVersion() : -1; }

const char* RomInfo::CardName() { return cardName.c_str(); }

int RomInfo::RomVersion() { return valid ? romReader.GetRomVersion() : -1; }

const char* RomInfo::RomVersionString() { return romVersionString.c_str(); }

bool RomInfo::Supports(string deviceId) {
    EmDevice device(deviceId);

    return device.SupportsROM(romReader);
}
