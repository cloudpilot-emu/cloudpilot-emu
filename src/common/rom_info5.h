#ifndef _ROM_INFO5_H_
#define _ROM_INFO5_H_

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>

#include "device_type5.h"

class RomInfo5 {
   public:
    RomInfo5(const void* buffer, size_t size);

    bool IsValid() const;

    const char* GetCardName() const;
    const char* GetManufacturer() const;
    const char* GetRomName() const;

    uint32_t GetCompanyId() const;
    uint32_t GetHalId() const;

    DeviceType5 GetDeviceType() const;

   private:
    bool Parse();
    bool ParseAt(size_t offset);

    uint32_t Read32(size_t offset, bool& error) const;
    std::string ReadString(size_t offset, uint8_t maxLen, bool& error) const;

   private:
    const uint8_t* buffer;
    size_t size;

    bool isValid{false};

    std::string cardName;
    std::string manufacturer;
    std::string romName;

    uint32_t companyId{0};
    uint32_t halId{0};
};

std::ostream& operator<<(std::ostream& stream, RomInfo5& romInfo);

#endif  // _ROM_INFO5_H_