#ifndef _MEMORY_STICK_STRUCTS_
#define _MEMORY_STICK_STRUCTS_

#include "EmCommon.h"

#pragma pack(push, 1)

struct MsBootBlkNfo {
    uint32 start;
    uint32 len;
    uint8 type;
    uint8 rfu[3];
};

struct MsBootBlock {             // BE
    uint16 blkid;                // 0x000 - 0x01 for boot block
    uint16 ftlVer;               // 0x002 - major.minor. must be >= 1.0, < 2.0,
    uint8 rfu0[0xB8];            // 0x004
    uint8 numBbis;               // 0x0BC - how many entries in "bbi[]" are used
    uint8 rfu1[0xB3];            // 0x0BD
    struct MsBootBlkNfo bbi[4];  // 0x170, only first two seem to ever be used, real number used is
                                 // in .numBbis [0].type must be MS_BBI_TYPE_BAD_BLOCK_TAB

    uint8 msClass;        // 0x1A0 - memory sticks have 0x01 here
    uint8 msSubclass;     // 0x1A1 - memory sticks have 0x02 here
    uint16 kbPerBlock;    // 0x1A2 - only 8 and 16 are acceptable values
    uint16 numBlocks;     // 0x1A4 - powers of 2 from 512 to 8192 acceptable
    uint16 usableBlocks;  // 0x1A6 - multiples of 0x1f0 << n acceptable, for n = 0..4. this is the
                          // number of "sectors" we show to the user more or less
    uint16 pageSize;  // 0x1A8 - must be 512 for memorysticks
    uint8 oobSize;    // 0x1AA - must be 16 for memorysticks, but 7 are used for ECC so only 9 are
                    // avail to us
    uint8 unk_1AB;               // 0x1AB
    uint8 manufTimeTzOfst;       // 0x1AC
    uint8 manufTimeYearHi;       // 0x1AD
    uint8 manufTimeYearLo;       // 0x1AE
    uint8 manufTimeMonth;        // 0x1AF
    uint8 manufDateDay;          // 0x1B0
    uint8 manufTimeHour;         // 0x1B1
    uint8 manufTimeMinute;       // 0x1B2
    uint8 manufTimeSecond;       // 0x1B3
    uint32 serialNum;            // 0x1B4
    uint8 assemblyMakerCode;     // 0x1B8 1 = sony, 2 = fujitsu
    uint8 assemblyModelCode[3];  // 0x1B9
    uint16 memManufCode;         // 0x1BC
    uint16 memModelCode;         // 0x1BE
    uint32 unk_1C0;              // 0x1C0
    uint8 vpp;                   // 0x1C4 (in decivolts)
    uint8 vcc;                   // 0x1C5 (in decivolts)
    uint16 controllerNum;        // 0x1C6
    uint16 controllerFunc;       // 0x1C8 - set to 0x1001 for magic gate sticks
    uint8 rfu2[0x09];            // 0x1CA
    uint8 interfaceType;         // 0x1D3
    uint16 controllerCode;       // 0x1D4
    uint8 formatType;            // 0x1D6 - must be 0x01
    uint8 unk_1D7;               // 0x1D7
    uint8 deviceType;            // 0x1D8 - 0 is flash, nonzero = rom
    uint8 rfu3[0x27];            // 0x1D9
};

#pragma pack(pop)

#endif  // _MEMORY_STICK_STRUCTS_
