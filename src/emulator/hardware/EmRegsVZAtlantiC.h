#ifndef _EM_REGS_VZ_ATLANTIC_H_
#define _EM_REGS_VZ_ATLANTIC_H_

#include <memory.h>

#include "EmCommon.h"
#include "EmRegsVZ.h"
#include "EmSPISlaveSD.h"

class EmRegsVZAtlantiC : public EmRegsVZNoScreen {
   public:
    EmRegsVZAtlantiC();
    ~EmRegsVZAtlantiC();

    void Reset(Bool hardwareReset) override;

    Bool GetLCDScreenOn() override;
    Bool GetLCDBacklightOn() override;

    uint8 GetPortInputValue(int port) override;
    uint8 GetPortInternalValue(int port) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    bool SupportsSlot(EmHAL::Slot slot) override;
    void Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

    Bool GetLineDriverState(EmUARTDeviceType) override;
    EmUARTDeviceType GetUARTDevice(int uartNum) override;

    Bool GetDTR(int uartNum) override;

    Bool GetVibrateOn(void) override;

    void PortDataChanged(int port, uint8 oldValue, uint8 newValue) override;

   protected:
    EmSPISlave* GetSPI2Slave(void) override;
    EmSPISlave* GetSPI1Slave() override;

    void Spi1AssertSlaveSelect() override;
    void Spi1DeassertSlaveSelect() override;

   private:
    unique_ptr<EmSPISlaveSD> spiSlaveSD = make_unique<EmSPISlaveSD>();
};

#endif  // _EM_REGS_VZ_ATLANTIC_H_
