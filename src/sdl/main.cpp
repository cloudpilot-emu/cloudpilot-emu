#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "EmCommon.h"
#include "EmDevice.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "Feature.h"
#include "MainLoop.h"
#include "SessionImage.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendManager.h"
#include "common.h"
#include "util.h"

#ifndef __EMSCRIPTEN__
    #include "Cli.h"
#endif

using namespace std;

void setupMemoryImage(void* image, size_t size) {
    if (size != static_cast<size_t>(gSession->GetMemorySize())) {
        cerr << "memory image size mismatch: expected " << gSession->GetMemorySize() << " , got "
             << size << endl
             << flush;
    }

    memcpy(gSession->GetMemoryPtr(), image, size);
}

void loadMemoryImage(string file) {
    unique_ptr<uint8[]> buffer;
    size_t len;

    if (!util::readFile(file, buffer, len)) {
        cerr << "failed to read memory dump '" << file << "'" << endl << flush;
        return;
    }

    setupMemoryImage(buffer.get(), len);

    cout << "loaded memory image from '" << file << "'" << endl << flush;
}

void loadSavestate(string file) {
    unique_ptr<uint8[]> buffer;
    size_t len;

    if (!util::readFile(file, buffer, len)) {
        cerr << "failed to read savestate '" << file << "'" << endl << flush;
        return;
    }

    if (gSession->Load(len, buffer.get())) {
        cout << "loaded savestate from '" << file << "'" << endl << flush;
    } else {
        cerr << "failed to load savestate from '" << file << "'" << endl << flush;
    }
}

#ifndef __EMSCRIPTEN__
void handleSuspend() {
    if (SuspendManager::IsSuspended()) {
        SuspendContext& context = SuspendManager::GetContext();

        switch (context.GetKind()) {
            case SuspendContext::Kind::clipboardCopy:
                SDL_SetClipboardText(context.AsContextClipboardCopy().GetClipboardContent());

                context.AsContextClipboardCopy().Resume();

                break;

            case SuspendContext::Kind::clipboardPaste: {
                const char* clipboardContent = SDL_GetClipboardText();

                context.AsContextClipboardPaste().Resume(clipboardContent ? clipboardContent : "");

                break;
            }

            case SuspendContext::Kind::networkRpc: {
                // ...

                break;
            }
        }
    }
}
#endif

int main(int argc, const char** argv) {
    if (argc != 2 && argc != 3 && argc != 4) {
        cerr << "usage: cloudpalm <romimage.rom> [memory.img] [savestate.bin]" << endl;

        exit(1);
    }

    if (!util::initializeSession(argv[1])) exit(1);

    if (argc > 2) {
        loadMemoryImage(argv[2]);
    }

    if (argc > 3) {
        loadSavestate(argv[3]);
    }

    srand(time(nullptr));

#ifdef __EMSCRIPTEN__
    EM_ASM({ module.sessionReady(); });

    SDL_setenv("SDL_EMSCRIPTEN_KEYBOARD_ELEMENT", "canvas", 1);
#endif

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    SDL_StartTextInput();

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(160 * SCALE, 220 * SCALE, 0, &window, &renderer) != 0) {
        cerr << "unable to create SDL window: " << SDL_GetError() << endl;
        exit(1);
    }

    MainLoop mainLoop(window, renderer);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((em_arg_callback_func)MainLoop::CycleStatic, &mainLoop, 0, true);
#else
    Cli::Start();

    Feature::SetClipboardIntegration(true);

    while (mainLoop.IsRunning()) {
        mainLoop.Cycle();

        if (Cli::Execute()) break;

        handleSuspend();
    };

    Cli::Stop();

    SDL_Quit();
    IMG_Quit();
#endif
}
