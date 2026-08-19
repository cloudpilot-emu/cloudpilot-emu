#include "display_configuration.h"

void displayConfigurationGet(enum DeviceType5 deviceType,
                             struct DisplayConfiguration *displayConfiguration) {
    displayConfiguration->width = 320;

    if (deviceType == deviceTypeFrankenE2) {
        displayConfiguration->height = 480;
        displayConfiguration->graffitiHeight = 0;
    } else {
        displayConfiguration->height = 320;
        displayConfiguration->graffitiHeight = 120;
    }
}
