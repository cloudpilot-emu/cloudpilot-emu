#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include <SDL2/SDL.h>

#include "ButtonEvent.h"
#include "Frame.h"
#include "Platform.h"

class MainLoop {
   public:
    MainLoop(SDL_Window* window, SDL_Renderer* renderer);

    bool isRunning() const;

    void cycle();

    static void cycleStatic(MainLoop* self);

   private:
    void loadSilkscreen();

    void drawSilkscreen(SDL_Renderer* renderer);

    void updateScreen();

    void handleEvents(long millis);

    bool updatePenPosition();

    void handlePenDown();

    void handlePenMove();

    void handlePenUp();

    void handleKeyDown(SDL_Event event);

    void handleButtonKey(SDL_Event event, ButtonEvent::Type type);

    void handleTextInput(SDL_Event event);

   private:
    SDL_Renderer* renderer{nullptr};
    SDL_Texture* lcdTexture{nullptr};
    SDL_Texture* silkscreenTexture{nullptr};

    Frame frame{1024 * 128};

    const long millisOffset{Platform::getMilliseconds() - 10};
    long clockEmu{0};

    bool mouseDown{false};
    long lastMouseMove{millisOffset};
    int penX{0}, penY{0};

    long running{true};
};

#endif  // _MAIN_LOOP_H_
