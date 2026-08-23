#ifndef _DEVICE_CONFIGURATION_H_
#define _DEVICE_CONFIGURATION_H_

#include <cstdint>

#include "device_type5.h"

struct DisplayConfiguration {
    uint16_t width{0};
    uint16_t height{0};
    uint16_t graffitiHeight{0};
};

DisplayConfiguration deviceConfigurationGetDsiplay(enum DeviceType5 deviceType);

bool deviceConfigurationSupportsRamSize(enum DeviceType5 deviceType, uint32_t ramSize);

uint32_t deviceConfigurationGetNandSize(enum DeviceType5 deviceType);

#endif  // _DEVICE_CONFIGURATION_H_
