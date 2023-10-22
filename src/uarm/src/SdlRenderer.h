#ifndef _SDL_REDERER_
#define _SDL_REDERER_

#include <SDL.h>

#include "SoC.h"
#include "device.h"

class SdlRenderer {
   public:
    SdlRenderer(SDL_Window* window, SDL_Renderer* renderer, SoC* soc, int scale);

    void Draw();

   private:
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    SDL_Texture* frameTexture{nullptr};
    SoC* soc{nullptr};

    const int scale;
    DeviceDisplayConfiguration displayConfiguration;

   private:
    SdlRenderer();
    SdlRenderer(const SdlRenderer&);
    SdlRenderer(SdlRenderer&&);
    SdlRenderer& operator=(const SdlRenderer&);
    SdlRenderer& operator=(SdlRenderer&&);
};

#endif  // _SDL_REDERER_
