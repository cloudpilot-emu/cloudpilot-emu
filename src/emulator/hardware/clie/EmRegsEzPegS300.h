#ifndef _EM_REGS_EZ_PEG_S300_
#define _EM_REGS_EZ_PEG_S300_

#include "EmRegsEZ.h"

class EmRegsEzPegS300 : public EmRegsEZ {
   public:
    EmRegsEzPegS300();
    virtual ~EmRegsEzPegS300();

   public:
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetSerialPortOn(int uartNum);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_EZ_PEG_S300_
