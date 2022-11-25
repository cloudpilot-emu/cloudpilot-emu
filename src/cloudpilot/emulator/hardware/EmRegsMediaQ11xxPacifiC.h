#ifndef _EM_REGS_MEDIAQ1XX_PACiFIC_H_
#define _EM_REGS_MEDIAQ1XX_PACiFIC_H_

#include "EmRegsMediaQ11xx.h"

class EmRegsMediaQ11xxPacifiC : public EmRegsMediaQ11xx {
   public:
    EmRegsMediaQ11xxPacifiC(EmRegsFrameBuffer& framebuffer, emuptr baseRegsAddr,
                            emuptr baseVideoAddr);

    virtual void SetSubBankHandlers(void);

   private:
    uint32 ReadFPREG9(emuptr address, int size);
};

#endif  // _EM_REGS_MEDIAQ1XX_PACiFIC_H_
