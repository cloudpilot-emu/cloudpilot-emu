#ifndef _EM_REGS_VZ_PALM_I705_H_
#define _EM_REGS_VZ_PALM_I705_H_

#include "EmPalmVZWithSD.h"
#include "EmRegsVZ.h"

class EmRegsVZPalmI705 : public EMPalmVZWithSD<EmRegsVZ> {
   public:
    EmRegsVZPalmI705(void);
    virtual ~EmRegsVZPalmI705(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);
    virtual Bool GetVibrateOn(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPI2Slave(void);
};

#endif  // _EM_REGS_VZ_PALM_I705_H_
