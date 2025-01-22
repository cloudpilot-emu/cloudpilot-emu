#ifndef _ROM_INFO_H_
#define _ROM_INFO_H_

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>

#include "deviceType.h"

class RomInfo {
   public:
    RomInfo(uint8_t* buffer, size_t size);

    bool IsValid() const;

    const std::string& GetCardName() const;
    const std::string& GetManufacturer() const;
    const std::string& GetRomName() const;

    uint32_t GetCompanyId() const;
    uint32_t GetHalId() const;

    DeviceType GetDeviceType() const;

   private:
    bool Parse();
    bool ParseAt(size_t offset);

    uint32_t Read32(size_t offset, bool& error) const;
    std::string ReadString(size_t offset, uint8_t maxLen, bool& error) const;

   private:
    uint8_t* buffer;
    size_t size;

    bool isValid{false};

    std::string cardName;
    std::string manufacturer;
    std::string romName;

    uint32_t companyId{0};
    uint32_t halId{0};
};

std::ostream& operator<<(std::ostream& stream, RomInfo& romInfo);

#endif  // _ROM_INFO_H_