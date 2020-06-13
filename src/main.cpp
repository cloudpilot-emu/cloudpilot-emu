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
#include <SDL2/SDL_image.h>

#include "EmDevice.h"
#include "EmHAL.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "Frame.h"
#include "Logging.h"
#include "PenEvent.h"
#include "Platform.h"
#include "Silkscreen.h"

using namespace std;

constexpr int SCALE = 3;
constexpr int CLOCK_DIV = 4;
constexpr int MOUSE_MOVE_THROTTLE = 25;

class MainLoop {
   public:
    MainLoop(SDL_Window* window, SDL_Renderer* renderer) : renderer(renderer) {
        loadSilkscreen();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        drawSilkscreen(renderer);

        SDL_RenderPresent(renderer);

        lcdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_STREAMING, 160, 160);
    }

    bool isRunning() const { return running; }

    void cycle() {
        const long hz = EmHAL::GetSystemClockFrequency();
        const long millis = Platform::getMilliseconds();
        if (millis - millisOffset - clockEmu > 500) clockEmu = millis - millisOffset - 10;

        const long cycles = (millis - millisOffset - clockEmu) * (hz / 1000 / CLOCK_DIV);

        if (cycles > 0) {
            long cyclesPassed = 0;

            while (cyclesPassed < cycles) cyclesPassed += gSession->RunEmulation(cycles);
            clockEmu += cyclesPassed / (hz / 1000 / CLOCK_DIV);
        }

        updateScreen();
        handleEvents(millis);
    }

    static void cycleStatic(MainLoop* self) { self->cycle(); }

   private:
    void loadSilkscreen() {
        SDL_RWops* rwops = SDL_RWFromConstMem((const void*)silkscreenPng_data, silkscreenPng_len);
        SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
        SDL_RWclose(rwops);

        silkscreenTexture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
    }

    void drawSilkscreen(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0xbb, 0xbb, 0xbb, 0xff);

        SDL_Rect rect = {.x = 0, .y = SCALE * 160, .w = SCALE * 160, .h = SCALE * 60};

        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, silkscreenTexture, nullptr, &rect);
    }

    void updateScreen() {
        if (!EmHAL::CopyLCDFrame(frame)) return;
        if (frame.lineWidth != 160 || frame.lines != 160 || frame.bpp != 1) return;

        uint32* pixels;
        int pitch;
        uint8* buffer = frame.GetBuffer();

        SDL_LockTexture(lcdTexture, nullptr, (void**)&pixels, &pitch);

        for (int x = 0; x < 160; x++)
            for (int y = 0; y < 160; y++)
                pixels[y * pitch / 4 + x] =
                    ((buffer[y * frame.bytesPerLine + (x + frame.margin) / 8] &
                      (0x80 >> ((x + frame.margin) % 8))) == 0
                         ? 0xddddddff
                         : 0x000000ff);

        SDL_UnlockTexture(lcdTexture);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        drawSilkscreen(renderer);

        SDL_Rect dest = {.x = 0, .y = 0, .w = SCALE * 160, .h = SCALE * 160};
        SDL_RenderCopy(renderer, lcdTexture, nullptr, &dest);

        SDL_RenderPresent(renderer);
    }

    void handleEvents(long millis) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseDown = true;
                        lastMouseMove = millis;
                        updatePenPosition();

                        handlePenDown();
                    }

                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseDown = false;
                        lastMouseMove = millis;
                        updatePenPosition();

                        handlePenUp();
                    }

                    break;

                case SDL_MOUSEMOTION:
                    if (mouseDown && (millis - lastMouseMove > MOUSE_MOVE_THROTTLE) &&
                        updatePenPosition()) {
                        lastMouseMove = millis;

                        handlePenMove();
                    }

                    break;
            }
            if (event.type == SDL_QUIT) running = false;
        }
    }

    bool updatePenPosition() {
        int x, y;
        SDL_GetMouseState(&x, &y);

        x /= SCALE;
        y /= SCALE;

        bool changed = x != penX || y != penY;

        penX = x;
        penY = y;

        return changed;
    }

    void handlePenDown() { gSession->QueuePenEvent(PenEvent::down(penX, penY)); }

    void handlePenMove() { gSession->QueuePenEvent(PenEvent::down(penX, penY)); }

    void handlePenUp() {
        // gSession->QueuePenEvent(PenEvent::down(penX, penY));
        gSession->QueuePenEvent(PenEvent::up());
    }

   private:
    SDL_Renderer* renderer{nullptr};
    SDL_Texture* lcdTexture{nullptr};
    SDL_Texture* silkscreenTexture{nullptr};

    Frame frame{1024 * 128};

    const long millisOffset{Platform::getMilliseconds() - 10};
    long clockEmu{0};

    bool mouseDown{false};
    long lastMouseMove{millisOffset};
    int penX{0}, penY{0};

    long running{true};
};

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

int main(int argc, const char** argv) {
    if (argc != 2) {
        cerr << "usage: cloudpalm <romimage.rom>" << endl;

        exit(1);
    }

    initializeSession(argv[1]);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(160 * SCALE, 220 * SCALE, 0, &window, &renderer) != 0) {
        cerr << "unable to create SDL window: " << SDL_GetError() << endl;
        exit(1);
    }

    MainLoop mainLoop(window, renderer);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((em_arg_callback_func)MainLoop::cycleStatic, &mainLoop, 0, true);
#else

    while (mainLoop.isRunning()) {
        mainLoop.cycle();
    };

    SDL_Quit();
    IMG_Quit();
#endif
}
