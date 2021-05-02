#ifndef _EM_REGS_PLD_ATLANTIC_H_
#define _EM_REGS_PLD_ATLANTIC_H_

#include "EmCommon.h"
#include "EmHAL.h"
#include "EmRegs.h"

class EmRegsPLDAtlantiC : public EmRegs, EmHALHandler {
   public:
    EmRegsPLDAtlantiC(emuptr baseAddress);

    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual void SetSubBankHandlers(void);

    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    uint32 ReadReg_0x06(emuptr address, int size);
    uint32 ReadReg_0x2e(emuptr address, int size);
    uint32 ReadReg_0x50(emuptr address, int size);

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);

   private:
    uint32 regs[0x15];
    emuptr baseAddress;
};

#endif  // _EM_REGS_PLD_ATLANTIC_H_
