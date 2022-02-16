#ifndef _EM_REGS_USB_PEG_N700C_
#define _EM_REGS_USB_PEG_N700C_

#include "EmRegs.h"
#include "PalmPack.h"
typedef struct HwrRegsUsbPegN700C {
    UInt8 dataWrite;  // 0x0000
    UInt8 dataRead;   // 0x0001
    UInt8 cmdWrite;   // 0x0002
    UInt8 cmdRead;    // 0x0003

    UInt8 _filler01[4];  // 0x0004-0007

    UInt8 USB0008;  // 0x0008
    UInt8 USB0009;  // 0x0009

    UInt8 _filler02[0x1F6];

    UInt8 USB0200;
    UInt8 USB0201;
    UInt8 USB0202;
    UInt8 USB0203;
    UInt8 USB0204;
    UInt8 USB0205;
    UInt8 USB0206;
    UInt8 USB0207;
    UInt8 USB0208;
    UInt8 USB0209;
    UInt8 _filler03[0x6];  // 0x020A-0x020F

    UInt8 _filler04[0x9F0];  // 0x0300-0x0BFF

    UInt8 USB0C00;
    UInt8 USB0C01;
    UInt8 USB0C02;
    UInt8 USB0C03;
    UInt8 USB0C04;
    UInt8 USB0C05;
    UInt8 USB0C06;
    UInt8 USB0C07;
    UInt8 USB0C08;
    UInt8 USB0C09;
    UInt8 USB0C0A;
    UInt8 USB0C0B;
    UInt8 USB0C0C;
    UInt8 USB0C0D;
    UInt8 USB0C0E;
    UInt8 USB0C0F;

    UInt8 _filler05[0x73F0];  // 0x0C10-0x7FFF

    UInt8 USB8000[0x7FFF];
} HwrUSBforPegN700C;
#include "PalmPackPop.h"

class EmRegsUsbPegN700C : public EmRegs {
   public:
    EmRegsUsbPegN700C(emuptr);
    virtual ~EmRegsUsbPegN700C(void);

    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    uint32 Read(emuptr address, int size);
    void Write(emuptr address, int size, uint32 value);

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

    emuptr fBaseAddr;
    HwrRegsUsbPegN700C fRegs;

    bool fSecondFlg;   // for Redwood & Naples
    uint8 fSecondCnt;  // for Redwood & Naples
};

#endif  // _EM_REGS_USB_PEG_N700C_
