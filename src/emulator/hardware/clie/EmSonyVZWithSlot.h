#ifndef _EM_SONY_VZ_WITH_SLOT_
#define _EM_SONY_VZ_WITH_SLOT_

#include "EmCommon.h"
#include "EmHAL.h"
#include "EmRegsMB86189.h"
#include "ExternalStorage.h"

template <class VZ>
class EmSonyVzWithSlot : public VZ {
   public:
    EmSonyVzWithSlot(EmRegsMB86189& mb86189);

    void Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

    uint8 GetPortInternalValue(int) override;

   private:
    EmRegsMB86189& mb86189;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class VZ>
EmSonyVzWithSlot<VZ>::EmSonyVzWithSlot(EmRegsMB86189& mb86189) : mb86189(mb86189) {
    mb86189.irqChange.AddHandler([=](bool newState) { VZ::UpdateIRQ3(newState ? 0x40 : 0x00); });
}

template <class VZ>
void EmSonyVzWithSlot<VZ>::Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) {
    if (this->GetNextHandler()) this->GetNextHandler()->Mount(slot, key, cardImage);
    if (slot != EmHAL::Slot::memorystick) return;

    VZ::UpdatePortDInterrupts();
}

template <class VZ>
void EmSonyVzWithSlot<VZ>::Unmount(EmHAL::Slot slot) {
    if (this->GetNextHandler()) this->GetNextHandler()->Unmount(slot);
    if (slot != EmHAL::Slot::memorystick) return;

    VZ::UpdatePortDInterrupts();
}

template <class VZ>
uint8 EmSonyVzWithSlot<VZ>::GetPortInternalValue(int port) {
    uint8 result = VZ::GetPortInternalValue(port);

    if (port == 'D') {
        if (gExternalStorage.IsMounted(EmHAL::Slot::memorystick))
            result |= 0x08;
        else
            result &= ~0x08;

        if (mb86189.GetIrq())
            result &= ~0x40;
        else
            result |= 0x40;
    }

    return result;
}

#endif  // _EM_SONY_VZ_WITH_SLOT_
