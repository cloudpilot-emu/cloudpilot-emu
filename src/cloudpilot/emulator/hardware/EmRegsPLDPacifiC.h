#ifndef _EM_REGS_PLD_PACIFIC_H_
#define _EM_REGS_PLD_PACIFIC_H_

#include "EmRegsPLDAtlantiC.h"

class EmRegsPLDPacifiC : public EmRegsPLDAtlantiC {
   public:
    EmRegsPLDPacifiC(emuptr baseAddress);

    virtual void SetSubBankHandlers(void);

   private:
    uint32 ReadReg_0x34(emuptr address, int size);
};

#endif  // _EM_REGS_PLD_PACIFIC_H_
