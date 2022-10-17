#ifndef _EM_REGS_SONY_DSP_
#define _EM_REGS_SONY_DSP_

#include "EmCommon.h"
#include "EmEvent.h"
#include "EmHAL.h"
#include "EmRegs.h"
#include "MemoryStick.h"

class EmRegsSonyDSP : public EmRegs, public EmHALHandler {
   public:
    EmRegsSonyDSP(emuptr baseAddress);

    void Reset(Bool hardwareReset) override;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

    bool SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) override;
    void Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

    bool GetIrqLine();

   public:
    EmEvent<bool> irqChange;

   protected:
    uint32 StdRead(emuptr address, int size);
    void StdWrite(emuptr address, int size, uint32 value);

   private:
    uint32 DoStdRead(emuptr address, int size);
    void DoStdWrite(emuptr address, int size, uint32 value);

    void IrqMaskWrite(emuptr address, int size, uint32 value);
    void IrqStatusWrite(emuptr address, int size, uint32 value);

    void ResetWrite(emuptr address, int size, uint32 value);

    void RaiseInt(uint16 flags);
    void ClearInt(uint16 flags);

    void IpcStatusWrite(emuptr address, int size, uint32 value);
    void IpcCmdWrite(emuptr address, int size, uint32 value);
    void IpcDispatch(uint16 cmd);

    void DoCmdReadBootBlock();
    void DoCmdReadOob();
    void DoCmdReadBlock();
    void DoCmdWriteBlock();

   private:
    uint8 regs[0x00010000];
    uint16 savedArguments[6];

    emuptr baseAddress;

    MemoryStick memoryStick;

   private:
    EmRegsSonyDSP(const EmRegsSonyDSP&);
    EmRegsSonyDSP(EmRegsSonyDSP&&);
    EmRegsSonyDSP& operator=(const EmRegsSonyDSP&);
    EmRegsSonyDSP& operator=(EmRegsSonyDSP&&);
};

#endif  // _EM_REGS_SONY_DSP_
