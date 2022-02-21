#include "EmRegsLCDCtrlT2.h"

#include "ChunkHelper.h"
#include "EmCommon.h"
#include "EmMemory.h"  // EmMemGetRealAddress
#include "EmRegsFrameBuffer.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Frame.h"
#include "Nibbler.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// Given a register (specified by its field name), return its address
// in emulated space.

#define addressof(x) (this->GetAddressStart() + offsetof(HwrLCDCtrlT2Type, x))

// Macro to help the installation of handlers for a register.

#define INSTALL_HANDLER(read, write, reg)                                         \
    this->SetHandler((ReadFunction)&EmRegsMQLCDControlT2::read,                   \
                     (WriteFunction)&EmRegsMQLCDControlT2::write, addressof(reg), \
                     sizeof(fRegs.reg))

#define READ_REGISTER_T2(reg) this->ReadLCDRegister(fRegs.reg)

namespace {
    constexpr uint16 g_highResMode = 160;
    constexpr uint32 SAVESTATE_VERSION = 1;
}  // namespace

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::EmRegsMQLCDControlT2
// ---------------------------------------------------------------------------

EmRegsMQLCDControlT2::EmRegsMQLCDControlT2(EmRegsFrameBuffer& framebuffer, emuptr baseRegsAddr,
                                           emuptr baseVideoAddr)
    : fBaseRegsAddr(baseRegsAddr),
      fBaseVideoAddr(baseVideoAddr),
      fRegs(),
      framebuffer(framebuffer) {}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::~EmRegsMQLCDControlT2
// ---------------------------------------------------------------------------

