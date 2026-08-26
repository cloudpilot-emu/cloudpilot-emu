#include "device_configuration.h"

#include <sys/stat.h>

#include "device_type5.h"

bool deviceConfigurationSupportsRamSize(enum DeviceType5 deviceType, uint32_t ramSize) {
    switch (deviceType) {
        case DeviceType5::deviceTypeE2:
        case DeviceType5::deviceTypeFrankenE2:
            return ramSize == (16ul << 20) || ramSize == (32ul << 20);

        case DeviceType5::deviceTypePV:
            return (1ul << __builtin_ctz(ramSize)) == ramSize && ramSize >= 8 << 20;

        default:
            return false;
    }
}

uint32_t deviceConfigurationGetNandSize(enum DeviceType5 deviceType) {
    switch (deviceType) {
        case DeviceType5::deviceTypeE2:
        case DeviceType5::deviceTypeFrankenE2:
            return 34603008;

        default:
            return 0;
    }
}

bool deviceConfigurationDeviceSupportsDisplayMode(DeviceType5 deviceType, DisplayMode displayMode) {
    switch (deviceType) {
        case DeviceType5::deviceTypeE2:
            return displayMode == DisplayMode::mode_320x320;

        case DeviceType5::deviceTypeFrankenE2:
            return displayMode == DisplayMode::mode_320x480;

        case DeviceType5::deviceTypePV:
            return true;

        default:
            return false;
    }
}

DisplayMode deviceConfigurationDefaultDisplayMode(DeviceType5 deviceType) {
    switch (deviceType) {
        case DeviceType5::deviceTypeFrankenE2:
        case DeviceType5::deviceTypePV:
            return DisplayMode::mode_320x480;

        default:
            return DisplayMode::mode_320x320;
    }
}

DisplayConfiguration deviceConfigurationDisplayConfigForMode(DisplayMode mode) {
    switch (mode) {
        case DisplayMode::mode_160x160:
            return {.width = 160, .height = 160, .graffitiHeight = 60, .density = 72};

        case DisplayMode::mode_160x220:
            return {.width = 160, .height = 220, .graffitiHeight = 0, .density = 72};

        case DisplayMode::mode_240x240:
            return {.width = 240, .height = 240, .graffitiHeight = 90, .density = 108};

        case DisplayMode::mode_240x320:
            return {.width = 240, .height = 320, .graffitiHeight = 0, .density = 108};

        case DisplayMode::mode_320x320:
            return {.width = 320, .height = 320, .graffitiHeight = 120, .density = 144};

        case DisplayMode::mode_320x480:
            return {.width = 320, .height = 480, .graffitiHeight = 0, .density = 144};

        case DisplayMode::mode_480x480:
            return {.width = 480, .height = 480, .graffitiHeight = 180, .density = 216};

        case DisplayMode::mode_480x720:
            return {.width = 480, .height = 720, .graffitiHeight = 0, .density = 216};

        default:
            return DisplayConfiguration{};
    }
}
