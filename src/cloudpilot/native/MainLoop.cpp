#include "MainLoop.h"

#include <SDL_image.h>

#include "Debugger.h"
#include "EmHAL.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Nibbler.h"
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

constexpr long SCREEN_REFRESH_GRACE_TIME = 10;

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

    lcdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
                                   screenDimensions.Width(), screenDimensions.Height());

    lcdTempTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                          screenDimensions.Width(), screenDimensions.Height());
}

bool MainLoop::IsRunning() const { return !eventHandler.IsQuit(); }

void MainLoop::Cycle() {
    const long millis = Platform::GetMilliseconds();
    const uint32 clocksPerSecond = gSession->GetClocksPerSecond();

    if (!gDebugger.IsStopped()) {
        if (millis - millisOffset - static_cast<long>(clockEmu) > 500)
            clockEmu = millis - millisOffset - 10;

        const long cycles = static_cast<long>(
            (static_cast<double>(millis - millisOffset) - clockEmu) * clocksPerSecond / 1000.);

        if (cycles > 0) {
            long cyclesPassed = 0;

            while (cyclesPassed < cycles && !gDebugger.IsStopped())
                cyclesPassed += gSession->RunEmulation(cycles);
            clockEmu +=
                static_cast<double>(cyclesPassed) / (static_cast<double>(clocksPerSecond) / 1000.);
        }
    } else {
        clockEmu = millis - millisOffset;
    }

    if (gSystemState.IsScreenDirty()) {
        UpdateScreen();
        gSystemState.MarkScreenClean();
    } else if (!SuspendManager::IsSuspended() && !gDebugger.IsStopped() && !gDebugger.IsStepping())
        SDL_Delay(16);

    eventHandler.HandleEvents(millis);
}

void MainLoop::LoadSilkscreen() {
    SDL_RWops* rwops = SDL_RWFromConstMem((const void*)silkscreenPng_data, silkscreenPng_len);
    SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
    SDL_RWclose(rwops);

    silkscreenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
}

void MainLoop::DrawSilkscreen(SDL_Renderer* renderer) {
    if (screenDimensions.SilkscreenHeight() == 0) return;

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

    if (gSession->IsPowerOn() && EmHAL::CopyLCDFrame(frame)) {
        if (frame.hasChanges && frame.lineWidth * frame.scaleX == screenDimensions.Width() &&
            frame.lines * frame.scaleY == screenDimensions.Height()) {
            uint32* pixels;
            int pitch;
            uint8* buffer = frame.GetBuffer();

            SDL_LockTexture(lcdTempTexture, nullptr, (void**)&pixels, &pitch);

            switch (frame.bpp) {
                case 1: {
                    Nibbler<1> nibbler;

                    for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        nibbler.reset(buffer + y * frame.bytesPerLine, frame.margin);
                        uint32* line = pixels + y * pitch / 4;

                        for (uint32 x = 0; x < frame.lineWidth; x++)
                            *(line++) = nibbler.nibble() == 0 ? BACKGROUND_COLOR : FOREGROUND_COLOR;
                    }
                } break;

                case 2: {
                    uint16 mapping = EmHAL::GetLCD2bitMapping();

                    uint32 palette[4] = {PALETTE_GRAYSCALE_16[mapping & 0x000f],
                                         PALETTE_GRAYSCALE_16[(mapping >> 4) & 0x000f],
                                         PALETTE_GRAYSCALE_16[(mapping >> 8) & 0x000f],
                                         PALETTE_GRAYSCALE_16[(mapping >> 12) & 0x000f]};

                    Nibbler<2> nibbler;

                    for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        nibbler.reset(buffer + y * frame.bytesPerLine, frame.margin);
                        uint32* line = pixels + y * pitch / 4;

                        for (uint32 x = 0; x < frame.lineWidth; x++)
                            *(line++) = palette[nibbler.nibble()];
                    }
                } break;

                case 4: {
                    Nibbler<4> nibbler;

                    for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        nibbler.reset(buffer + y * frame.bytesPerLine, frame.margin);
                        uint32* line = pixels + y * pitch / 4;

                        for (uint32 x = 0; x < frame.lineWidth; x++)
                            *(line++) = PALETTE_GRAYSCALE_16[nibbler.nibble()];
                    }
                } break;

                case 24: {
                    for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                        memcpy(pixels + y * pitch / 4,
                               buffer + y * frame.bytesPerLine + 4 * frame.margin,
                               4 * frame.lineWidth);
                    }
                } break;
            }

            SDL_UnlockTexture(lcdTempTexture);

            SDL_Rect src = {
                .x = 0,
                .y = static_cast<int32>(frame.firstDirtyLine),
                .w = static_cast<int32>(frame.lineWidth),
                .h = static_cast<int32>(frame.lastDirtyLine - frame.firstDirtyLine + 1)};

            SDL_Rect dest = {.x = 0,
                             .y = static_cast<int32>(frame.firstDirtyLine * frame.scaleY),
                             .w = static_cast<int32>(frame.lineWidth * frame.scaleX),
                             .h = static_cast<int32>(
                                 (frame.lastDirtyLine - frame.firstDirtyLine + 1) * frame.scaleY)};

            SDL_SetRenderTarget(renderer, lcdTexture);
            SDL_RenderCopy(renderer, lcdTempTexture, &src, &dest);
        }

        SDL_Rect dest = {.x = 0,
                         .y = 0,
                         .w = static_cast<int32>(scale * screenDimensions.Width()),
                         .h = static_cast<int32>(scale * screenDimensions.Height())};

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, lcdTexture, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, BACKGROUND_HUE, BACKGROUND_HUE, BACKGROUND_HUE, 0xff);

        SDL_Rect dest = {.x = 0,
                         .y = 0,
                         .w = static_cast<int32>(scale * screenDimensions.Width()),
                         .h = static_cast<int32>(scale * screenDimensions.Height())};

        SDL_RenderFillRect(renderer, &dest);
    }

    DrawSilkscreen(renderer);

    const long timestamp = Platform::GetMilliseconds();
    if (timestamp - lastScreenRefreshAt < SCREEN_REFRESH_GRACE_TIME) {
        SDL_Delay(SCREEN_REFRESH_GRACE_TIME - timestamp + lastScreenRefreshAt);
    }

    SDL_RenderPresent(renderer);

    lastScreenRefreshAt = Platform::GetMilliseconds();
}
