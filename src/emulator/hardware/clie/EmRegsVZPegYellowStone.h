#ifndef _EM_REGS_VZ_YELLOWSTONE_
#define _EM_REGS_VZ_YELLOWSTONE_

#include "EmRegsVZ.h"
#include "EmSonyVZWithSlot.h"

class EmScreenUpdateInfo;

class EmRegsVzPegYellowStone : public EmSonyVzWithSlot<EmRegsVZNoScreen> {
   public:
    EmRegsVzPegYellowStone(EmRegsMB86189& mb86189);
    virtual ~EmRegsVzPegYellowStone(void);

   public:
    Bool GetLCDScreenOn(void) override;
    Bool GetLCDBacklightOn(void) override;
    uint16 GetLEDState(void) override;

    virtual Bool GetSerialPortOn(int uartNum);
    Bool GetVibrateOn(void) override;

    uint8 GetPortInputValue(int) override;
    uint8 GetPortInternalValue(int) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

   protected:
    EmSPISlave* GetSPI2Slave(void) override;

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif  // _EM_REGS_VZ_YELLOWSTONE_
