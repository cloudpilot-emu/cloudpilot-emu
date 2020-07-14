#include <emscripten.h>

#include "ButtonEvent.h"
#include "EmCommon.h"
#include "EmErrCodes.h"
#include "EmFileImport.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Logging.h"

ButtonEvent::Button buttonFromId(string id) {
    if (id == "app1") return ButtonEvent::Button::app1;
    if (id == "app2") return ButtonEvent::Button::app2;
    if (id == "app3") return ButtonEvent::Button::app3;
    if (id == "app4") return ButtonEvent::Button::app4;
    if (id == "rockerUp") return ButtonEvent::Button::rockerUp;
    if (id == "rockerDown") return ButtonEvent::Button::rockerDown;
    if (id == "cradle") return ButtonEvent::Button::cradle;
    if (id == "power") return ButtonEvent::Button::power;

    return ButtonEvent::Button::invalid;
}

extern "C" void EMSCRIPTEN_KEEPALIVE buttonDown(const char* id) {
    gSession->QueueButtonEvent(ButtonEvent(buttonFromId(id), ButtonEvent::Type::press));
}

extern "C" void EMSCRIPTEN_KEEPALIVE buttonUp(const char* id) {
    gSession->QueueButtonEvent(ButtonEvent(buttonFromId(id), ButtonEvent::Type::release));
}

extern "C" void EMSCRIPTEN_KEEPALIVE installFile(uint8* buffer, size_t len, const char* name) {
    log::printf("installing %s...", name);

    Err err = EmFileImport::LoadPalmFile(buffer, len, kMethodHomebrew);

    if (err != kError_NoError) {
        log::printf("installation failed with code x%04x", err);
    } else {
        log::printf("installion successful");
    }
}

extern "C" int EMSCRIPTEN_KEEPALIVE memorySize() { return (int)gSession->GetMemorySize(); }

extern "C" uint8* EMSCRIPTEN_KEEPALIVE memoryPtr() { return gSession->GetMemoryPtr(); }

extern "C" uint8* EMSCRIPTEN_KEEPALIVE dirtyPagesPtr() { return gSession->GetDirtyPagesPtr(); }

extern "C" void EMSCRIPTEN_KEEPALIVE setUserName(const char* username) {
    gSession->SetHotsyncUserName(username);
}

extern "C" int EMSCRIPTEN_KEEPALIVE uiInitialized() { return gSystemState.IsUIInitialized(); }
