#ifndef _EM_REGS_EZ_PEG_S500C_H_
#define _EM_REGS_EZ_PEG_S500C_H_

#include "EmRegsEZ.h"
#include "EmRegsMB86189.h"
#include "EmSonyXZWithSlot.h"

class EmRegsEzPegS500C : public EmSonyXzWithSlot<EmRegsEZ> {
   public:
    EmRegsEzPegS500C(EmRegsMB86189& mb86189);
    virtual ~EmRegsEzPegS500C();

   public:
    Bool GetLCDScreenOn(void) override;
    Bool GetLCDBacklightOn(void) override;

    uint8 GetPortInputValue(int) override;
    uint8 GetPortInternalValue(int) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    Bool IDDetectAsserted(void) override;

   protected:
    EmSPISlave* GetSPISlave(void) override;

    // Override those with noops --- the framebuffer is not backed with meta
    // memory
    void MarkScreen() override;
    void UnmarkScreen() override;
    void MarkScreenDirty() override;

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  //_EM_REGS_EZ_PEG_S500C_H_
