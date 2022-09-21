#ifndef _EM_REGS_MB86189_
#define _EM_REGS_MB86189_

#include "EmCommon.h"
#include "EmRegs.h"

class EmRegsMB86189 : public EmRegs {
   public:
    EmRegsMB86189(emuptr baseAddress);

    void Reset(Bool hardwareReset) override;

    uint8* GetRealAddress(emuptr address) override;
    emuptr GetAddressStart(void) override;
    uint32 GetAddressRange(void) override;
    void SetSubBankHandlers(void) override;

   private:
    uint32 mscmdRead(emuptr address, int size);
    uint32 mscsRead(emuptr address, int size);
    uint32 msdataRead(emuptr address, int size);
    uint32 msicsRead(emuptr address, int size);
    uint32 msppcdRead(emuptr address, int size);

    void mscmdWrite(emuptr address, int size, uint32 value);
    void mscsWrite(emuptr address, int size, uint32 value);
    void msdataWrite(emuptr address, int size, uint32 value);
    void msicsWrite(emuptr address, int size, uint32 value);
    void msppcdWrite(emuptr address, int size, uint32 value);

    uint32 stubRead(emuptr address, int size);
    void stubdWrite(emuptr address, int size, uint32 value);

    inline uint32 compositeRegisterRead(emuptr base, emuptr address, int size, uint16 target);
    inline void compositeRegisterWrite(emuptr base, emuptr address, int size, uint32 value,
                                       uint16& target);

   private:
    struct Registers {
        uint16 mscmd;
        uint16 mscs;
        uint16 msics;
        uint16 msppcd;
    };

   private:
    emuptr baseAddress;
    Registers reg;

   private:
    EmRegsMB86189(const EmRegsMB86189&) = delete;
    EmRegsMB86189(EmRegsMB86189&&) = delete;
    EmRegsMB86189& operator=(const EmRegsMB86189&) = delete;
    EmRegsMB86189& operator=(EmRegsMB86189&&) = delete;
};

#endif  // _EM_REGS_MB86189_
