#ifndef _EM_REGS_SONY_DSP_
#define _EM_REGS_SONY_DSP_

#include "EmCommon.h"
#include "EmRegs.h"

class EmRegsSonyDSP : public EmRegs {
   public:
    EmRegsSonyDSP(emuptr baseAddress);

    void Reset(Bool hardwareReset) override;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

   protected:
    uint32 StdRead(emuptr address, int size);
    void StdWrite(emuptr address, int size, uint32 value);

   private:
    uint32 DoStdRead(emuptr address, int size);
    void DoStdWrite(emuptr address, int size, uint32 value);

   private:
    uint8 regs[0x00010000];

    emuptr baseAddress;
};

#endif  // _EM_REGS_SONY_DSP_
