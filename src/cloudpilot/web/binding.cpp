#pragma GCC diagnostic ignored "-Wunused-function"

#include <cstddef>

#include "Cloudpilot.h"
#include "EmTransportSerialBuffer.h"
#include "Frame.h"
#include "GunzipContext.h"
#include "RomInfo.h"
#include "SessionImage.h"
#include "SkinLoader.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendContextSerialSync.h"
#include "ZipfileWalker.h"
#include "rom_info5.h"
#include "session/session_file5.h"

// clang-format off
#include "binding/binding.cpp"
// clang-format on
