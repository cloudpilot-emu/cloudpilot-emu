#ifndef _SDL_REDERER_
#define _SDL_REDERER_

#include <SDL.h>

#include "Rotation.h"
#include "SoC.h"
#include "device_configuration.h"

class SdlRenderer {
   public:
    SdlRenderer(SDL_Window* window, SoC* soc, int scale, DisplayConfiguration& displayConfiguration,
                Rotation rotation);
    ~SdlRenderer();

    void Draw(bool forceRedraw);

   private:
    void DrawSilkscreen();

   private:
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    SDL_Texture* frameTexture{nullptr};
    SDL_Texture* silkscreenTexture{nullptr};

    bool frameTextureValid{false};
    bool lcdEnabled{true};

    SoC* soc{nullptr};

    const int scale;
    Rotation rotation;
    DisplayConfiguration displayConfiguration;

   private:
    SdlRenderer();
    SdlRenderer(const SdlRenderer&);
    SdlRenderer(SdlRenderer&&);
    SdlRenderer& operator=(const SdlRenderer&);
    SdlRenderer& operator=(SdlRenderer&&);
};

#endif  // _SDL_REDERER_
