#ifndef _EM_REGS_VZ_PALM_M130_
#define _EM_REGS_VZ_PALM_M130_

#include "EmRegsVZ.h"

class EmRegsVZPalmM130 : public EmRegsVZNoScreen {
   public:
    EmRegsVZPalmM130(void);
    virtual ~EmRegsVZPalmM130(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_VZ_PALM_M130_
