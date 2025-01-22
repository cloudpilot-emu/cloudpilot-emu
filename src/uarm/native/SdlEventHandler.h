#ifndef _SDL_EVENT_HANDLER_
#define _SDL_EVENT_HANDLER_

#include "Rotation.h"
#include "SoC.h"
#include "device.h"

class SdlEventHandler {
   public:
    SdlEventHandler(struct SoC* soc, int scale,
                    DeviceDisplayConfiguration& deviceDisplayConfiguration, Rotation rotation);

    void HandleEvents();

    bool RedrawRequested() const;
    bool QuitRequested() const;
    void ClearRedrawRequested();

    void SetRotation(Rotation rotation);

   private:
    int RotateX(int x, int y);
    int RotateY(int x, int y);

   private:
    struct SoC* soc;

    bool penDown{false};
    int scale{1};
    bool redrawRequested{false};
    bool quitRequested{false};

    int width;
    int height;
    Rotation rotation;

   private:
    SdlEventHandler();
    SdlEventHandler(const SdlEventHandler&);
    SdlEventHandler(SdlEventHandler&&);
    SdlEventHandler& operator=(const SdlEventHandler&);
    SdlEventHandler& operator=(SdlEventHandler&&);
};

#endif  // _SDL_EVENT_HANDLER_
