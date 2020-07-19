#include "MainLoop.h"

#include <SDL2/SDL_image.h>

#include "EmHAL.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Silkscreen.h"
#include "common.h"

constexpr int CLOCK_DIV = 2;
constexpr uint8 SILKSCREEN_BACKGROUND_HUE = 0xbb;
constexpr uint32 BACKGROUND_HUE = 0xd2;
constexpr uint32 FOREGROUND_COLOR = 0x000000ff;
constexpr uint32 BACKGROUND_COLOR =
    0xff | (BACKGROUND_HUE << 8) | (BACKGROUND_HUE << 16) | (BACKGROUND_HUE << 24);

constexpr uint32 PALETTE_GRAYSCALE_16[] = {
    0xd2d2d2ff, 0xc4c4c4ff, 0xb6b6b6ff, 0xa8a8a8ff, 0x9a9a9aff, 0x8c8c8cff, 0x7e7e7eff, 0x707070ff,
    0x626262ff, 0x545454ff, 0x464646ff, 0x383838ff, 0x2a2a2aff, 0x1c1c1cff, 0x0e0e0eff, 0x000000ff};

MainLoop::MainLoop(SDL_Window* window, SDL_Renderer* renderer) : renderer(renderer) {
    LoadSilkscreen();

    SDL_SetRenderDrawColor(renderer, 0xdd, 0xdd, 0xdd, 0xff);
    SDL_RenderClear(renderer);
    DrawSilkscreen(renderer);

    SDL_RenderPresent(renderer);

    lcdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                   160, 160);

    gSession->SetClockDiv(CLOCK_DIV);
}

bool MainLoop::IsRunning() const { return !eventHandler.IsQuit(); }

void MainLoop::Cycle() {
    const long millis = Platform::GetMilliseconds();
    const uint32 clocksPerSecond = gSession->GetClocksPerSecond();

    if (millis - millisOffset - clockEmu > 500) clockEmu = millis - millisOffset - 10;

    const long cycles = (millis - millisOffset - clockEmu) * clocksPerSecond / 1000;

    if (cycles > 0) {
        long cyclesPassed = 0;

        while (cyclesPassed < cycles) cyclesPassed += gSession->RunEmulation(cycles);
        clockEmu += cyclesPassed / (clocksPerSecond / 1000);
    }

    if (gSystemState.IsScreenDirty()) {
        UpdateScreen();
        gSystemState.MarkScreenClean();
    }

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
        if (frame.lineWidth != 160 || frame.lines != 160) return;

        uint32* pixels;
        int pitch;
        uint8* buffer = frame.GetBuffer();

        SDL_LockTexture(lcdTexture, nullptr, (void**)&pixels, &pitch);

        switch (frame.bpp) {
            case 1: {
                for (int x = 0; x < 160; x++)
                    for (int y = 0; y < 160; y++)
                        pixels[y * pitch / 4 + x] =
                            ((buffer[y * frame.bytesPerLine + (x + frame.margin) / 8] &
                              (0x80 >> ((x + frame.margin) % 8))) == 0
                                 ? BACKGROUND_COLOR
                                 : FOREGROUND_COLOR);
            } break;

            case 4: {
                for (int x = 0; x < 160; x++)
                    for (int y = 0; y < 160; y++)
                        pixels[y * pitch / 4 + x] =
                            PALETTE_GRAYSCALE_16[((buffer[y * frame.bytesPerLine +
                                                          (x + frame.margin) / 2]) >>
                                                  (x % 2 ? 0 : 4)) &
                                                 0xf];
            } break;

            case 2: {
                uint16 mapping = EmHAL::GetLCD2bitMapping();

                uint32 palette[4] = {PALETTE_GRAYSCALE_16[mapping & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 4) & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 8) & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 12) & 0x000f]};

                for (int x = 0; x < 160; x++)
                    for (int y = 0; y < 160; y++)
                        pixels[y * pitch / 4 + x] =
                            palette[((buffer[y * frame.bytesPerLine + (x + frame.margin) / 4]) >>
                                     (6 - (x % 4) * 2)) &
                                    0x3];
            } break;
        }

        SDL_UnlockTexture(lcdTexture);

        SDL_RenderCopy(renderer, lcdTexture, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, BACKGROUND_HUE, BACKGROUND_HUE, BACKGROUND_HUE, 0xff);
        SDL_RenderFillRect(renderer, &dest);
    }

    DrawSilkscreen(renderer);

    SDL_RenderPresent(renderer);
}
