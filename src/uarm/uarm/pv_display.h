#ifndef _PV_DISPLAY_H_
#define _PV_DISPLAY_H_

#include <cstdint>

#include "memory_buffer.h"

struct PvDisplay;
struct ArmMem;
struct ArmRom;
struct ArmRam;
class SoC;

PvDisplay* pvDisplayInit(ArmMem* mem, ArmRam* ram, ArmRom* rom, SoC* soc, MemoryBuffer* bufferClut,
                         uint32_t width, uint32_t height, uint32_t density);

bool pvDisplayRenderFramebuffer(PvDisplay* display, uint32_t* target, uint32_t lowWatermark,
                                uint32_t highWatermark, uint32_t& firstDirtyLine,
                                uint32_t& lastDirtyLine);

template <typename T>
void pvDisplaySave(PvDisplay* display, T& savestate);

template <typename T>
void pvDisplayLoad(PvDisplay* display, T& loader);

#endif  // _PV_DISPLAY_H_
