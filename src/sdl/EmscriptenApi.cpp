#include <emscripten.h>

#include "ButtonEvent.h"
#include "DbInstaller.h"
#include "EmCommon.h"
#include "EmErrCodes.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Logging.h"
#include "Savestate.h"

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
    logging::printf("installing %s...", name);

    if (DbInstaller::Install(len, buffer) == DbInstaller::Result::failure) {
        logging::printf("installation failed");
    } else {
        logging::printf("installion successful");
    }
}

extern "C" int EMSCRIPTEN_KEEPALIVE memorySize() { return (int)gSession->GetMemorySize(); }

extern "C" uint8* EMSCRIPTEN_KEEPALIVE memoryPtr() { return gSession->GetMemoryPtr(); }

extern "C" uint8* EMSCRIPTEN_KEEPALIVE dirtyPagesPtr() { return gSession->GetDirtyPagesPtr(); }

extern "C" void EMSCRIPTEN_KEEPALIVE setUserName(const char* username) {
    gSession->SetHotsyncUserName(username);
}

extern "C" const char* EMSCRIPTEN_KEEPALIVE getUserName() {
    return gSystemState.GetHotsyncUserName().c_str();
}

extern "C" int EMSCRIPTEN_KEEPALIVE uiInitialized() { return gSystemState.IsUIInitialized(); }

extern "C" void EMSCRIPTEN_KEEPALIVE reset(const char* _name) {
    string name(_name);

    if (name == "soft") gSession->Reset(EmSession::ResetType::soft);
    if (name == "noext") gSession->Reset(EmSession::ResetType::noext);
    if (name == "hard") gSession->Reset(EmSession::ResetType::hard);
}

extern "C" int EMSCRIPTEN_KEEPALIVE saveState() { return gSession->Save(); }

extern "C" void* EMSCRIPTEN_KEEPALIVE getSavestatePtr() {
    return gSession->GetSavestate().GetBuffer();
}

extern "C" size_t EMSCRIPTEN_KEEPALIVE getSavestateSize() {
    return gSession->GetSavestate().GetSize();
}

extern "C" uint8* EMSCRIPTEN_KEEPALIVE getRomPtr() {
    auto [size, image] = gSession->GetRomImage();

    return image;
}

extern "C" size_t EMSCRIPTEN_KEEPALIVE getRomSize() {
    auto [size, image] = gSession->GetRomImage();

    return size;
}

extern "C" const char* EMSCRIPTEN_KEEPALIVE getDeviceId() {
    static string id;
    id = gSession->GetDevice().GetIDString();

    return id.c_str();
}
