#ifndef _EM_REGS_ACER_DSP_STUB_
#define _EM_REGS_ACER_DSP_STUB_

#include "EmCommon.h"
#include "EmRegs.h"

class EmRegsAcerDSPStub : public EmRegs {
   public:
    EmRegsAcerDSPStub() = default;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

   private:
    uint32 stubRead(emuptr address, int size);
    void stubWrite(emuptr address, int size, uint32 value);
};

#endif  // _EM_REGS_ACER_DSP_STUB_
