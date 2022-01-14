#include "PatchModuleClieSlotDriver.h"

#include "EmCommon.h"
#include "Marshal.h"

#define expErrorClass 0x2900  // Expansion Manager and Slot Driver Library

#define expErrUnsupportedOperation (expErrorClass | 1)
#define expErrNotEnoughPower (expErrorClass | 2)
#define expErrCardNotPresent (expErrorClass | 3)
#define expErrInvalidSlotRefNumber (expErrorClass | 4)
#define expErrSlotDeallocated (expErrorClass | 5)
#define expErrCardNoSectorReadWrite (expErrorClass | 6)
#define expErrCardReadOnly (expErrorClass | 7)
#define expErrCardBadSector (expErrorClass | 8)
#define expErrCardProtectedSector (expErrorClass | 9)
#define expErrNotOpen (expErrorClass | 10)
#define expErrStillOpen (expErrorClass | 11)
#define expErrUnimplemented (expErrorClass | 12)
#define expErrEnumerationEmpty (expErrorClass | 13)

namespace {
    CallROMType HeadPatchSelector8(void) {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 slotRefNum, void *infoP");

        PUT_RESULT_VAL(Err, expErrCardNotPresent);

        return kSkipROM;
    }

    ProtoPatchTableEntry protoPatchTable[] = {{sysLibTrapBase + 8, HeadPatchSelector8, NULL},
                                              {0, NULL, NULL}};
}  // namespace

PatchModuleClieSlotDriver::PatchModuleClieSlotDriver() { LoadProtoPatchTable(protoPatchTable); }
