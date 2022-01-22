#ifndef _EM_REGS_VZ_YELLOWSTONE_
#define _EM_REGS_VZ_YELLOWSTONE_

#include "EmRegsVZ.h"

class EmScreenUpdateInfo;

class EmRegsVzPegYellowStone : public EmRegsVZNoScreen {
   public:
    EmRegsVzPegYellowStone(void);
    virtual ~EmRegsVzPegYellowStone(void);

   public:
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual uint16 GetLEDState(void);

    virtual Bool GetSerialPortOn(int uartNum);
    virtual Bool GetVibrateOn(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_VZ_YELLOWSTONE_
