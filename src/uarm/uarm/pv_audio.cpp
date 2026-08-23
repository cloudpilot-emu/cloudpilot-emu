#include "pv_audio.h"

#include <stdio.h>

#include <cstdint>
#include <cstdio>

#include "RAM.h"
#include "audio_queue.h"
#include "cputil.h"
#include "mem.h"
#include "pv_ic.h"
#include "savestate/savestateAll.h"

#define AUDIO_BASE 0x30000700
#define AUDIO_SIZE 12
#define AUDIO_OFFSET_CFG 0
#define AUDIO_OFFSET_BUFFER_LENGTH 4
#define AUDIO_OFFSET_BUFFER_BASE 8

#define IRQ_NO_SOUND_HALF 6
#define IRQ_NO_SOUND_FULL 7

// #define MONO

#ifdef MONO
    // mono, 44100Hz
    #define AUDIO_CFG (0x00000000 | 44100)
#else
    // stereo, 44100Hz
    #define AUDIO_CFG (0x80000000 | 44100)
#endif

#define SAVESTATE_VERSION 0

struct PvAudio {
    bool enabled{false};

    uint32_t bufferBase{0};
    uint32_t bufferLength{0};
    uint32_t offset{0};

    PvIc* ic{nullptr};
    ArmRam* ram{nullptr};
    AudioQueue* queue{nullptr};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.DoBool(enabled).Do32(bufferBase).Do32(bufferLength).Do32(offset);
    }
};

static bool pvAudioPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid access to audio interface \n");
    }

    uint32_t& value = *reinterpret_cast<uint32_t*>(buf);
    auto audio = reinterpret_cast<PvAudio*>(userData);

    switch ((pa - AUDIO_BASE) >> 2) {
        case (AUDIO_OFFSET_CFG >> 2):
            if (write) {
                if (value & 0x01) pvIcInt(audio->ic, IRQ_NO_SOUND_HALF, false);
                if (value & 0x02) pvIcInt(audio->ic, IRQ_NO_SOUND_FULL, false);
            } else {
                value = AUDIO_CFG;
            }

            break;

        case (AUDIO_OFFSET_BUFFER_LENGTH >> 2):
            if (write) {
                audio->bufferLength = value & ~0x07;
                audio->enabled = audio->bufferLength != 0;
                audio->offset = 0;
            } else {
                value = audio->bufferLength;
            }

            break;

        case (AUDIO_OFFSET_BUFFER_BASE >> 2):
            if (write) {
                audio->bufferBase = value & ~0x07;
                audio->offset = 0;
            } else {
                return false;
            }

            break;

        default:
            return false;
    }

    return true;
}

PvAudio* pvAudioInit(ArmMem* mem, ArmRam* ram, PvIc* ic) {
    auto audio = new PvAudio();

    audio->ic = ic;
    audio->ram = ram;

    memRegionAdd(mem, AUDIO_BASE, AUDIO_SIZE, pvAudioPrvMemAccessF, audio);

    return audio;
}

void pvAudioSetQueue(PvAudio* audio, AudioQueue* queue) { audio->queue = queue; }

void pvAudioPullSamples(PvAudio* audio, uint32_t count) {
    if (!audio->enabled && !audio->queue) return;

#ifdef MONO
    uint16_t* sampleBuffer = audio->enabled
                                 ? reinterpret_cast<uint16_t*>(ramResolveAddress(
                                       audio->ram, audio->bufferBase, audio->bufferLength))
                                 : nullptr;
#else
    uint32_t* sampleBuffer = audio->enabled
                                 ? reinterpret_cast<uint32_t*>(ramResolveAddress(
                                       audio->ram, audio->bufferBase, audio->bufferLength))
                                 : nullptr;
#endif

    if (!sampleBuffer) {
        for (uint32_t i = 0; i < count; i++) audioQueuePush(audio->queue, 0);
        return;
    }

#ifdef MONO
    const uint32_t bufferLengthSamples = audio->bufferLength >> 1;
#else
    const uint32_t bufferLengthSamples = audio->bufferLength >> 2;
#endif
    const uint32_t bufferLengthSamplesHalf = bufferLengthSamples >> 1;

    for (uint32_t i = 0; i < count; i++) {
#ifdef MONO
        audioQueuePush(audio->queue,
                       (sampleBuffer[audio->offset] << 16) | sampleBuffer[audio->offset]);
        audio->offset++;
#else
        audioQueuePush(audio->queue, sampleBuffer[audio->offset++]);
#endif

        if (audio->offset == bufferLengthSamplesHalf) {
            pvIcInt(audio->ic, IRQ_NO_SOUND_HALF, true);
        }

        if (audio->offset == bufferLengthSamples) {
            pvIcInt(audio->ic, IRQ_NO_SOUND_FULL, true);
            audio->offset = 0;
        }
    }
}

template <typename T>
void pvAudioSave(struct PvAudio* audio, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvAudio, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    audio->DoSaveLoad(helper);
}

template <typename T>
void pvAudioLoad(struct PvAudio* audio, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvAudio, SAVESTATE_VERSION, "pvAudio");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    audio->DoSaveLoad(helper);
}

template void pvAudioSave<Savestate<ChunkType>>(PvAudio* audio, Savestate<ChunkType>& savestate);
template void pvAudioSave<SavestateProbe<ChunkType>>(PvAudio* audio,
                                                     SavestateProbe<ChunkType>& savestate);
template void pvAudioLoad<SavestateLoader<ChunkType>>(PvAudio* audio,
                                                      SavestateLoader<ChunkType>& loader);
