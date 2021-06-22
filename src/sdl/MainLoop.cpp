#include "MainLoop.h"

#include <SDL_image.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "EmHAL.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Silkscreen.h"
#include "SuspendManager.h"

constexpr uint8 SILKSCREEN_BACKGROUND_HUE = 0xbb;
constexpr uint32 BACKGROUND_HUE = 0xd2;
constexpr uint32 FOREGROUND_COLOR = 0xff000000;
constexpr uint32 BACKGROUND_COLOR =
    0xff000000 | BACKGROUND_HUE | (BACKGROUND_HUE << 8) | (BACKGROUND_HUE << 16);

constexpr uint32 PALETTE_GRAYSCALE_16[] = {
    0xffd2d2d2, 0xffc4c4c4, 0xffb6b6b6, 0xffa8a8a8, 0xff9a9a9a, 0xff8c8c8c, 0xff7e7e7e, 0xff707070,
    0xff626262, 0xff545454, 0xff464646, 0xff383838, 0xff2a2a2a, 0xff1c1c1c, 0xff0e0e0e, 0xff000000};

MainLoop::MainLoop(SDL_Window* window, SDL_Renderer* renderer, int scale)
    : renderer(renderer),
      scale(scale),
      screenDimensions(gSession->GetDevice().GetScreenDimensions()),
      eventHandler(scale) {
    LoadSilkscreen();

    SDL_SetRenderDrawColor(renderer, 0xdd, 0xdd, 0xdd, 0xff);
    SDL_RenderClear(renderer);
    DrawSilkscreen(renderer);

    SDL_RenderPresent(renderer);

    lcdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                                   screenDimensions.Width(), screenDimensions.Height());
}

bool MainLoop::IsRunning() const { return !eventHandler.IsQuit(); }

void MainLoop::Cycle() {
    const long millis = Platform::GetMilliseconds();
    const uint32 clocksPerSecond = gSession->GetClocksPerSecond();

    if (millis - millisOffset - static_cast<long>(clockEmu) > 500)
        clockEmu = millis - millisOffset - 10;

    const long cycles = static_cast<long>((static_cast<double>(millis - millisOffset) - clockEmu) *
                                          clocksPerSecond / 1000.);

    if (cycles > 0) {
        long cyclesPassed = 0;

        while (cyclesPassed < cycles) cyclesPassed += gSession->RunEmulation(cycles);
        clockEmu +=
            static_cast<double>(cyclesPassed) / (static_cast<double>(clocksPerSecond) / 1000.);
    }

    if (gSystemState.IsScreenDirty()) {
        UpdateScreen();
        gSystemState.MarkScreenClean();
    }
#ifndef __EMSCRIPTEN__
    else if (!SuspendManager::IsSuspended())
        SDL_Delay(16);
#endif

#ifdef __EMSCRIPTEN__
    EM_ASM({ module.onCycle&& module.onCycle(); });
#endif

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

    SDL_Rect rect = {.x = 0,
                     .y = static_cast<int32>(scale * screenDimensions.Height()),
                     .w = static_cast<int32>(scale * screenDimensions.Width()),
                     .h = static_cast<int32>(scale * screenDimensions.SilkscreenHeight())};

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, silkscreenTexture, nullptr, &rect);
}

void MainLoop::UpdateScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect dest = {.x = 0,
                     .y = 0,
                     .w = static_cast<int32>(scale * screenDimensions.Width()),
                     .h = static_cast<int32>(scale * screenDimensions.Height())};

    if (gSession->IsPowerOn() && EmHAL::CopyLCDFrame(frame)) {
        if (frame.lineWidth != screenDimensions.Width() || frame.lines != screenDimensions.Height())
            return;

        uint32* pixels;
        int pitch;
        uint8* buffer = frame.GetBuffer();

        SDL_LockTexture(lcdTexture, nullptr, (void**)&pixels, &pitch);

        switch (frame.bpp) {
            case 1: {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    for (uint32 y = 0; y < frame.lines; y++)
                        pixels[y * pitch / 4 + x] =
                            ((buffer[y * frame.bytesPerLine + (x + frame.margin) / 8] &
                              (0x80 >> ((x + frame.margin) % 8))) == 0
                                 ? BACKGROUND_COLOR
                                 : FOREGROUND_COLOR);
            } break;

            case 4: {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    for (uint32 y = 0; y < frame.lines; y++)
                        pixels[y * pitch / 4 + x] =
                            PALETTE_GRAYSCALE_16[((buffer[y * frame.bytesPerLine +
                                                          (x + frame.margin) / 2]) >>
                                                  ((x + frame.margin) % 2 ? 0 : 4)) &
                                                 0xf];
            } break;

            case 2: {
                uint16 mapping = EmHAL::GetLCD2bitMapping();

                uint32 palette[4] = {PALETTE_GRAYSCALE_16[mapping & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 4) & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 8) & 0x000f],
                                     PALETTE_GRAYSCALE_16[(mapping >> 12) & 0x000f]};

                for (uint32 x = 0; x < frame.lineWidth; x++)
                    for (uint32 y = 0; y < frame.lines; y++)
                        pixels[y * pitch / 4 + x] =
                            palette[((buffer[y * frame.bytesPerLine + (x + frame.margin) / 4]) >>
                                     (6 - ((x + frame.margin) % 4) * 2)) &
                                    0x3];
            } break;

            case 24: {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    for (uint32 y = 0; y < frame.lines; y++) {
                        uint32* buffer32 = reinterpret_cast<uint32*>(buffer);

                        pixels[y * pitch / 4 + x] =
                            buffer32[x + frame.margin + (y * frame.lineWidth)];
                    }
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
