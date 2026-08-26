#ifndef _DEVICE_CONFIGURATION_H_
#define _DEVICE_CONFIGURATION_H_

#include <cstdint>

#include "device_type5.h"

enum class DisplayMode {
    mode_160x160 = 1,
    mode_160x220 = 2,
    mode_240x240 = 3,
    mode_240x320 = 4,
    mode_320x320 = 5,
    mode_320x480 = 6,
    mode_480x480 = 7,
    mode_480x720 = 8
};

struct DisplayConfiguration {
    uint16_t width{0};
    uint16_t height{0};
    uint16_t graffitiHeight{0};
    uint16_t density{0};
};

bool deviceConfigurationDeviceSupportsDisplayMode(DeviceType5 deviceType, DisplayMode displayMode);

DisplayMode deviceConfigurationDefaultDisplayMode(DeviceType5 deviceType);

DisplayConfiguration deviceConfigurationDisplayConfigForMode(DisplayMode mode);

bool deviceConfigurationSupportsRamSize(DeviceType5 deviceType, uint32_t ramSize);

uint32_t deviceConfigurationGetNandSize(DeviceType5 deviceType);

#endif  // _DEVICE_CONFIGURATION_H_
