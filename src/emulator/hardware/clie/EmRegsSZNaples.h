#ifndef _EM_REGS_SZ_NAPLES_H_H
#define _EM_REGS_SZ_NAPLES_H_H

#include "EmRegsSZ.h"
#include "EmRegsSonyDSP.h"

class EmScreenUpdateInfo;

class EmRegsSzNaples : public EmRegsSZ {
   public:
    EmRegsSzNaples(EmRegsSonyDSP& dsp);
    virtual ~EmRegsSzNaples(void);

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
    EmRegsSonyDSP& dsp;
};

#endif  // _EM_REGS_SZ_NAPLES_H_H
