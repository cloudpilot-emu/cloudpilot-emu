#ifndef _EM_REGS_VZ_PEG_VENICE_H_
#define _EM_REGS_VZ_PEG_VENICE_H_

#include <memory>

#include "EmRegsVZ.h"
#include "EmSonyVZWithSlot.h"

class EmRegsVzPegVenice : public EmSonyVzWithSlot<EmRegsVZ> {
   public:
    EmRegsVzPegVenice(EmRegsMB86189& mb86189);
    virtual ~EmRegsVzPegVenice(void);

   public:
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual uint16 GetLEDState(void);

    virtual Bool GetSerialPortOn(int uartNum);
    virtual Bool GetVibrateOn(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh);

   protected:
    virtual EmSPISlave* GetSPI2Slave(void);

   private:
    EmSPISlave* fSPISlaveADC;
    unique_ptr<uint32[]> framebufferTmp;
};

#endif  // _EM_REGS_VZ_PEG_VENICE_H_
