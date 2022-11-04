#ifndef _EM_REGS_SONY_DSP_
#define _EM_REGS_SONY_DSP_

#include "EmCommon.h"
#include "EmEvent.h"
#include "EmHAL.h"
#include "EmRegs.h"
#include "MemoryStick.h"

class EmRegsSonyDSP : public EmRegs, public EmHALHandler {
   public:
    static constexpr uint32 ADDRESS_SPACE_SIZE = 0x00010000;

   public:
    EmRegsSonyDSP(emuptr baseAddress);

    void Initialize() override;
    void Reset(Bool hardwareReset) override;

    void Save(Savestate&) override;
    void Save(SavestateProbe&) override;
    void Load(SavestateLoader&) override;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

    bool SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) override;
    void Mount(EmHAL::Slot slot, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;
    void Remount(EmHAL::Slot, CardImage& cardImage) override;

    bool GetIrqLine();

   public:
    EmEvent<bool> irqChange;

   protected:
    uint32 StdRead(emuptr address, int size);
    void StdWrite(emuptr address, int size, uint32 value);

   private:
    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

    inline void MarkPageDirty(emuptr address);
    inline void MarkRangeDirty(emuptr base, uint32 size);

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

    void DoCmdReadBootBlock(uint16 cmd);
    void DoCmdReadOob(uint16 cmd);
    void DoCmdReadBlock(uint16 cmd);
    void DoCmdWriteBlock(uint16 cmd);
    void DoCmdReadBlockOob(uint16 cmd);

   private:
    uint8* regs;
    uint8* regsDirtyPages;
    uint16 savedArguments[6];

    emuptr baseAddress;

    MemoryStick memoryStick;

    bool isMounted{false};

   private:
    EmRegsSonyDSP(const EmRegsSonyDSP&);
    EmRegsSonyDSP(EmRegsSonyDSP&&);
    EmRegsSonyDSP& operator=(const EmRegsSonyDSP&);
    EmRegsSonyDSP& operator=(EmRegsSonyDSP&&);
};

#endif  // _EM_REGS_SONY_DSP_
