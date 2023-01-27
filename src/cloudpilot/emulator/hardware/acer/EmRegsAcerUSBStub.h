#ifndef _EM_REGS_ACER_USB_STUB_
#define _EM_REGS_ACER_USB_STUB_

#include "EmCommon.h"
#include "EmRegs.h"

class EmRegsAcerUSBStub : public EmRegs {
   public:
    EmRegsAcerUSBStub() = default;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

   private:
    uint32 stubRead(emuptr address, int size);
    void stubWrite(emuptr address, int size, uint32 value);
};

#endif  // _EM_REGS_ACER_USB_STUB_
