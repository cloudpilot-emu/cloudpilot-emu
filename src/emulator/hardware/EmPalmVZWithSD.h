#ifndef _EM_PALM_VZ_WITH_SD_
#define _EM_PALM_VZ_WITH_SD_

#include <memory>

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveSD.h"
#include "ExternalStorage.h"

template <class XZ>
class EMPalmVZWithSD : public XZ {
   public:
    void Reset(bool hardwareReset) override;

    uint8 GetPortInternalValue(int port) override;

    bool SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) override;
    void Mount(EmHAL::Slot slot, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

    void PortDataChanged(int port, uint8 oldValue, uint8 newValue) override;

   protected:
    EMPalmVZWithSD() = default;

    EmSPISlave* GetSPI1Slave() override;

    void Spi1AssertSlaveSelect() override;
    void Spi1DeassertSlaveSelect() override;

   protected:
    EmSPISlaveSD spiSlaveSD;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class XZ>
void EMPalmVZWithSD<XZ>::Reset(bool hardwareReset) {
    XZ::Reset(hardwareReset);

    spiSlaveSD.Reset();
}

template <class XZ>
uint8 EMPalmVZWithSD<XZ>::GetPortInternalValue(int port) {
    uint32 result = XZ::GetPortInternalValue(port);

    if (port == 'D') {
        if (!gExternalStorage.IsMounted(EmHAL::Slot::sdcard))
            result |= 0x20;
        else
            result &= ~0x20;
    };

    return result;
}

template <class XZ>
bool EMPalmVZWithSD<XZ>::SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) {
    return slot == EmHAL::Slot::sdcard &&
           EmSPISlaveSD::IsSizeRepresentable(cardImage.BlocksTotal());
}

template <class XZ>
void EMPalmVZWithSD<XZ>::Mount(EmHAL::Slot slot, CardImage& cardImage) {
    XZ::UpdateIRQ2(0x20);
}

template <class XZ>
void EMPalmVZWithSD<XZ>::Unmount(EmHAL::Slot slot) {
    XZ::UpdateIRQ2(0x00);
}

template <class XZ>
void EMPalmVZWithSD<XZ>::PortDataChanged(int port, uint8 oldValue, uint8 newValue) {
    XZ::PortDataChanged(port, oldValue, newValue);

    if (port != 'J') return;

    uint8 portJSelect = _get_reg(&XZ::f68VZ328Regs.portJSelect);
    if ((portJSelect & 0x08) == 0) return;
    if (((oldValue ^ newValue) & 0x08) == 0) return;

    if (newValue & 0x08)
        spiSlaveSD.Disable();
    else
        spiSlaveSD.Enable();
}

template <class XZ>
EmSPISlave* EMPalmVZWithSD<XZ>::GetSPI1Slave() {
    return &spiSlaveSD;
}

template <class XZ>
void EMPalmVZWithSD<XZ>::Spi1AssertSlaveSelect() {
    uint8 portJSelect = _get_reg(&XZ::f68VZ328Regs.portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD.Enable();
}

template <class XZ>
void EMPalmVZWithSD<XZ>::Spi1DeassertSlaveSelect() {
    uint8 portJSelect = _get_reg(&XZ::f68VZ328Regs.portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD.Disable();
}

#endif  // _EM_PALM_VZ_WITH_SD_
