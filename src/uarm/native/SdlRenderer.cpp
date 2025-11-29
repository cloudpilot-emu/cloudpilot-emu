#include "SdlRenderer.h"

#include "SDL_image.h"
#include "Silkscreen.h"

namespace {
    SDL_Texture* loadSilkscreen(SDL_Renderer* renderer) {
        SDL_RWops* rwops = SDL_RWFromConstMem((const void*)silkscreenPng_data, silkscreenPng_len);
        SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
        SDL_RWclose(rwops);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);

        return texture;
    }
}  // namespace

SdlRenderer::SdlRenderer(SDL_Window* window, SoC* soc, int scale, Rotation rotation)
    : window(window), soc(soc), scale(scale), rotation(rotation) {
    deviceGetDisplayConfiguration(socGetDeviceType(soc), &displayConfiguration);

    int windowHeight, windowWidth;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    renderer = SDL_CreateRenderer(window, -1, 0);

    int renderHeight = windowHeight, renderWidth = windowWidth;
    SDL_GetRendererOutputSize(renderer, &renderWidth, &renderHeight);

    if (renderHeight != windowHeight && renderWidth != windowWidth) {
        SDL_RenderSetScale(renderer, static_cast<float>(renderWidth / windowWidth),
                           static_cast<float>(renderHeight / windowHeight));
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    frameTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                          displayConfiguration.width, displayConfiguration.height);

    if (displayConfiguration.graffitiHeight > 0) {
        silkscreenTexture = loadSilkscreen(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
    DrawSilkscreen();
    SDL_RenderPresent(renderer);
}

SdlRenderer::~SdlRenderer() {
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(silkscreenTexture);
    SDL_DestroyRenderer(renderer);
}

void SdlRenderer::Draw(bool forceRedraw) {
    const bool wasLcdEnabled = lcdEnabled;
    lcdEnabled = socLcdEnabled(soc);

    const uint32_t* frame = socGetPendingFrame(soc);
    if (!frame && !forceRedraw && lcdEnabled == wasLcdEnabled) return;

    if (frame) {
        uint8_t* pixels;
        int pitch;
        SDL_LockTexture(frameTexture, NULL, (void**)&pixels, &pitch);

        if (pitch == 4 * displayConfiguration.width) {
            memcpy(pixels, frame, 4 * displayConfiguration.width * displayConfiguration.height);
        } else {
            for (int y = 0; y < displayConfiguration.height; y++) {
                memcpy(pixels, frame, 4 * displayConfiguration.width);
                frame += displayConfiguration.width;
                pixels += pitch;
            }
        }

        SDL_UnlockTexture(frameTexture);
        frameTextureValid = true;
    }

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
    DrawSilkscreen();

    if (frameTextureValid && lcdEnabled) {
        SDL_Rect dest = {.x = 0,
                         .y = 0,
                         .w = scale * displayConfiguration.width,
                         .h = scale * displayConfiguration.height};

        SDL_Point origin;

        switch (rotation) {
            case Rotation::landscape_90:
                dest.y = scale * displayConfiguration.width;
                origin = {0, 0};
                SDL_RenderCopyEx(renderer, frameTexture, nullptr, &dest, 270, &origin,
                                 SDL_FLIP_NONE);

                break;

            case Rotation::portrait_180:
                dest.y = scale * displayConfiguration.graffitiHeight;

                SDL_RenderCopyEx(renderer, frameTexture, nullptr, &dest, 180, nullptr,
                                 SDL_FLIP_NONE);

                break;

            case Rotation::landscape_270:
                dest.x = scale * (displayConfiguration.height +
                                  displayConfiguration.graffitiHeight - displayConfiguration.width);
                dest.y = scale * displayConfiguration.width;

                origin = {scale * displayConfiguration.width, 0};

                SDL_RenderCopyEx(renderer, frameTexture, nullptr, &dest, 90, &origin,
                                 SDL_FLIP_NONE);

                break;

            default:

                SDL_RenderCopy(renderer, frameTexture, nullptr, &dest);

                break;
        }
    }

    SDL_RenderPresent(renderer);

    socResetPendingFrame(soc);
}

void SdlRenderer::DrawSilkscreen() {
    if (!silkscreenTexture) return;

    SDL_Rect dest = {.x = 0,
                     .y = 0,
                     .w = scale * displayConfiguration.width,
                     .h = scale * displayConfiguration.graffitiHeight};

    SDL_Point origin;

    switch (rotation) {
        case Rotation::landscape_90:
            dest.x = scale * displayConfiguration.height - scale * displayConfiguration.width;
            origin = {scale * displayConfiguration.width, 0};

            SDL_RenderCopyEx(renderer, silkscreenTexture, nullptr, &dest, 270, &origin,
                             SDL_FLIP_NONE);

            break;

        case Rotation::portrait_180:
            SDL_RenderCopyEx(renderer, silkscreenTexture, nullptr, &dest, 180, nullptr,
                             SDL_FLIP_NONE);

            break;

        case Rotation::landscape_270:
            dest.x = scale * displayConfiguration.graffitiHeight;
            origin = {0, 0};

            SDL_RenderCopyEx(renderer, silkscreenTexture, nullptr, &dest, 90, &origin,
                             SDL_FLIP_NONE);

            break;

        default:
            dest.y = scale * displayConfiguration.height;
            SDL_RenderCopy(renderer, silkscreenTexture, nullptr, &dest);

            break;
    }
}
