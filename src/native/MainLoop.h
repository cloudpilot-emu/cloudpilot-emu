#ifndef _SDL_MAIN_LOOP_H_
#define _SDL_MAIN_LOOP_H_

#include <SDL.h>

#include "ButtonEvent.h"
#include "EventHandler.h"
#include "Frame.h"
#include "Platform.h"
#include "ScreenDimensions.h"

class MainLoop {
   public:
    MainLoop(SDL_Window* window, SDL_Renderer* renderer, int scale);

    bool IsRunning() const;

    void Cycle();

   private:
    void LoadSilkscreen();

    void DrawSilkscreen(SDL_Renderer* renderer);

    void UpdateScreen();

   private:
    SDL_Renderer* renderer{nullptr};
    SDL_Texture* lcdTexture{nullptr};
    SDL_Texture* lcdTempTexture{nullptr};
    SDL_Texture* silkscreenTexture{nullptr};

    int scale{1};
    ScreenDimensions screenDimensions;
    Frame frame{320 * 480 * 4};

    const long millisOffset{Platform::GetMilliseconds()};
    double clockEmu{0};

    long lastScreenRefreshAt = 0;

    EventHandler eventHandler;
};

#endif  // _SDL_MAIN_LOOP_H_
