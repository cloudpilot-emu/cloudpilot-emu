#ifndef _SDL_EVENT_HANDLER_
#define _SDL_EVENT_HANDLER_

#include "SoC.h"

class SdlEventHandler {
   public:
    SdlEventHandler(struct SoC* soc, int scale);

    void HandleEvents();

    bool RedrawRequested() const;
    void ClearRedrawRequested();

   private:
    struct SoC* soc;

    bool penDown{false};
    int scale{1};
    bool redrawRequested{false};

   private:
    SdlEventHandler();
    SdlEventHandler(const SdlEventHandler&);
    SdlEventHandler(SdlEventHandler&&);
    SdlEventHandler& operator=(const SdlEventHandler&);
    SdlEventHandler& operator=(SdlEventHandler&&);
};

#endif  // _SDL_EVENT_HANDLER_
