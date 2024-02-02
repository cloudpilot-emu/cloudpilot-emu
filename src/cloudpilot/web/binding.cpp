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
#include "ZipfileWalker.h"

// clang-format off
#include "binding/binding.cpp"
// clang-format on
