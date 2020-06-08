#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <SDL2/SDL.h>

#include "EmDevice.h"
#include "EmHAL.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "Frame.h"
#include "Logging.h"
#include "Platform.h"

using namespace std;

bool readFile(string file, unique_ptr<uint8[]>& buffer, long& len) {
    fstream stream(file, ios_base::in);
    if (stream.fail()) return false;

    stream.seekg(0, ios_base::end);
    len = stream.tellg();

    stream.seekg(0, ios_base::beg);
    buffer = make_unique<uint8[]>(len);

    stream.read((char*)buffer.get(), len);
    if (stream.gcount() != len) return false;

    return true;
}

void analyzeRom(EmROMReader& reader) {
    cout << "ROM info" << endl;
    cout << "================================================================================"
         << endl;
    cout << "Card version:          " << reader.GetCardVersion() << endl;
    cout << "Card name:             " << reader.GetCardName() << endl;
    cout << "Card manufacturer:     " << reader.GetCardManufacturer() << endl;
    cout << "Store version:         " << reader.GetStoreVersion() << endl;
    cout << "Company ID:            " << reader.GetCompanyID() << endl;
    cout << "HAL ID:                " << reader.GetHalID() << endl;
    cout << "ROM version:           " << reader.GetRomVersion() << endl;
    cout << "ROM version string:    " << reader.GetRomVersionString() << endl;

    cout << "CPU:                   ";
    if (reader.GetFlag328()) cout << "328 ";
    if (reader.GetFlagEZ()) cout << "EZ ";
    if (reader.GetFlagSZ()) cout << "SZ ";
    if (reader.GetFlagVZ()) cout << "VZ ";
    cout << endl;

    cout << "Databases:             ";
    for (auto&& database : reader.Databases()) {
        cout << database.Name() << " ";
    }
    cout << endl;

    cout << "================================================================================"
         << endl;
}

void initializeSession(string file) {
    unique_ptr<uint8[]> buffer;
    long len;

    if (!readFile(file, buffer, len)) {
        cerr << "unable to open " << file << endl;

        exit(1);
    }

    EmROMReader reader(buffer.get(), len);

    if (!reader.AcquireCardHeader() || !reader.AcquireROMHeap() || !reader.AcquireDatabases() ||
        !reader.AcquireFeatures()) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl;

        exit(1);
    }

    analyzeRom(reader);

    EmDevice* device = new EmDevice("PalmV");

    if (!device->SupportsROM(reader)) {
        cerr << "ROM not supported by Palm V" << endl;

        exit(1);
    }

    if (!gSession->Initialize(device, buffer.get(), len)) {
        cerr << "Session failed to initialize" << endl;

        exit(1);
    }
}

struct MainLoopContext {
    MainLoopContext(SDL_Renderer* renderer, SDL_Texture* texture)
        : renderer(renderer), texture(texture) {}

    SDL_Renderer* renderer{nullptr};
    SDL_Texture* texture{nullptr};
};

void mainLoop(MainLoopContext* ctx) {
    static Frame frame(1024 * 128);
    static const long millisOffset = Platform::getMilliseconds() - 10;
    static long clockEmu = 0;

    constexpr int mhz = 4;

    const long millis = Platform::getMilliseconds();
    if (millis - millisOffset - clockEmu > 500) clockEmu = millis - millisOffset - 10;

    const uint32 cycles = (millis - millisOffset - clockEmu) * 1000 * mhz;
    uint32 cyclesPassed = 0;

    while (cyclesPassed < cycles) cyclesPassed += gSession->RunEmulation(cycles);
    clockEmu += cyclesPassed / 1000 / mhz;

    bool updateScreen = EmHAL::CopyLCDFrame(frame);

    if (updateScreen && frame.lineWidth == 160 && frame.lines == 160 && frame.bpp == 1) {
        uint32* pixels;
        int pitch;
        uint8* buffer = frame.GetBuffer();

        SDL_LockTexture(ctx->texture, nullptr, (void**)&pixels, &pitch);

        for (int x = 0; x < 160; x++)
            for (int y = 0; y < 160; y++)
                pixels[y * pitch / 4 + x] =
                    ((buffer[y * frame.bytesPerLine + (x + frame.margin) / 8] &
                      (0x80 >> ((x + frame.margin) % 8))) == 0
                         ? 0xffffffff
                         : 0x000000ff);

        SDL_UnlockTexture(ctx->texture);

        SDL_RenderCopy(ctx->renderer, ctx->texture, nullptr, nullptr);
        SDL_RenderPresent(ctx->renderer);
    }
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        cerr << "usage: cloudpalm <romimage.rom>" << endl;

        exit(1);
    }

    initializeSession(argv[1]);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(480, 480, 0, &window, &renderer) != 0) {
        cerr << "unable to create SDL window: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING, 160, 160);

    MainLoopContext ctx(renderer, texture);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((em_arg_callback_func)mainLoop, &ctx, 0, true);
#else
    bool running = true;

    while (running) {
        mainLoop(&ctx);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
    }

    SDL_Quit();
#endif
}
