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

SdlEventHandler::SdlEventHandler(struct SoC* soc, int scale,
                                 DeviceDisplayConfiguration& deviceDisplayConfiguration,
                                 Rotation rotation)
    : soc(soc), scale(scale), rotation(rotation) {
    width = deviceDisplayConfiguration.width;
    height = deviceDisplayConfiguration.height + deviceDisplayConfiguration.graffitiHeight;
}

void SdlEventHandler::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button != SDL_BUTTON_LEFT) break;
                penDown = true;
                socPenDown(soc, RotateX(event.button.x, event.button.y) / scale,
                           RotateY(event.button.x, event.button.y) / scale);

                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button != SDL_BUTTON_LEFT) break;
                penDown = false;
                socPenUp(soc);

                break;

            case SDL_MOUSEMOTION:
                if (!penDown) break;

                socPenDown(soc, RotateX(event.motion.x, event.motion.y) / scale,
                           RotateY(event.motion.x, event.motion.y) / scale);

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

            case SDL_WINDOWEVENT_EXPOSED:
                redrawRequested = true;
                break;
        }
    }
}

bool SdlEventHandler::RedrawRequested() const { return redrawRequested; }

bool SdlEventHandler::QuitRequested() const { return quitRequested; }

void SdlEventHandler::ClearRedrawRequested() { redrawRequested = false; }

void SdlEventHandler::SetRotation(Rotation rotation) { this->rotation = rotation; }

int SdlEventHandler::RotateX(int x, int y) {
    switch (rotation) {
        case Rotation::landscape_90:
            return scale * width - y;

        case Rotation::portrait_180:
            return scale * width - x;

        case Rotation::landscape_270:
            return y;

        default:
            return x;
    }
}

int SdlEventHandler::RotateY(int x, int y) {
    switch (rotation) {
        case Rotation::landscape_90:
            return x;

        case Rotation::portrait_180:
            return scale * height - y;

        case Rotation::landscape_270:
            return scale * height - x;

        default:
            return y;
    }
}
