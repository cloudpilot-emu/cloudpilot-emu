#include "device_configuration.h"

#include "device_type5.h"

DisplayConfiguration deviceConfigurationGetDsiplay(enum DeviceType5 deviceType) {
    DisplayConfiguration displayConfiguration;

    displayConfiguration.width = 320;

    if (deviceType == deviceTypeFrankenE2) {
        displayConfiguration.height = 480;
        displayConfiguration.graffitiHeight = 0;
    } else {
        displayConfiguration.height = 320;
        displayConfiguration.graffitiHeight = 120;
    }

    return displayConfiguration;
}

bool deviceConfigurationSupportsRamSize(enum DeviceType5 deviceType, uint32_t ramSize) {
    switch (deviceType) {
        case deviceTypeE2:
        case deviceTypeFrankenE2:
            return ramSize == (16ul << 20) || ramSize == (32ul << 20);

        case deviceTypePV:
            return (1ul << __builtin_ctz(ramSize)) == ramSize && ramSize >= 8 << 20;

        default:
            return false;
    }
}

uint32_t deviceConfigurationGetNandSize(enum DeviceType5 deviceType) {
    switch (deviceType) {
        case deviceTypeE2:
        case deviceTypeFrankenE2:
            return 34603008;

        default:
            return 0;
    }
}
