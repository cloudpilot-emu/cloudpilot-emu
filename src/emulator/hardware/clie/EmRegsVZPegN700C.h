
#ifndef _EM_REGS_VZ_PEG_N700C_
#define _EM_REGS_VZ_PEG_N700C_

#include "EmRegsVZ.h"

class EmRegsVzPegN700C : public EmRegsVZNoScreen {
   public:
    EmRegsVzPegN700C();
    virtual ~EmRegsVzPegN700C();

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
    virtual EmSPISlave* GetSPI2Slave(void);
    virtual void portDIntReqEnWrite(emuptr address, int size, uint32 value);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_VZ_PEG_N700C_
