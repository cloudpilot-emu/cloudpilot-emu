#include "rom_info5.h"

#include <iomanip>
#include <iostream>
#include <string>

#pragma GCC diagnostic ignored "-Wmultichar"

using namespace std;

namespace {
    constexpr uint32_t HAL_ID_DMITRY_TUNGSTEN_E3 = 'DmGR';
    constexpr uint32_t HAL_ID_TUNGSTEN_E2 = 'hspr';
    constexpr uint32_t COMPANY_ID_PALM = 'Palm';

    constexpr uint32_t SIGNATURE = 0xfeedbeef;

    std::string formatMultichar(uint32_t multichar) {
        char str[5] = "????";

        for (int8_t i = 3; i >= 0; i--) {
            char c = multichar & 0xff;
            multichar >>= 8;

            if (c >= 0x21 && c <= 0x7e) str[i] = c;
        }

        return str;
    }

    const char* describeDeviceType(DeviceType5 deviceType) {
        switch (deviceType) {
            case deviceTypeE2:
                return "Tungsten E2";

            case deviceTypeFrankenE2:
                return "Tungsten E3 / Dmitry's Frankenpilot";

            default:
                return "unknown";
        }
    }
}  // namespace

RomInfo5::RomInfo5(const void* buffer, size_t size)
    : buffer(static_cast<const uint8_t*>(buffer)), size(size) {
    isValid = Parse();
}

bool RomInfo5::IsValid() const { return isValid; }

const char* RomInfo5::GetCardName() const { return cardName.c_str(); }

const char* RomInfo5::GetManufacturer() const { return manufacturer.c_str(); }

const char* RomInfo5::GetRomName() const { return romName.c_str(); }

bool RomInfo5::IsDmitryPatched() const {
    return (GetCardName() != string("PalmCard") || GetHalId() != HAL_ID_TUNGSTEN_E2);
}

bool RomInfo5::NeedsNand() const { return !IsDmitryPatched(); }

bool RomInfo5::NeedsNandPatch() const { return IsDmitryPatched(); }

uint32_t RomInfo5::GetCompanyId() const { return companyId; }

uint32_t RomInfo5::GetHalId() const { return halId; }

uint32_t RomInfo5::GetRecommendedRamSize() const {
    return IsDmitryPatched() ? (32ul << 20) : (16ul << 20);
}

DeviceType5 RomInfo5::GetDeviceType() const {
    if (!isValid) return deviceTypeInvalid;

    if (companyId == COMPANY_ID_PALM && halId == HAL_ID_TUNGSTEN_E2) {
        return deviceTypeE2;
    }

    if (companyId == COMPANY_ID_PALM && halId == HAL_ID_DMITRY_TUNGSTEN_E3) {
        return deviceTypeFrankenE2;
    }

    return deviceTypeInvalid;
}

bool RomInfo5::Parse() {
    size_t offset = 0;
    bool signatureFound = false;
    bool error = false;

    for (offset = 0; offset + 12 <= size && !signatureFound && !error; offset += 32 * 1024) {
        uint32_t sig = Read32(offset + 8, error);
        signatureFound = sig == SIGNATURE;

        if (!signatureFound) continue;

        if (ParseAt(offset)) return true;
    }

    return false;
}

bool RomInfo5::ParseAt(size_t offset) {
    bool error = false;

    cardName = ReadString(offset + 0x10, 32, error);
    manufacturer = ReadString(offset + 0x30, 32, error);
    romName = ReadString(offset + 0x8c, 32, error);

    companyId = Read32(offset + 0x80, error);
    halId = Read32(offset + 0x84, error);

    return !error;
}

uint32_t RomInfo5::Read32(size_t offset, bool& error) const {
    if (error) return 0;

    if (offset + 4 > size) {
        error = true;
        return 0;
    }

    return buffer[offset] | (buffer[offset + 1] << 8) | (buffer[offset + 2] << 16) |
           (buffer[offset + 3] << 24);
}

std::string RomInfo5::ReadString(size_t offset, uint8_t maxLen, bool& error) const {
    if (error) return "";

    char str[256];
    bool terminated = false;

    for (size_t i = 0; i < maxLen && !terminated; i++) {
        str[i] = buffer[offset + i];
        terminated = str[i] == '\0';
    }

    if (!terminated) {
        error = true;
        return "";
    }

    return str;
}

std::ostream& operator<<(std::ostream& stream, RomInfo5& info) {
    if (!info.IsValid()) {
        stream << "invalid ROM" << std::endl;
        return stream;
    }

    stream << "device: " << describeDeviceType(info.GetDeviceType()) << std::endl
           << "card name: " << info.GetCardName() << std::endl
           << "ROM version: " << info.GetRomName() << std::endl
           << "manufacturer: " << info.GetManufacturer() << std::endl
           << "HAL ID: " << formatMultichar(info.GetHalId()) << std::endl
           << "company ID: " << formatMultichar(info.GetCompanyId()) << std::endl;

    return stream;
}
