#include "EmRegsUsbPegN700C.h"

#include "ChunkHelper.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

#undef addressof
#undef INSTALL_HANDLER

#define addressof(x) (this->GetAddressStart() + offsetof(HwrRegsUsbPegN700C, x))

#define INSTALL_HANDLER(read, write, reg)                    \
    this->SetHandler((ReadFunction)&EmRegsUsbPegN700C::read, \
                     (WriteFunction)&EmRegsUsbPegN700C::write, addressof(reg), sizeof(fRegs.reg))

namespace {
    constexpr uint32 SAVESTATE_VERSION = 2;
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::EmRegsUSBforPegN700C
// ---------------------------------------------------------------------------

EmRegsUsbPegN700C::EmRegsUsbPegN700C(emuptr baseAddr) : fBaseAddr(baseAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::~EmRegsUSBforPegN700C
// ---------------------------------------------------------------------------

EmRegsUsbPegN700C::~EmRegsUsbPegN700C(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmRegsUSBforPegN700C::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  May also be called from the Load function
 *				to share common functionality.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmRegsUsbPegN700C::Initialize(void) {
    EmRegs::Initialize();
    Reset(true);
}

/***********************************************************************
 *
 * FUNCTION:	EmRegsUSBforPegN700C::Reset
 *
 * DESCRIPTION: Standard reset function.  Sets the sub-system to a
 *				default state.	This occurs not only on a Reset (as
 *				from the menu item), but also when the sub-system
 *				is first initialized (Reset is called after Initialize)
 *				as well as when the system is re-loaded from an
 *				insufficient session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmRegsUsbPegN700C::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(&fRegs, 0, sizeof(fRegs));
        fRegs.USB0C06 = 0xFC;
        fRegs.USB0C07 = 0x00;

        fSecondFlg = false;
        fSecondCnt = 0;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmRegsUSBforPegN700C::Dispose
 *
 * DESCRIPTION: Standard dispose function.	Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmRegsUsbPegN700C::Dispose(void) {}

void EmRegsUsbPegN700C::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsUsbPegN700C::Save(SavestateProbe& savestateProbe) { DoSave(savestateProbe); }

void EmRegsUsbPegN700C::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::regsUsbClieN700C);
    if (!chunk) {
        logging::printf("unable to restore EmRegsUsbPegN700C: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore EmRegsUsbPegN700C: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);
}

template <typename T>
void EmRegsUsbPegN700C::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsUsbClieN700C);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void EmRegsUsbPegN700C::DoSaveLoad(T& helper, uint32 version) {
    helper.Do(typename T::Pack8() << fRegs.USB0C06 << fRegs.USB0C07);

    if (version > 1) {
        helper.DoBool(fSecondFlg).Do8(fSecondCnt);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::SetSubBankHandlers
// ---------------------------------------------------------------------------
void EmRegsUsbPegN700C::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(Read, Write, dataWrite);
    INSTALL_HANDLER(Read, Write, dataRead);
    INSTALL_HANDLER(Read, Write, cmdWrite);
    INSTALL_HANDLER(Read, Write, cmdRead);
    INSTALL_HANDLER(Read, Write, USB0200);
    INSTALL_HANDLER(Read, Write, USB0201);
    INSTALL_HANDLER(Read, Write, USB0202);
    INSTALL_HANDLER(Read, Write, USB0203);
    INSTALL_HANDLER(Read, Write, USB0204);
    INSTALL_HANDLER(Read, Write, USB0205);
    INSTALL_HANDLER(Read, Write, USB0206);
    INSTALL_HANDLER(Read, Write, USB0207);
    INSTALL_HANDLER(Read, Write, USB0208);
    INSTALL_HANDLER(Read, Write, USB0209);

    INSTALL_HANDLER(Read, Write, USB0C00);
    INSTALL_HANDLER(Read, Write, USB0C01);
    INSTALL_HANDLER(Read, Write, USB0C02);
    INSTALL_HANDLER(Read, Write, USB0C03);
    INSTALL_HANDLER(Read, Write, USB0C04);
    INSTALL_HANDLER(Read, Write, USB0C05);
    INSTALL_HANDLER(Read, Write, USB0C06);
    INSTALL_HANDLER(Read, Write, USB0C07);
    INSTALL_HANDLER(Read, Write, USB0C08);
    INSTALL_HANDLER(Read, Write, USB0C09);
    INSTALL_HANDLER(Read, Write, USB0C0A);
    INSTALL_HANDLER(Read, Write, USB0C0B);
    INSTALL_HANDLER(Read, Write, USB0C0C);
    INSTALL_HANDLER(Read, Write, USB0C0D);
    INSTALL_HANDLER(Read, Write, USB0C0E);
    INSTALL_HANDLER(Read, Write, USB0C0F);

    INSTALL_HANDLER(Read, Write, _filler01);
    INSTALL_HANDLER(Read, Write, _filler02);
    INSTALL_HANDLER(Read, Write, _filler03);
    INSTALL_HANDLER(Read, Write, _filler04);
    INSTALL_HANDLER(Read, Write, _filler05);

    INSTALL_HANDLER(Read, Write, USB8000);
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsUsbPegN700C::GetRealAddress(emuptr address) {
    uint8* loc = ((uint8*)&fRegs) + (address - this->GetAddressStart());

    return loc;
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsUsbPegN700C::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsUsbPegN700C::GetAddressRange(void) { return sizeof(fRegs); }

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Write
// ---------------------------------------------------------------------------
void EmRegsUsbPegN700C::Write(emuptr address, int size, uint32 value)

{
    if (address == (GetAddressStart() + 0x0C06) && size == 2) {
        if (this->StdReadBE(address, size) == 0xFC00) value = 0x0001;
    }

    this->StdWriteBE(address, size, value);
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBforPegN700C::Read
// ---------------------------------------------------------------------------

uint32 EmRegsUsbPegN700C::Read(emuptr address, int size) {
    uint32 rstValue = this->StdReadBE(address, size);

    if (address == (GetAddressStart() + 0x0C06) && size == 2) {
        if (fSecondFlg == true) {
            switch (fSecondCnt) {
                case 0:
                    fSecondCnt = 1;
                    rstValue = 0xFC00;
                    break;

                case 1:
                    fSecondCnt = 2;
                    rstValue = 0x0001;
                    break;

                case 2:
                    fSecondCnt = 0;
                    fSecondFlg = false;
                    fRegs.USB0C06 = 0xfc;
                    rstValue = 0;

                    break;
            }
        }
        if (rstValue == 0x0000 && fSecondFlg == false) fSecondFlg = true;
    }

    return rstValue;
}
