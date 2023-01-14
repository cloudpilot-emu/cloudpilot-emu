#ifndef EmRegsLCDCtrlT2_H
#define EmRegsLCDCtrlT2_H

#include "EmHAL.h"   // EmHALHandler
#include "EmRegs.h"  // EmRegs
#include "MediaQFramebuffer.h"

#define MQ_GraphicController_T2_LCDOnBit 0x00000088
#define MQ_GraphicController_T2_Enabled 0x00000001
#define MQ_GraphicController_T2_LowRezBit 0x0000C000
#define MQ_GraphicController_T2_ColorDepthMask 0x000000F0

// Color Depth

#define MQ_GraphicController_T2_1BPP_ColorPalette 0x00000000
#define MQ_GraphicController_T2_2BPP_ColorPalette 0x00000010
#define MQ_GraphicController_T2_4BPP_ColorPalette 0x00000020
#define MQ_GraphicController_T2_8BPP_ColorPalette 0x00000030
#define MQ_GraphicController_T2_1BPP_mono_NoColorPalette \
    0x00000080  //  1-bpp monochrome graphics. Color palette is bypassed.
#define MQ_GraphicController_T2_2BPP_mono_NoColorPalette \
    0x00000090  //  2-bpp monochrome graphics. Color palette is bypassed.
#define MQ_GraphicController_T2_4BPP_mono_NoColorPalette \
    0x000000A0  //  4-bpp monochrome graphics. Color palette is bypassed.
#define MQ_GraphicController_T2_8BPP_mono_NoColorPalette \
    0x000000B0  //  8-bpp monochrome graphics. Color palette is bypassed.
#define MQ_GraphicController_T2_16BPP_NoColorPalette \
    0x000000C0  // 16-bpp (RGB565)graphics with color palette bypassed.
#define MQ_GraphicController_T2_16BPP_BGR565_NoColorPalette \
    0x000000D0  // 16-bpp (GRB565)graphics with color palette bypassed.

#include "PalmPack.h"

