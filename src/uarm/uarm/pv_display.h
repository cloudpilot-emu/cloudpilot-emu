#ifndef _PV_DISPLAY_H_
#define _PV_DISPLAY_H_

#include <cstdint>

#include "memory_buffer.h"

struct PvDisplay;
struct ArmMem;
struct ArmRam;

PvDisplay* pvDisplayInit(ArmMem* mem, ArmRam* ram, MemoryBuffer* bufferClut);

bool pvDisplayRenderFramebuffer(PvDisplay* display, uint32_t* target);

bool pvIsDirty(PvDisplay* display);

void pvDisplayClearDirty(PvDisplay* display);

template <typename T>
void pvDisplaySave(PvDisplay* display, T& savestate);

template <typename T>
void pvDisplayLoad(PvDisplay* display, T& loader);

#endif  // _PV_DISPLAY_H_
