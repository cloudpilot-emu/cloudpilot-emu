#ifndef _EM_REGS_EZ_PEG_S300_
#define _EM_REGS_EZ_PEG_S300_

#include "EmRegsEZ.h"
#include "EmRegsMB86189.h"
#include "EmSonyXZWithSlot.h"

class EmRegsEzPegS300 : public EmSonyXzWithSlot<EmRegsEZ> {
   public:
    EmRegsEzPegS300(EmRegsMB86189& mb86169);
    virtual ~EmRegsEzPegS300();

   public:
    Bool GetLCDScreenOn(void) override;
    Bool GetLCDBacklightOn(void) override;

    uint8 GetPortInputValue(int) override;
    uint8 GetPortInternalValue(int) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    Bool IDDetectAsserted(void) override;

   protected:
    EmSPISlave* GetSPISlave(void) override;

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_EZ_PEG_S300_
