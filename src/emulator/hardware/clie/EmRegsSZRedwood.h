#ifndef _EM_REGS_SZ_REDWOOD_H_
#define _EM_REGS_SZ_REDWOOD_H_

#include "EmRegsSZ.h"
#include "EmRegsSonyDSP.h"

class EmScreenUpdateInfo;

class EmRegsSzRedwood : public EmRegsSZ {
   public:
    EmRegsSzRedwood(EmRegsSonyDSP& dsp);
    virtual ~EmRegsSzRedwood(void);

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
    EmRegsSonyDSP& dsp;
};

#endif  //  _EM_REGS_SZ_REDWOOD_H_
