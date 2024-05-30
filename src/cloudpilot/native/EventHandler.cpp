#include "EventHandler.h"

#include "ButtonEvent.h"
#include "EmSession.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"

constexpr int MOUSE_MOVE_THROTTLE = 25;

EventHandler::EventHandler(int scale) : scale(scale) {}

bool EventHandler::IsQuit() const { return quit; }

bool EventHandler::HandleEvents(long millis) {
    SDL_Event event;
    bool redraw = false;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseDown = true;
                    lastMouseMove = millis;
                    UpdatePenPosition();

                    HandlePenDown();
                }

                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseDown = false;
                    lastMouseMove = millis;
                    UpdatePenPosition();

                    HandlePenUp();
                }

                break;

            case SDL_MOUSEMOTION:
                if (mouseDown && (millis - lastMouseMove > MOUSE_MOVE_THROTTLE) &&
                    UpdatePenPosition()) {
                    lastMouseMove = millis;

                    HandlePenMove();
                }

                break;

            case SDL_TEXTINPUT:
                HandleTextInput(event);
                break;

            case SDL_KEYDOWN:
                HandleKeyDown(event);
                break;

            case SDL_KEYUP:
                if ((event.key.keysym.mod & KMOD_SHIFT) && (event.key.keysym.mod & KMOD_CTRL))
                    HandleButtonKey(event, ButtonEvent::Type::release);
                break;

            case SDL_WINDOWEVENT_EXPOSED:
                redraw = true;
                break;
        }
    }

    return redraw;
}

bool EventHandler::UpdatePenPosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);

    x /= scale;
    y /= scale;

    bool changed = x != penX || y != penY;

    penX = x;
    penY = y;

    return changed;
}

void EventHandler::HandlePenDown() { gSession->QueuePenEvent(PenEvent::down(penX, penY)); }

void EventHandler::HandlePenMove() { gSession->QueuePenEvent(PenEvent::down(penX, penY)); }

void EventHandler::HandlePenUp() { gSession->QueuePenEvent(PenEvent::up()); }

void EventHandler::HandleKeyDown(SDL_Event event) {
    if ((event.key.keysym.mod & KMOD_SHIFT) && (event.key.keysym.mod & KMOD_CTRL) &&
        (!event.key.repeat))
        return HandleButtonKey(event, ButtonEvent::Type::press);

    switch (event.key.keysym.sym) {
        case SDLK_RETURN:
            gSession->QueueKeyboardEvent('\n');
            break;

        case SDLK_LEFT:
            gSession->QueueKeyboardEvent(chrLeftArrow);
            break;

        case SDLK_RIGHT:
            gSession->QueueKeyboardEvent(chrRightArrow);
            break;

        case SDLK_UP:
            gSession->QueueKeyboardEvent(chrUpArrow);
            break;

        case SDLK_DOWN:
            gSession->QueueKeyboardEvent(chrDownArrow);
            break;

        case SDLK_BACKSPACE:
            gSession->QueueKeyboardEvent(chrBackspace);
            break;

        case SDLK_TAB:
            gSession->QueueKeyboardEvent(chrHorizontalTabulation);
            break;
    }
}

void EventHandler::HandleButtonKey(SDL_Event event, ButtonEvent::Type type) {
    switch (event.key.keysym.sym) {
        case SDLK_u:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::app1, type));
            break;

        case SDLK_i:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::app2, type));
            break;

        case SDLK_o:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::app3, type));
            break;

        case SDLK_p:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::app4, type));
            break;

        case SDLK_UP:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::rockerUp, type));
            break;

        case SDLK_DOWN:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::rockerDown, type));
            break;

        case SDLK_j:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::cradle, type));
            break;

        case SDLK_k:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::contrast, type));
            break;

        case SDLK_l:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::antenna, type));
            break;

        case SDLK_m:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::power, type));
            break;

        case SDLK_w:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::wheelUp, type));
            break;

        case SDLK_s:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::wheelPush, type));
            break;

        case SDLK_x:
            gSession->QueueButtonEvent(ButtonEvent(ButtonEvent::Button::wheelDown, type));
            break;
    }
}

void EventHandler::HandleTextInput(SDL_Event event) {
    const char* text = event.text.text;
    char c = 0;

    if ((text[0] & 0x80) == 0) {
        // U+0000 -- U+007F: ASCII
        c = text[0];
    } else if ((text[0] & 0xfc) == 0xc0) {
        // U+0080 -- U+00FF: LATIN-1
        c = ((text[0] & 0x03) << 6) | (text[1] & 0x3f);
    }

    if (c) gSession->QueueKeyboardEvent(c & 0xff);
}
