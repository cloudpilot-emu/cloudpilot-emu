#include "MainLoop.h"

#include <SDL2/SDL_image.h>

#include "EmHAL.h"
#include "EmSession.h"
#include "Silkscreen.h"
#include "common.h"

constexpr int CLOCK_DIV = 4;
constexpr uint8 SILKSCREEN_BACKGROUND_HUE = 0xbb;
constexpr uint32 BACKGROUND_HUE = 0xdd;
constexpr uint32 FOREGROUND_COLOR = 0x000000ff;
constexpr uint32 BACKGROUND_COLOR =
    0xff | (BACKGROUND_HUE << 8) | (BACKGROUND_HUE << 16) | (BACKGROUND_HUE << 24);

MainLoop::MainLoop(SDL_Window* window, SDL_Renderer* renderer) : renderer(renderer) {
    LoadSilkscreen();

    SDL_SetRenderDrawColor(renderer, 0xdd, 0xdd, 0xdd, 0xff);
    SDL_RenderClear(renderer);
    DrawSilkscreen(renderer);

    SDL_RenderPresent(renderer);

    lcdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                   160, 160);
}

bool MainLoop::IsRunning() const { return !eventHandler.IsQuit(); }

void MainLoop::Cycle() {
    const long hz = EmHAL::GetSystemClockFrequency();
    const long millis = Platform::GetMilliseconds();
    if (millis - millisOffset - clockEmu > 500) clockEmu = millis - millisOffset - 10;

    const long cycles = (millis - millisOffset - clockEmu) * (hz / 1000 / CLOCK_DIV);

    if (cycles > 0) {
        long cyclesPassed = 0;

        while (cyclesPassed < cycles) cyclesPassed += gSession->RunEmulation(cycles);
        clockEmu += cyclesPassed / (hz / 1000 / CLOCK_DIV);
    }

    UpdateScreen();
    eventHandler.HandleEvents(millis);
}

void MainLoop::CycleStatic(MainLoop* self) { self->Cycle(); }

void MainLoop::LoadSilkscreen() {
    SDL_RWops* rwops = SDL_RWFromConstMem((const void*)silkscreenPng_data, silkscreenPng_len);
    SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
    SDL_RWclose(rwops);

    silkscreenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
}

void MainLoop::DrawSilkscreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, SILKSCREEN_BACKGROUND_HUE, SILKSCREEN_BACKGROUND_HUE,
                           SILKSCREEN_BACKGROUND_HUE, 0xff);

    SDL_Rect rect = {.x = 0, .y = SCALE * 160, .w = SCALE * 160, .h = SCALE * 60};

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, silkscreenTexture, nullptr, &rect);
}

void MainLoop::UpdateScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect dest = {.x = 0, .y = 0, .w = SCALE * 160, .h = SCALE * 160};

    if (gSession->IsPowerOn() && EmHAL::CopyLCDFrame(frame)) {
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
                         ? BACKGROUND_COLOR
                         : FOREGROUND_COLOR);

        SDL_UnlockTexture(lcdTexture);

        SDL_RenderCopy(renderer, lcdTexture, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, BACKGROUND_HUE, BACKGROUND_HUE, BACKGROUND_HUE, 0xff);
        SDL_RenderFillRect(renderer, &dest);
    }

    DrawSilkscreen(renderer);

    SDL_RenderPresent(renderer);
}