typedef struct {
    // CPU Control Register
    UInt32 CPUControl;           // 0x0000 : CC00
    UInt32 GraphicEngineStatus;  // 0x0004 : CC01
    UInt32 GPIOControl00;        // 0x0008 : CC02
    UInt32 GPIOControl01;        // 0x000C : CC03
    UInt32 CPUUsbHostControl;    // 0x0010 : CC04
    UInt32 GraphicsMemCompare;   // 0x0014 : CC05

    UInt8 _filler01[0x68];  // 0x0018 - 0x007F

    // MIU Interface Control Register
    UInt32 MIUIFCtrl0;  // 0x0080 : MM00
    UInt32 MIUIFCtrl1;  // 0x0084 : MM01
    UInt32 MIUIFCtrl2;  // 0x0084 : MM02

    UInt8 _filler02[0x0C];  // 0x0088 - 0x0093

    UInt32 MIUClockControl;  // 0x0094 : MM05

    UInt8 _filler03[0x64];  // 0x0098 - 0x00FF

    // Intterrupt Control Register
    UInt32 InterruptControl;       // 0x0100
    UInt32 InterruptMask;          // 0x0104
    UInt32 InterruptStatus;        // 0x0108
    UInt32 InterruptPinRawStatus;  // 0x010C

    // Unknown registers
    UInt8 _filler04[0x70];  // 0x0110 - 0x017F

    // Graphic Control Register
    UInt32 GraphicController;  // 0x0180 : GC00
    UInt32 PowerSequencing;    // 0x0184 : GC01
    UInt32 HorizontalDisplay;  // 0x0188 : GC02
    UInt32 VerticalDisplay;    // 0x018C : GC03

    UInt32 HorizontalSynchronize;  // 0x0190 : GC04
    UInt32 VerticalSynchronize;    // 0x0194 : GC05
    UInt32 HorizontalCounter;      // 0x0198 : GC06
    UInt32 VerticalCounter;        // 0x019C : GC07

    UInt32 HorizontalWindow;           // 0x01A0 : GC08
    UInt32 VerticalWindow;             // 0x01A4 : GC09
    UInt32 LineClock;                  // 0x01A8 : GC0A
    UInt32 AlternateLineClockControl;  // 0x01AC : GC0B

    UInt32 WindowStartAddress;           // 0x01B0 : GC0C
    UInt32 AlternateWindowStartAddress;  // 0x01B4 : GC0D
    UInt32 WindowStride;                 // 0x01B8 : GC0E
    UInt32 Reserved01BC;                 // 0x01BC : GC0F

    UInt32 HwrCursorPosition;   // 0x01C0 : GC10
    UInt32 HwrCursorStart;      // 0x01C4 : GC11
    UInt32 HwrCursorForeColor;  // 0x01C8 : GC12
    UInt32 HwrCursorBackColor;  // 0x01CC : GC13

    UInt32 Reserved01D0;  // 0x01D0 : GC14
    UInt32 Reserved01D4;  // 0x01D4 : GC15
    UInt32 Reserved01D8;  // 0x01D8 : GC16
    UInt32 Reserved01DC;  // 0x01DC : GC17

    UInt32 Reserved01E0;         // 0x01E0 : GC18
    UInt32 Reserved01E4;         // 0x01E4 : GC19
    UInt32 FrameCloock;          // 0x01E8 : GC1A
    UInt32 MiscellaneousSignal;  // 0x01EC : GC1B

    UInt32 HorizontalParameter;  // 0x01F0 : GC1C
    UInt32 VerticalParameter;    // 0x01F4 : GC1D
    UInt32 WindowLineStart;      // 0x01F8 : GC1E
    UInt32 CursorLineStart;      // 0x01FC : GC1F

    // Secure Digital
    UInt8 _filler05[0x80];  // 0x0200 - 0x027F

    // Synchronous Serial Controller
    UInt8 _filler06[0x80];  // 0x0280 - 0x02FF

    // Serial Peripheral Interface
    UInt8 _filler07[0x80];  // 0x0300 - 0x037F

    // Device Configration Register
    UInt32 DeviceConfig0380;  // 0x0380 : DC00
    UInt32 DeviceConfig0384;  // 0x0384 : DC01
    UInt32 DeviceConfig0388;  // 0x0388 : DC02
    UInt32 DeviceConfig038C;  // 0x038C : DC03

    UInt32 DeviceConfig0390;  // 0x0390 : DC04
    UInt32 DeviceConfig0394;  // 0x0394 : DC05
    UInt32 DeviceConfig0398;  // 0x0398 : DC06
    UInt32 DeviceConfig039C;  // 0x039C : DC07

    UInt32 DeviceConfig03A0;  // 0x03A0 : DC08
    UInt32 DeviceConfig03A4;  // 0x03A4 : DC09

    UInt8 _filler08[0x58];  // 0x03A8 - 0x03FF

    // PCI Configuration Header
    UInt8 _filler09[0x100];  // 0x0400 - 0x04FF

    // USB Host
    UInt8 _filler10[0x100];  // 0x0500 - 0x05FF

    // Flat Panel Control Register
    UInt32 FlatPanel600;  // 0x0600 : FP00
    UInt32 FlatPanel604;  // 0x0604
    UInt32 FlatPanel608;  // 0x0608
    UInt32 FlatPanel60C;  // 0x060C

    UInt32 FlatPanel610;  // 0x0610
    UInt32 FlatPanel614;  // 0x0614
    UInt32 FlatPanel618;  // 0x0618
    UInt32 FlatPanel61C;  // 0x061C

    UInt32 FlatPanel620;  // 0x0620
    UInt32 FlatPanel624;  // 0x0624
    UInt32 FlatPanel628;  // 0x0628
    UInt32 FlatPanel62C;  // 0x062C

    UInt32 FlatPanel630;  // 0x0630
    UInt32 FlatPanel634;  // 0x0634
    UInt32 FlatPanel638;  // 0x0638
    UInt32 FlatPanel63C;  // 0x063C

    UInt32 FlatPanel640;  // 0x0640
    UInt32 FlatPanel644;  // 0x0644
    UInt32 FlatPanel648;  // 0x0648
    UInt32 FlatPanel64C;  // 0x064C

    UInt32 FlatPanel650;  // 0x0650
    UInt32 FlatPanel654;  // 0x0654
    UInt32 FlatPanel658;  // 0x0658
    UInt32 FlatPanel65C;  // 0x065C

    UInt32 FlatPanel660;  // 0x0660
    UInt32 FlatPanel664;  // 0x0664
    UInt32 FlatPanel668;  // 0x0668
    UInt32 FlatPanel66C;  // 0x066C

    UInt32 FlatPanel670;  // 0x0670
    UInt32 FlatPanel674;  // 0x0674
    UInt32 FlatPanel678;  // 0x0678
    UInt32 FlatPanel67C;  // 0x067C

    UInt32 FlatPanel680;  // 0x0680
    UInt32 FlatPanel684;  // 0x0684
    UInt32 FlatPanel688;  // 0x0688
    UInt32 FlatPanel68C;  // 0x068C

    UInt32 FlatPanel690;  // 0x0690
    UInt32 FlatPanel694;  // 0x0694
    UInt32 FlatPanel698;  // 0x0698
    UInt32 FlatPanel69C;  // 0x069C

    UInt32 FlatPanel6A0;  // 0x06A0
    UInt32 FlatPanel6A4;  // 0x06A4
    UInt32 FlatPanel6A8;  // 0x06A8
    UInt32 FlatPanel6AC;  // 0x06AC

    UInt32 FlatPanel6B0;  // 0x06B0
    UInt32 FlatPanel6B4;  // 0x06B4
    UInt32 FlatPanel6B8;  // 0x06B8
    UInt32 FlatPanel6BC;  // 0x06BC

    UInt32 FlatPanel6C0;  // 0x06C0
    UInt32 FlatPanel6C4;  // 0x06C4
    UInt32 FlatPanel6C8;  // 0x06C8
    UInt32 FlatPanel6CC;  // 0x06CC

    UInt32 FlatPanel6D0;  // 0x06D0
    UInt32 FlatPanel6D4;  // 0x06D4

    UInt8 _filler11[0x128];  // 0x06D8 - 0x07FF

    // Color Palette
    UInt8 ColorPalette800[0x0400];  // 0x0800 - 0x0BFF

    // Source FIFO Space
    UInt8 SourceFIFOSpaceC00[0x400];  // 0x0C00 - 0x0FFF

    // Unknown
    UInt8 _filler12[0x80];  // 0x1000 - 0x1080

    // Video Interface Port register
    UInt8 _filler13[0x80];  // 0x1080 - 0x10FF

    // Video Interface Port Y FIFO
    UInt8 _filler14[0x100];  // 0x1100 - 0x11FF

    // Video Interface Port U FIFO
    UInt8 _filler15[0x100];  // 0x1200 - 0x12FF

    // Video Interface Port V FIFO
    UInt8 _filler16[0x100];  // 0x1300 - 0x13FF

    // Graphic Engine Primary Register
    UInt32 GraphicEngine1400;  // 0x1400 : GE00
    UInt32 GraphicEngine1404;  // 0x1404
    UInt32 GraphicEngine1408;  // 0x1408
    UInt32 GraphicEngine140C;  // 0x140C

    UInt32 GraphicEngine1410;  // 0x1410
    UInt32 GraphicEngine1414;  // 0x1414
    UInt32 GraphicEngine1418;  // 0x1418
    UInt32 GraphicEngine141C;  // 0x141C

    UInt32 GraphicEngine1420;  // 0x1420
    UInt32 GraphicEngine1424;  // 0x1424
    UInt32 GraphicEngine1428;  // 0x1428
    UInt32 GraphicEngine142C;  // 0x142C

    UInt32 GraphicEngine1430;  // 0x1430
    UInt32 GraphicEngine1434;  // 0x1434
    UInt32 GraphicEngine1438;  // 0x1438
    UInt32 GraphicEngine143C;  // 0x143C

    UInt32 GraphicEngine1440;  // 0x1440
    UInt32 GraphicEngine1444;  // 0x1444
    UInt32 GraphicEngine1448;  // 0x1448
    UInt32 GraphicEngine144C;  // 0x144C

    UInt8 _filler17[0x30];  // 0x1450 - 0x147F

    // Graphic Engine Secondary Register
    UInt32 GraphicEngine1480;  // 0x1480 : GE20
    UInt32 GraphicEngine1484;  // 0x1484
    UInt32 GraphicEngine1488;  // 0x1488
    UInt32 GraphicEngine148C;  // 0x148C

    UInt32 GraphicEngine1490;  // 0x1490
    UInt32 GraphicEngine1494;  // 0x1494
    UInt32 GraphicEngine1498;  // 0x1498
    UInt32 GraphicEngine149C;  // 0x149C

    UInt32 GraphicEngine14A0;  // 0x14A0
    UInt32 GraphicEngine14A4;  // 0x14A4
    UInt32 GraphicEngine14A8;  // 0x14A8
    UInt32 GraphicEngine14AC;  // 0x14AC

    UInt32 GraphicEngine14B0;  // 0x14B0
    UInt32 GraphicEngine14B4;  // 0x14B4
    UInt32 GraphicEngine14B8;  // 0x14B8
    UInt32 GraphicEngine14BC;  // 0x14BC

    UInt32 GraphicEngine14C0;  // 0x14C0
    UInt32 GraphicEngine14C4;  // 0x14C4
    UInt32 GraphicEngine14C8;  // 0x14C8
    UInt32 GraphicEngine14CC;  // 0x14CC

    UInt8 _filler18[0x30];  // 0x14D0 - 0x14FF

    // USB Device Space
    UInt8 USBDevice1000[0xB00];  // 0x1500 - 0x1FFF

} HwrLCDCtrlT2Type;

