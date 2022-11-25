#ifndef _EM_REGS_VZ_ATLANTIC_H_
#define _EM_REGS_VZ_ATLANTIC_H_

#include <memory.h>

#include "EmCommon.h"
#include "EmPalmVZWithSD.h"
#include "EmRegsVZ.h"

class EmRegsVZAtlantiC : public EMPalmVZWithSD<EmRegsVZNoScreen> {
   public:
    EmRegsVZAtlantiC();
    ~EmRegsVZAtlantiC();

    Bool GetLCDScreenOn() override;
    Bool GetLCDBacklightOn() override;

    uint8 GetPortInputValue(int port) override;
    uint8 GetPortInternalValue(int port) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    Bool GetLineDriverState(EmUARTDeviceType) override;
    EmUARTDeviceType GetUARTDevice(int uartNum) override;

    Bool GetDTR(int uartNum) override;

    Bool GetVibrateOn(void) override;

   protected:
    EmSPISlave* GetSPI2Slave(void) override;
};

#endif  // _EM_REGS_VZ_ATLANTIC_H_
