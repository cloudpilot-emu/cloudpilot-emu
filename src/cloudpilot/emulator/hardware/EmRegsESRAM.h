#ifndef _EM_REGS_ESRAM_
#define _EM_REGS_ESRAM_

#include "EmRegs.h"

class EmRegsESRAM : public EmRegs {
   public:
    EmRegsESRAM() = default;

    void Initialize() override;
    void Reset(Bool hardwareReset) override;

    void Enable(emuptr baseAddr);
    void Disable();

    void SetFramebufferBase(emuptr framebufferBase);
    bool IsFramebuffer() const;

    uint32 GetLong(emuptr address) override;
    uint32 GetWord(emuptr address) override;
    uint32 GetByte(emuptr address) override;
    void SetLong(emuptr address, uint32 value) override;
    void SetWord(emuptr address, uint32 value) override;
    void SetByte(emuptr address, uint32 value) override;

    int ValidAddress(emuptr address, uint32 size) override;
    void SetSubBankHandlers() override;
    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart() override;
    uint32 GetAddressRange() override;

   private:
    emuptr baseAddr{0};
    emuptr framebufferBase{0};
    bool isActive{false};
    bool isFramebuffer{false};
};

#endif  // _EM_REGS_ESRAM_
