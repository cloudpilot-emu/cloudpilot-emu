#include "SdlEventHandler.h"

#include <SDL.h>

#include "keys.h"

namespace {
    enum KeyId mapKey(SDL_Keycode keysym) {
        switch (keysym) {
            case SDLK_F1:
                return keyIdHard1;

            case SDLK_F2:
                return keyIdHard2;

            case SDLK_F3:
                return keyIdHard3;

            case SDLK_F4:
                return keyIdHard4;

            case SDLK_LEFT:
                return keyIdLeft;

            case SDLK_RIGHT:
                return keyIdRight;

            case SDLK_UP:
                return keyIdUp;

            case SDLK_DOWN:
                return keyIdDown;

            case SDLK_RETURN:
                return keyIdSelect;

            case SDLK_ESCAPE:
                return keyIdPower;

            default:
                return keyInvalid;
        }
    }
}  // namespace

SdlEventHandler::SdlEventHandler(struct SoC* soc, int scale) : soc(soc), scale(scale) {}

void SdlEventHandler::HandleEvents() {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button != SDL_BUTTON_LEFT) break;
                penDown = true;
                socPenDown(soc, event.button.x / scale, event.button.y / scale);

                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button != SDL_BUTTON_LEFT) break;
                penDown = false;
                socPenUp(soc);

                break;

            case SDL_MOUSEMOTION:
                if (!penDown) break;
                socPenDown(soc, event.motion.x / scale, event.motion.y / scale);

                break;

            case SDL_KEYDOWN: {
                enum KeyId key = mapKey(event.key.keysym.sym);
                if (key) socKeyDown(soc, key);
                break;
            }

            case SDL_KEYUP: {
                enum KeyId key = mapKey(event.key.keysym.sym);
                if (key) socKeyUp(soc, key);
                break;
            }
        }
    }
}
