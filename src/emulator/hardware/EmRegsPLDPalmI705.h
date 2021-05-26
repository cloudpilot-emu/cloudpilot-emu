#ifndef _EM_REGS_PLD_PALM_I705_H_
#define _EM_REGS_PLD_PALM_I705_H_

#include "EmPalmStructs.h"  // EmProxyHwrPalmI705PLDType
#include "EmRegsVZ.h"

class EmRegsPLDPalmI705 : public EmRegs, public EmHALHandler {
   public:
    EmRegsPLDPalmI705(emuptr);
    virtual ~EmRegsPLDPalmI705(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual uint16 GetLEDState(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    uint32 low_battRead(emuptr address, int size);
    uint32 usb_intRead(emuptr address, int size);
    uint32 bullet_detRead(emuptr address, int size);

    emuptr fBaseAddr;
    EmProxyHwrPalmI705PLDType fRegs;
};

#endif  // _EM_REGS_PLD_PALM_I705_H_
