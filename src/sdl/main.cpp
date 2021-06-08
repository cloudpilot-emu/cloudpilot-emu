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
#include "ScreenDimensions.h"
#include "SessionImage.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"
#include "WebsocketClient.h"
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

#ifndef __EMSCRIPTEN__
static WebsocketClient* websocketClient = WebsocketClient::Create("localhost", "6666");

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

            case SuspendContext::Kind::networkConnect:
                websocketClient->Start();

                context.AsContextNetworkConnect().Resume();
                break;

            case SuspendContext::Kind::networkRpc: {
                {
                    auto [request, size] = context.AsContextNetworkRpc().GetRequest();

                    websocketClient->Send(request, size);

                    auto [responseBuffer, responseSize] = websocketClient->Receive();
                    cout << "received response" << endl << flush;

                    context.AsContextNetworkRpc().ReceiveResponse(responseBuffer, responseSize);

                    break;
                }
            }
        }
    }
}
#endif

int main(int argc, const char** argv) {
    switch (argc) {
        case 2:
            if (!util::initializeSession(argv[1])) exit(1);
            break;

        case 3:
            if (!util::initializeSession(argv[2], argv[1])) exit(1);
            break;

        default:
            cerr << "usage: cloudpalm [deviceId] <romimage.rom>" << endl;
            exit(1);
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

    ScreenDimensions::Kind screenDimensionsKind = gSession->GetDevice().GetScreenDimensions();
    ScreenDimensions screenDimensions(screenDimensionsKind);
    int scale = screenDimensionsKind == ScreenDimensions::screen320x320 ? 2 : 3;

    if (SDL_CreateWindowAndRenderer(
            screenDimensions.Width() * scale,
            (screenDimensions.Height() + screenDimensions.SilkscreenHeight()) * scale, 0, &window,
            &renderer) != 0) {
        cerr << "unable to create SDL window: " << SDL_GetError() << endl;
        exit(1);
    }

    MainLoop mainLoop(window, renderer, scale);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((em_arg_callback_func)MainLoop::CycleStatic, &mainLoop, 0, true);
#else
    Cli::Start();

    Feature::SetClipboardIntegration(true);
    Feature::SetNetworkRedirection(true);

    while (mainLoop.IsRunning()) {
        mainLoop.Cycle();

        if (Cli::Execute()) break;

        handleSuspend();
    };

    Cli::Stop();
    websocketClient->Stop();

    SDL_Quit();
    IMG_Quit();
#endif
}
