#include "EmRegsPLDPalmI705.h"

#undef db_addressof
#define db_addressof(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#undef INSTALL_HANDLER
#define INSTALL_HANDLER(read, write, reg)                                         \
    this->SetHandler((ReadFunction)&EmRegsPLDPalmI705::read,                      \
                     (WriteFunction)&EmRegsPLDPalmI705::write, db_addressof(reg), \
                     fRegs.reg.GetSize())

void EmRegsPLDPalmI705::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());
    }
}
// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::EmRegsPLDPalmI705
// ---------------------------------------------------------------------------

EmRegsPLDPalmI705::EmRegsPLDPalmI705(emuptr baseAddr) : fBaseAddr(baseAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::~EmRegsPLDPalmI705
// ---------------------------------------------------------------------------

EmRegsPLDPalmI705::~EmRegsPLDPalmI705(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Initialize
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Initialize(void) { EmRegs::Initialize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Dispose
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsPLDPalmI705::GetLEDState(void) {
    uint16 result = kLEDOff;

    if (fRegs.green_led) result |= kLEDGreen;

    if (fRegs.red_led) result |= kLEDRed;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, StdWriteBE, todsln3);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, green_led);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, pin_notify_enable);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, red_led);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr);
    INSTALL_HANDLER(low_battRead, StdWriteBE, low_batt);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dto3svc_irq);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dsp_irq_enable);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dsp_irq_polarity);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_progress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_prog2);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_prog_OE);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, twister_irq_wake);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, low_batt_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr_polarity);
    INSTALL_HANDLER(usb_intRead, StdWriteBE, usb_int);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, usb_int_mask);
    INSTALL_HANDLER(bullet_detRead, StdWriteBE, bullet_det);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, bullet_det_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, bullet_det_pol);
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsPLDPalmI705::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsPLDPalmI705::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::GetAddressRange(void) { return fRegs.GetSize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::low_battRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::low_battRead(emuptr address, int size) {
    uint32 reg = StdReadBE(address, size);

#define hwrPLDlow_battOn 0  // this is inverted!
#define hwrPLDlow_battOff 0x80

    // If we're reading the whole register, or if we're reading the second
    // byte, then set the bit that says that the low-battery condition is off.

    if ((size == 2) || ((size == 1) && ((address & 1) == 1))) {
        reg |= hwrPLDlow_battOff;
    }

    return reg;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::usb_intRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::usb_intRead(emuptr /*address*/, int /*size*/) {
    // this bit is low-asserted

    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::bullet_detRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::bullet_detRead(emuptr /*address*/, int /*size*/) {
    // 1 = undocked, 0 = docked

    return ~0;
}
