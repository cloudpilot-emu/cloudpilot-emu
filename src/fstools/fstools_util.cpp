#include "fstools_util.h"

uint32_t FSToolsUtil::determineImageSize(unsigned int size) {
    switch (size) {
        case 4:
            return 494 * 16 * 512;

        case 8:
            return (2 * 496 - 2) * 16 * 512;

        case 16:
            return (2 * 496 - 2) * 32 * 512;

        case 32:
            return (4 * 496 - 2) * 32 * 512;

        case 64:
            return (8 * 496 - 2) * 32 * 512;

        case 128:
            return (16 * 496 - 2) * 32 * 512;

        default:
            return 0;
    }
}
