#include "SdlAudioHandler.h"

#include <SDL.h>

#include <iostream>

#include "SoC.h"
#include "audio_queue.h"

namespace {
    extern "C" void audioCallback(void* userData, uint8_t* stream, int len) {
        reinterpret_cast<SdlAudioHandler*>(userData)->AudioCallback(stream, len);
    }
}  // namespace

SdlAudioHandler::SdlAudioHandler(SoC* soc, AudioQueue* audioQueue)
    : soc(soc), audioQueue(audioQueue) {}

void SdlAudioHandler::Start() {
    if (initialized) return;

    SDL_AudioSpec audioSpecRequested = {.freq = 44100,
                                        .format = AUDIO_S16,
                                        .channels = 2,
                                        .silence = 0,
                                        .samples = 512,
                                        .padding = 0,
                                        .size = 0,
                                        .callback = ::audioCallback,
                                        .userdata = this};

    SDL_AudioSpec audioSpecActual;

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(
        nullptr, 0, &audioSpecRequested, &audioSpecActual, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
    if (audioDevice == 0) {
        std::cout << "failed to open audio device" << std::endl;
    } else {
        std::cout << "audio running, period size " << audioSpecActual.samples << std::endl;
    }

    socSetPcmOutputEnabled(soc, true);
    SDL_PauseAudioDevice(audioDevice, 0);

    initialized = true;
}

bool SdlAudioHandler::GetAudioBackpressure() const { return audioBackpressure; }

void SdlAudioHandler::AudioCallback(uint8_t* stream, int len) {
    size_t samplesRemaining = len / 4;
    size_t samplesPending = audioQueuePendingSamples(audioQueue);

    if (audioBuffering && samplesPending > 44100 / 60 * 4) audioBuffering = false;

    if (!audioBuffering && samplesPending < static_cast<size_t>(len)) {
        audioBuffering = true;

        std::cout << "audio underrun" << std::endl;
    }

    if (!audioBackpressure && samplesPending > 44100 / 60 * 7) audioBackpressure = true;
    if (audioBackpressure && samplesPending < 44100 / 60 * 8) audioBackpressure = false;

    if (!audioBuffering) {
        samplesRemaining -=
            audioQueuePopChunk(audioQueue, reinterpret_cast<uint32_t*>(stream), samplesRemaining);
    }

    if (samplesRemaining > 0) {
        memset(stream + (len / 4 - samplesRemaining) * 4, 0, samplesRemaining * 4);
    }
}
