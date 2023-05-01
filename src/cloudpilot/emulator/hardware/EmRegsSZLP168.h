#ifndef _EM_REGS_SZ_LP168_
#define _EM_REGS_SZ_LP168_

#include "EmRegsSZ.h"

class EmRegsSZLP168 : public EmRegsSZ {
   public:
    EmRegsSZLP168();
    virtual ~EmRegsSZLP168(void);

   public:
    Bool GetLCDScreenOn(void) override;
    Bool GetLCDBacklightOn(void) override;

    uint8 GetPortInputValue(int port) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

   protected:
    uint8 GetKeyBits() override;
};

#endif  // _EM_REGS_SZ_LP168_