#include "PalmPackPop.h"

class EmRegsFrameBuffer;

class EmRegsMQLCDControlT2 : public EmRegs, public MediaQFramebuffer<EmRegsMQLCDControlT2> {
    friend MediaQFramebuffer<EmRegsMQLCDControlT2>;

   public:
    static constexpr uint32 FRAMEBUFFER_SIZE = 320 * 1024;

   public:
    EmRegsMQLCDControlT2(EmRegsFrameBuffer& framebuffer, emuptr baseRegsAddr, emuptr baseVideoAddr);
    virtual ~EmRegsMQLCDControlT2();

    // EmRegs overrides
    virtual void Initialize();
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose();

    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual void SetSubBankHandlers();
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart();
    virtual uint32 GetAddressRange();

    // EmHAL overrides
    virtual Bool GetLCDScreenOn();
    virtual Bool GetLCDBacklightOn();
    virtual Bool GetLCDHasFrame();
    virtual uint16 GetLCD2bitMapping();

    static UInt16 GetResolutionMode();

   private:
    void ColorPaletteWrite(emuptr address, int size, uint32 value);

    // These are required by MediaQFramebuffer
    uint32 GetBPP();
    uint32 GetWidth();
    uint32 GetHeight();
    uint32 GetFrameBuffer();
    uint32 GetFrameBufferSize();
    Bool GetXDoubling();
    Bool GetYDoubling();
    int16 GetPitch();
    bool FlipX() const;
    bool SwapXY() const;
    emuptr GetFrameBufferBase() const;
    uint32 DC380Read(emuptr address, int size);

    void InvalidateWrite(emuptr address, int size, uint32 value);
    void IgnoreWrite(emuptr address, int size, uint32 value);

    void PrvUpdatePalette();

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);
    UInt32 ReadLCDRegister(UInt32 reg);

   private:
    emuptr fBaseRegsAddr;
    emuptr fBaseVideoAddr;
    HwrLCDCtrlT2Type fRegs;
    uint32 palette[256];
    EmRegsFrameBuffer& framebuffer;
    bool paletteDirty{true};
};

#define MQ_LCDControllerT2_BaseAddress 0x1F000000
#define MQ_LCDControllerT2_RegsAddr 0x1F050000
#define MQ_LCDControllerT2_VideoMemStart (MQ_LCDControllerT2_BaseAddress)
#define MQ_LCDControllerT2_VideoMemSize 0x00050000  // 256K of memory for VRAM

#endif  // EmRegsLCDCtrlT2_H
