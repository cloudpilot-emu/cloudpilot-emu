#include "EmRegsMediaQ11xxPacifiC.h"

EmRegsMediaQ11xxPacifiC::EmRegsMediaQ11xxPacifiC(EmRegsFrameBuffer& framebuffer,
                                                 emuptr baseRegsAddr, emuptr baseVideoAddr)
    : EmRegsMediaQ11xx(framebuffer, baseRegsAddr, baseVideoAddr) {}

#define mq_addressof(x)                                                       \
    (static_cast<emuptr>(((long)fRegs.x.GetPtr()) - ((long)fRegs.GetPtr())) + \
     ((emuptr)this->GetAddressStart()))

#define INSTALL_HANDLER(read, write, reg)                          \
    this->SetHandler((ReadFunction)&EmRegsMediaQ11xxPacifiC::read, \
                     (WriteFunction)&EmRegsMediaQ11xxPacifiC::write, mq_addressof(reg), 4)

void EmRegsMediaQ11xxPacifiC::SetSubBankHandlers(void) {
    EmRegsMediaQ11xx::SetSubBankHandlers();
    INSTALL_HANDLER(ReadFPREG9, MQWrite, fpREG[0x09]);
}

uint32 EmRegsMediaQ11xxPacifiC::ReadFPREG9(emuptr address, int size) {
    return MQRead(address, size) | 0x80;
}
