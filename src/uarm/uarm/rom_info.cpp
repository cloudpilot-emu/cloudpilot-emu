#include "rom_info.h"

#include <iomanip>
#include <iostream>

#pragma GCC diagnostic ignored "-Wmultichar"

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

    const char* describeDeviceType(DeviceType deviceType) {
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

RomInfo::RomInfo(const uint8_t* buffer, size_t size) : buffer(buffer), size(size) {
    isValid = Parse();
}

bool RomInfo::IsValid() const { return isValid; }

const std::string& RomInfo::GetCardName() const { return cardName; }

const std::string& RomInfo::GetManufacturer() const { return manufacturer; }

const std::string& RomInfo::GetRomName() const { return romName; }

uint32_t RomInfo::GetCompanyId() const { return companyId; }

uint32_t RomInfo::GetHalId() const { return halId; }

DeviceType RomInfo::GetDeviceType() const {
    if (!isValid) return deviceTypeInvalid;

    if (companyId == COMPANY_ID_PALM && halId == HAL_ID_TUNGSTEN_E2) {
        return deviceTypeE2;
    }

    if (companyId == COMPANY_ID_PALM && halId == HAL_ID_DMITRY_TUNGSTEN_E3) {
        return deviceTypeFrankenE2;
    }

    return deviceTypeInvalid;
}

bool RomInfo::Parse() {
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

bool RomInfo::ParseAt(size_t offset) {
    bool error = false;

    cardName = ReadString(offset + 0x10, 32, error);
    manufacturer = ReadString(offset + 0x30, 32, error);
    romName = ReadString(offset + 0x8c, 32, error);

    companyId = Read32(offset + 0x80, error);
    halId = Read32(offset + 0x84, error);

    return !error;
}

uint32_t RomInfo::Read32(size_t offset, bool& error) const {
    if (error) return 0;

    if (offset + 4 > size) {
        error = true;
        return 0;
    }

    return buffer[offset] | (buffer[offset + 1] << 8) | (buffer[offset + 2] << 16) |
           (buffer[offset + 3] << 24);
}

std::string RomInfo::ReadString(size_t offset, uint8_t maxLen, bool& error) const {
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

std::ostream& operator<<(std::ostream& stream, RomInfo& info) {
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
