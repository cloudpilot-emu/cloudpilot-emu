#ifndef _EM_REGS_VZ_HANDERA_330C_H_
#define _EM_REGS_VZ_HANDERA_330C_H_

#include "EmRegsVZHandEra330.h"

class EmRegsVZHandEra330c : public EmRegsVZHandEra330 {
   public:
    EmRegsVZHandEra330c(HandEra330PortManager** portManager);

    virtual int32 GetROMSize(void);

    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh);
    virtual uint16 GetLCD2bitMapping();

   protected:
    virtual void MarkScreen();
    virtual void UnmarkScreen();
    virtual void MarkScreenDirty();
};

#endif  //  _EM_REGS_VZ_HANDERA_330C_H_
