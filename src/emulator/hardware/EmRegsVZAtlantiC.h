#ifndef _EM_REGS_VZ_ATLANTIC_H_
#define _EM_REGS_VZ_ATLANTIC_H_

#include "EmCommon.h"
#include "EmRegsVZ.h"

class EmRegsVZAtlantiC : public EmRegsVZNoScreen {
   public:
    EmRegsVZAtlantiC();
    ~EmRegsVZAtlantiC();

    virtual Bool GetLCDScreenOn();
    virtual Bool GetLCDBacklightOn();

    virtual uint8 GetPortInputValue(int port);
    virtual uint8 GetPortInternalValue(int port);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

    virtual Bool GetLineDriverState(EmUARTDeviceType);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);

    virtual Bool GetDTR(int uartNum);

    virtual Bool GetVibrateOn(void);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_VZ_ATLANTIC_H_
