#ifndef _EM_REGS_EZ_PEG_S500C_H_
#define _EM_REGS_EZ_PEG_S500C_H_

#include "EmRegsEZ.h"

class EmRegsEzPegS500C : public EmRegsEZ {
   public:
    EmRegsEzPegS500C();
    virtual ~EmRegsEzPegS500C();

   public:
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetSerialPortOn(int uartNum);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

    // Override those with noops --- the framebuffer is not backed with meta
    // memory
    virtual void MarkScreen();
    virtual void UnmarkScreen();

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  //_EM_REGS_EZ_PEG_S500C_H_
