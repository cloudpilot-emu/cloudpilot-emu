#include "SdlRenderer.h"

SdlRenderer::SdlRenderer(SDL_Window* window, SDL_Renderer* renderer, SoC* soc, int scale)
    : window(window), renderer(renderer), soc(soc), scale(scale) {
    deviceGetDisplayConfiguration(&displayConfiguration);

    frameTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                          displayConfiguration.width, displayConfiguration.height);
}

void SdlRenderer::Draw() {
    uint32_t* frame = socGetPendingFrame(soc);
    if (!frame) return;

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

    SDL_Rect src = {
        .x = 0, .y = 0, .w = displayConfiguration.width, .h = displayConfiguration.height};
    SDL_Rect dest = {.x = 0, .y = 0, .w = scale * src.w, .h = scale * src.h};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, frameTexture, &src, &dest);
    SDL_RenderPresent(renderer);

    socResetPendingFrame(soc);
}
