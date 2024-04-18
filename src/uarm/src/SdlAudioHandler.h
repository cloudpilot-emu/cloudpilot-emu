#ifndef _SDL_AUDIO_HANDLER_
#define _SDL_AUDIO_HANDLER_

#include <atomic>

struct SoC;
struct AudioQueue;

class SdlAudioHandler {
   public:
    SdlAudioHandler(SoC* soc, AudioQueue* audioQueue);

    void Start();
    bool GetAudioBackpressure() const;

    void AudioCallback(uint8_t* stream, int len);

   private:
    SoC* soc;
    AudioQueue* audioQueue;

    bool initialized{false};

    std::atomic<bool> audioBuffering{true};
    bool audioBackpressure{false};
};

#endif  //_SDL_AUDIO_HANDLER_