#ifndef _EM_REGS_VZ_ACER_S1X_H_
#define _EM_REGS_VZ_ACER_S1X_H_

#include "EmRegsMB86189.h"
#include "EmRegsVZ.h"

class EmRegsVZAcerS1x : public EmRegsVZ {
   public:
    EmRegsVZAcerS1x(EmRegsMB86189& mb86189);
    virtual ~EmRegsVZAcerS1x();

    Bool GetLCDScreenOn() override;
    Bool GetLCDBacklightOn() override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    uint8 GetPortInternalValue(int port) override;

   protected:
    EmSPISlave* GetSPI2Slave() override;

   private:
    EmRegsMB86189& mb86189;
};

#endif  //  _EM_REGS_VZ_ACER_S1X_H_
