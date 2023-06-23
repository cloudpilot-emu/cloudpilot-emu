#ifndef _PIXELFLUT_H_
#define _PIXELFLUT_H_

#include <SDL.h>

#include <cstdint>
#include <string>

namespace pixelflut {
    struct Config {
        std::string host;
        unsigned int port;

        unsigned int offsetX;
        unsigned int offsetY;
    };

    bool IsActive();

    void Start(Config config, size_t width, size_t height);
    void Stop();

    void Update(SDL_Renderer* renderer);
}  // namespace pixelflut

#endif  //  _PIXELFLUT_H_
