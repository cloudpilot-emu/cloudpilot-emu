#ifndef _EM_REGS_VZ_PEG_NASCA_H_
#define _EM_REGS_VZ_PEG_NASCA_H_

#include "EmRegsVZ.h"
#include "EmSonyVZWithSlot.h"

class EmRegsVzPegNasca : public EmSonyVzWithSlot<EmRegsVZ> {
   public:
    EmRegsVzPegNasca(EmRegsMB86189& mb86189);
    virtual ~EmRegsVzPegNasca(void);

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

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  //  _EM_REGS_VZ_PEG_NASCA_H_
