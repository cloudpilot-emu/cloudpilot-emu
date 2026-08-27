#ifndef _PV_AUDIO_H_
#define _PV_AUDIO_H_

#include <cstdint>

struct PvAudio;
struct AudioQueue;
struct PvIc;
struct ArmMem;
struct ArmRam;
struct ArmRom;

PvAudio* pvAudioInit(ArmMem* mem, ArmRam* ram, ArmRom* rom, PvIc* ic);

void pvAudioSetQueue(PvAudio* audio, AudioQueue* queue);

void pvAudioPullSamples(PvAudio* audio, uint32_t count);

template <typename T>
void pvAudioSave(struct PvAudio* audio, T& savestate);

template <typename T>
void pvAudioLoad(struct PvAudio* audio, T& loader);

#endif  // _PV_AUDIO_H_
