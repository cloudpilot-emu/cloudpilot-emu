#ifndef _DISPLAY_CONFIGURATION_H_
#define _DISPLAY_CONFIGURATION_H_

#include <cstdint>

#include "device_type5.h"

struct DisplayConfiguration {
    uint16_t width;
    uint16_t height;
    uint16_t graffitiHeight;
};

void displayConfigurationGet(enum DeviceType5 deviceType,
                             struct DisplayConfiguration *displayConfiguration);

#endif  // _DISPLAY_CONFIGURATION_H_
