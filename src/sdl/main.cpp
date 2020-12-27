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

#include "EmCommon.h"
#include "EmDevice.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "MainLoop.h"
#include "common.h"

#ifndef __EMSCRIPTEN__
    #include "Cli.h"
#endif

using namespace std;

constexpr uint32 IMAGE_FILE_MAGIC = 0x20150103;

struct Buffer {
    Buffer() = default;
    Buffer(uint8* buffer, size_t len) : len(len), buffer(buffer) {}

    size_t len{0};
    uint8* buffer{nullptr};
};

uint32 read32LE(const uint8* buffer, size_t i) {
    return buffer[i] | (buffer[i + 1] << 8) | (buffer[i + 2] << 16) | (buffer[i + 3] << 24);
}

bool readFile(string file, unique_ptr<uint8[]>& buffer, size_t& len) {
    fstream stream(file, ios_base::in);
    if (stream.fail()) return false;

    stream.seekg(0, ios_base::end);
    len = stream.tellg();

    stream.seekg(0, ios_base::beg);
    buffer = make_unique<uint8[]>(len);

    stream.read((char*)buffer.get(), len);
    if (static_cast<size_t>(stream.gcount()) != len) return false;

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

bool tryToParseImage(uint8* buffer, size_t len, Buffer& romImage, Buffer& memoryImage) {
    if (len < 16) return false;

    if (read32LE(buffer, 0) != IMAGE_FILE_MAGIC) return false;

    uint32 romNameSize = read32LE(buffer, 4);
    uint32 romSize = read32LE(buffer, 8);
    uint32 memorySize = read32LE(buffer, 12);

    if ((romNameSize + romSize + memorySize + 16) != len) return false;

    romImage.len = romSize;
    romImage.buffer = buffer + 16 + romNameSize;

    memoryImage.len = memorySize;
    memoryImage.buffer = romImage.buffer + romSize;

    return true;
}

void setupMemoryImage(Buffer image) {
    if ((uint32)image.len != gSession->GetMemorySize()) {
        cerr << "memory image size mismatch: expected " << gSession->GetMemorySize() << " , got "
             << image.len << endl
             << flush;
    }

    memcpy(gSession->GetMemoryPtr(), image.buffer, image.len);

    cout << "loaded memory image" << endl << flush;
}

void initializeSession(string file) {
    unique_ptr<uint8[]> fileBuffer;
    size_t fileSize;

    if (!readFile(file, fileBuffer, fileSize)) {
        cerr << "unable to open " << file << endl;

        exit(1);
    }

    Buffer romImage, memoryImage;

    if (!tryToParseImage(fileBuffer.get(), fileSize, romImage, memoryImage)) {
        romImage.buffer = fileBuffer.get();
        romImage.len = fileSize;
    }

    EmROMReader reader(romImage.buffer, romImage.len);

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

    if (!gSession->Initialize(device, romImage.buffer, romImage.len)) {
        cerr << "Session failed to initialize" << endl;

        exit(1);
    }

    if (memoryImage.buffer) {
        setupMemoryImage(memoryImage);
    }
}

void loadMemoryImage(string file) {
    unique_ptr<uint8[]> buffer;
    size_t len;

    if (!readFile(file, buffer, len)) {
        cerr << "failed to read memory dump '" << file << "'" << endl << flush;
        return;
    }

    setupMemoryImage(Buffer(buffer.get(), len));

    cout << "loaded memory image from '" << file << "'" << endl << flush;
}

int main(int argc, const char** argv) {
    if (argc != 2 && argc != 3) {
        cerr << "usage: cloudpalm <romimage.rom> [memory.img]" << endl;

        exit(1);
    }

    initializeSession(argv[1]);

    if (argv[2]) {
        loadMemoryImage(argv[2]);
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

    while (mainLoop.IsRunning()) {
        mainLoop.Cycle();
        if (Cli::Execute()) break;
    };

    Cli::Stop();

    SDL_Quit();
    IMG_Quit();
#endif
}
