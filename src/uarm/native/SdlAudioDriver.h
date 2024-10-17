#ifndef _SDL_AUDIO_DRIVER_
#define _SDL_AUDIO_DRIVER_

#include <SDL.h>

#include <atomic>

struct SoC;
struct AudioQueue;

class SdlAudioDriver {
   public:
    SdlAudioDriver(SoC* soc, AudioQueue* audioQueue);

    void Start();
    void Pause();
    bool GetAudioBackpressure() const;

    void AudioCallback(uint8_t* stream, int len);

   private:
    SoC* soc;
    AudioQueue* audioQueue;
    SDL_AudioDeviceID audioDevice;

    bool initialized{false};

    std::atomic<bool> audioBuffering{true};
    bool audioBackpressure{false};

    uint32_t bufferThresholdStart{0};
    uint32_t bufferThresholdStop{0};
    uint32_t backpressureThresholdStart{0};
    uint32_t backpressureThresholdStop{0};
};

#endif  //_SDL_AUDIO_DRIVER_