EmRegsMQLCDControlT2::~EmRegsMQLCDControlT2(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::Initialize
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::Initialize(void) {
    EmRegs::Initialize();
    paletteDirty = true;
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::Reset
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);
    paletteDirty = true;

    if (hardwareReset) {
        memset(&fRegs, 0, sizeof(fRegs));
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::Dispose
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::Dispose(void) { EmRegs::Dispose(); }

void EmRegsMQLCDControlT2::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsMQLCDControlT2::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmRegsMQLCDControlT2::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::regsMQ1168);
    if (!chunk) {
        logging::printf("unable to restore RegsMQLCDControlT2: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsMQLCDControlT2: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);

    paletteDirty = true;
}

template <typename T>
void EmRegsMQLCDControlT2::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsMQ1168);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void EmRegsMQLCDControlT2::DoSaveLoad(T& helper) {
    helper.DoBuffer(&fRegs, sizeof(fRegs));
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, StdWriteBE, CPUControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngineStatus);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOControl00);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOControl01);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, CPUUsbHostControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicsMemCompare);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler01);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, MIUIFCtrl0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MIUIFCtrl1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MIUIFCtrl2);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler02);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MIUClockControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler03);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, InterruptControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, InterruptMask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, InterruptStatus);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, InterruptPinRawStatus);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler04);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicController);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, PowerSequencing);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, HorizontalDisplay);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, VerticalDisplay);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, HorizontalSynchronize);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, VerticalSynchronize);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, HorizontalCounter);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, VerticalCounter);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, HorizontalWindow);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, VerticalWindow);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, LineClock);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, AlternateLineClockControl);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, WindowStartAddress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, AlternateWindowStartAddress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, WindowStride);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01BC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, HwrCursorPosition);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, HwrCursorStart);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, HwrCursorForeColor);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, HwrCursorBackColor);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01D0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01D4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01D8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01DC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01E0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, Reserved01E4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FrameCloock);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MiscellaneousSignal);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, HorizontalParameter);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, VerticalParameter);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, WindowLineStart);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, CursorLineStart);

    // Tenerife's 0x00000380 register is read only.
    INSTALL_HANDLER(DC380Read, IgnoreWrite, DeviceConfig0380);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig0384);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig0388);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig038C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig0390);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig0394);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig0398);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DeviceConfig039C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel600);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel604);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel608);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel60C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel610);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel614);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel618);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel61C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel620);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel624);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel628);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel62C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel630);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel634);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel638);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel63C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel640);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel644);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel648);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel64C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel650);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel654);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel658);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel65C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel660);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel664);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel668);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel66C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel670);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel674);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel678);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel67C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel680);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel684);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel688);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel68C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel690);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel694);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel698);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel69C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6A0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6A4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6A8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6AC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6B0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6B4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6B8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6BC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6C0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6C4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6C8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6CC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6D0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FlatPanel6D4);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1400);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1404);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1408);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine140C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1410);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1414);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1418);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine141C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1420);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1424);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1428);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine142C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1430);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1434);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1438);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine143C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler17);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1480);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1484);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1488);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine148C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1490);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1494);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine1498);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine149C);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14A0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14A4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14A8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14AC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14B0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14B4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14B8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14BC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14C0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14C4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14C8);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GraphicEngine14CC);

    INSTALL_HANDLER(StdReadBE, StdWriteBE, _filler18);

    INSTALL_HANDLER(StdReadBE, ColorPaletteWrite, ColorPalette800);
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsMQLCDControlT2::GetRealAddress(emuptr address) {
    return ((uint8*)&fRegs) + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsMQLCDControlT2::GetAddressStart(void) { return fBaseRegsAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsMQLCDControlT2::GetAddressRange(void) { return sizeof(fRegs); }

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsMQLCDControlT2::GetLCDScreenOn(void) {
    UInt32 GraphicController = READ_REGISTER_T2(GraphicController);
    return ((GraphicController & MQ_GraphicController_T2_Enabled) ==
            MQ_GraphicController_T2_Enabled);
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsMQLCDControlT2::GetLCDBacklightOn(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsMQLCDControlT2::GetLCDHasFrame(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    UInt32 GraphicController = READ_REGISTER_T2(GraphicController);

    int32 height = ((GraphicController & MQ_GraphicController_T2_LowRezBit) ==
                    MQ_GraphicController_T2_LowRezBit)
                       ? 240
                       : 480;

    int32 rowBytes = READ_REGISTER_T2(WindowStride) & 0x0000FFFF;
    uint32 offset = READ_REGISTER_T2(WindowStartAddress);
    emuptr baseAddr = fBaseVideoAddr + offset;

    begin = baseAddr;
    end = baseAddr + rowBytes * height;
}

uint16 EmRegsMQLCDControlT2::GetLCD2bitMapping() { return 0xfa50; }

bool EmRegsMQLCDControlT2::CopyLCDFrame(Frame& frame) {
    class Scaler1x {
       public:
        Scaler1x(uint32* buffer, uint32) : buffer(buffer) {}

        inline void draw(uint32 color) { *(buffer++) = color; }

       private:
        uint32* buffer;
    };

    class Scaler2x {
       public:
        Scaler2x(uint32* buffer, uint32 width) : buf1(buffer), buf2(buffer + width), width(width) {}

        inline void draw(uint32 color) {
            *(buf1++) = *(buf1++) = *(buf2++) = *(buf2++) = color;

            x += 2;
            if (x >= width) {
                x = 0;
                buf1 += width;
                buf2 += width;
            }
        }

       private:
        uint32* buf1;
        uint32* buf2;
        uint32 width;
        uint32 x{0};
    };

    return (READ_REGISTER_T2(GraphicController) & MQ_GraphicController_T2_LowRezBit)
               ? CopyLCDFrameWithScale<Scaler2x, 2>(frame)
               : CopyLCDFrameWithScale<Scaler1x, 1>(frame);
}

template <typename T, int scale>
bool EmRegsMQLCDControlT2::CopyLCDFrameWithScale(Frame& frame) {
    int32 bpp = GetBpp();
    int32 height = 480;
    int32 width = 320;
    int32 rowBytes = READ_REGISTER_T2(WindowStride) & 0x0000FFFF;

    if (rowBytes <= 0) return false;

    emuptr baseAddr = fBaseVideoAddr + READ_REGISTER_T2(WindowStartAddress);

    EmAssert(gSession);
    const ScreenDimensions screenDimensions(gSession->GetDevice().GetScreenDimensions());

    if (width != static_cast<int32>(screenDimensions.Width()) ||
        height != static_cast<int32>(screenDimensions.Height())) {
        return false;
    }

    frame.bpp = 24;
    frame.lineWidth = width;
    frame.lines = height;
    frame.margin = 0;
    frame.bytesPerLine = width * 3;

    if (4 * width * height > static_cast<ssize_t>(frame.GetBufferSize())) return false;

    T scaler(reinterpret_cast<uint32*>(frame.GetBuffer()), width);

    switch (bpp) {
        case 1: {
            PrvUpdatePalette();
            Nibbler<1, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr), 0);

            for (int32 y = 0; y < height / scale; y++)
                for (int32 x = 0; x < width / scale; x++) scaler.draw(palette[nibbler.nibble()]);

            break;
        }

        case 2: {
            PrvUpdatePalette();
            Nibbler<2, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr), 0);

            for (int32 y = 0; y < height / scale; y++)
                for (int32 x = 0; x < width / scale; x++) scaler.draw(palette[nibbler.nibble()]);

            break;
        }

        case 4: {
            PrvUpdatePalette();
            Nibbler<4, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr), 0);

            for (int32 y = 0; y < height / scale; y++)
                for (int32 x = 0; x < width / scale; x++) scaler.draw(palette[nibbler.nibble()]);

            break;
        }

        case 8: {
            PrvUpdatePalette();
            uint8* buffer = framebuffer.GetRealAddress(baseAddr);

            for (int32 y = 0; y < height / scale; y++)
                for (int32 x = 0; x < width / scale; x++)
                    scaler.draw(palette[*(uint8*)((long)(buffer++) ^ 1)]);

            break;
        }

        default: {
            uint8* buffer = framebuffer.GetRealAddress(baseAddr);

            for (int32 y = 0; y < height / scale; y++)
                for (int32 x = 0; x < width / scale; x++) {
                    uint8 p1 = *(uint8*)((long)(buffer++) ^ 1);  // GGGBBBBB
                    uint8 p2 = *(uint8*)((long)(buffer++) ^ 1);  // RRRRRGGG

                    // Merge the two together so that we get RRRRRGGG GGGBBBBB

                    uint16 p;

                    p = (p1 << 8) | p2;

                    // Shift the bits around, forming RRRRRrrr, GGGGGGgg, and
                    // BBBBBbbb values, where the lower-case bits are copies of
                    // the least significant bits in the upper-case bits.
                    //
                    // Note that all of this could also be done with three 64K
                    // lookup tables.  If speed is an issue, we might want to
                    // investigate that.

                    scaler.draw(0xff000000 | ((((p << 3) & 0xF8) | ((p >> 0) & 0x07)) << 16) |
                                ((((p >> 3) & 0xFC) | ((p >> 5) & 0x03)) << 8) |
                                (((p >> 8) & 0xF8) | ((p >> 11) & 0x07)));
                }

            break;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::InvalidateWrite
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::InvalidateWrite(emuptr address, int size, uint32 value) {
    this->StdWriteBE(address, size, value);
    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::IgnoreWrite
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::IgnoreWrite(emuptr address, int size, uint32 value) {
    // Ignore write value to address
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::ColorPaletteWrite
// ---------------------------------------------------------------------------

void EmRegsMQLCDControlT2::ColorPaletteWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    // Always set the vertical non-display status high since in the real
    // hardware, the ROM will check this flag in order to write the CLUT
    // registers.
    this->StdWriteBE(address, size, value);

    gSystemState.MarkScreenDirty();
    paletteDirty = true;
}

// ---------------------------------------------------------------------------
//		� EmRegsMQLCDControlT2::DC380Read
// ---------------------------------------------------------------------------

uint32 EmRegsMQLCDControlT2::DC380Read(emuptr address, int size) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    // Read Device-Configration-Register addres:0000 at always return 0.
    // Offset 0x00040380 from LCD controller register .
    return 0;
}

void EmRegsMQLCDControlT2::PrvUpdatePalette() {
    if (!paletteDirty) return;

    for (int ii = 0; ii < 256; ++ii)
        palette[ii] = 0xff000000 | (fRegs.ColorPalette800[ii * 4 + 1] << 0) |
                      (fRegs.ColorPalette800[ii * 4] << 8) |
                      (fRegs.ColorPalette800[ii * 4 + 3] << 16);

    paletteDirty = false;
}

UInt16 EmRegsMQLCDControlT2::GetResolutionMode() { return g_highResMode; }

UInt32 EmRegsMQLCDControlT2::ReadLCDRegister(UInt32 reg) {
    if (!reg) return 0;

    UInt32 upper_bit = (reg << 8) & 0xFF00FF00;
    UInt32 lower_bit = (reg >> 8) & 0x00FF00FF;

    return upper_bit + lower_bit;
}

uint8 EmRegsMQLCDControlT2::GetBpp() {
    switch (READ_REGISTER_T2(GraphicController) & MQ_GraphicController_T2_ColorDepthMask) {
        case MQ_GraphicController_T2_1BPP_ColorPalette:
            return 1;

        case MQ_GraphicController_T2_2BPP_ColorPalette:
            return 2;

        case MQ_GraphicController_T2_4BPP_ColorPalette:
            return 4;

        case MQ_GraphicController_T2_8BPP_ColorPalette:
            return 8;

        case MQ_GraphicController_T2_16BPP_NoColorPalette:
            return 16;

        default:
            return 0;
    }
}
