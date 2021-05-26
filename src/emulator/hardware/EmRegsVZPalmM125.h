#ifndef _EM_REGS_VZ_PALM_M125_H_
#define _EM_REGS_VZ_PALM_M125_H_

#include "EmRegsVZ.h"

class EmRegsVZPalmM125 : public EmRegsVZ {
   public:
    EmRegsVZPalmM125(void);
    virtual ~EmRegsVZPalmM125(void);

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

#endif  // _EM_REGS_VZ_PALM_M125_H_
