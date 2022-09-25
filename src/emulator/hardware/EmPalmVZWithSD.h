#ifndef _EM_PALM_VZ_WITH_SD_
#define _EM_PALM_VZ_WITH_SD_

#include <memory>

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveSD.h"
#include "ExternalStorage.h"

template <class VZ>
class EMPalmVZWithSD : public VZ {
   public:
    void Reset(bool hardwareReset) override;

    uint8 GetPortInternalValue(int port) override;

    bool SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) override;
    void Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

    void PortDataChanged(int port, uint8 oldValue, uint8 newValue) override;

   protected:
    EMPalmVZWithSD() = default;

    EmSPISlave* GetSPI1Slave() override;

    void Spi1AssertSlaveSelect() override;
    void Spi1DeassertSlaveSelect() override;

   protected:
    unique_ptr<EmSPISlaveSD> spiSlaveSD = make_unique<EmSPISlaveSD>();
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class VZ>
void EMPalmVZWithSD<VZ>::Reset(bool hardwareReset) {
    VZ::Reset(hardwareReset);

    spiSlaveSD->Reset();
}

template <class VZ>
uint8 EMPalmVZWithSD<VZ>::GetPortInternalValue(int port) {
    uint32 result = VZ::GetPortInternalValue(port);

    if (port == 'D') {
        if (!gExternalStorage.IsMounted(EmHAL::Slot::sdcard))
            result |= 0x20;
        else
            result &= ~0x20;
    };

    return result;
}

template <class VZ>
bool EMPalmVZWithSD<VZ>::SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) {
    return slot == EmHAL::Slot::sdcard &&
           EmSPISlaveSD::IsSizeRepresentable(cardImage.BlocksTotal());
}

template <class VZ>
void EMPalmVZWithSD<VZ>::Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) {
    VZ::UpdateIRQ2(0x20);
}

template <class VZ>
void EMPalmVZWithSD<VZ>::Unmount(EmHAL::Slot slot) {
    VZ::UpdateIRQ2(0x00);
}

template <class VZ>
void EMPalmVZWithSD<VZ>::PortDataChanged(int port, uint8 oldValue, uint8 newValue) {
    VZ::PortDataChanged(port, oldValue, newValue);

    if (port != 'J') return;

    uint8 portJSelect = _get_reg(&VZ::f68VZ328Regs.portJSelect);
    if ((portJSelect & 0x08) == 0) return;
    if (((oldValue ^ newValue) & 0x08) == 0) return;

    if (newValue & 0x08)
        spiSlaveSD->Disable();
    else
        spiSlaveSD->Enable();
}

template <class VZ>
EmSPISlave* EMPalmVZWithSD<VZ>::GetSPI1Slave() {
    return spiSlaveSD.get();
}

template <class VZ>
void EMPalmVZWithSD<VZ>::Spi1AssertSlaveSelect() {
    uint8 portJSelect = _get_reg(&VZ::f68VZ328Regs.portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD->Enable();
}

template <class VZ>
void EMPalmVZWithSD<VZ>::Spi1DeassertSlaveSelect() {
    uint8 portJSelect = _get_reg(&VZ::f68VZ328Regs.portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD->Disable();
}

#endif  // _EM_PALM_VZ_WITH_SD_
