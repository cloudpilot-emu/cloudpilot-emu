#ifndef _PV_IC_
#define _PV_IC_

#include <cstdint>

class SoC;
struct ArmCpu;
struct ArmMem;

struct PvIc;

PvIc* pvIcInit(ArmCpu* cpu, ArmMem* mem);

void pvIcInt(PvIc* ic, uint8_t intNo, bool raise);

template <typename T>
void pvIcSave(struct PvIc* ic, T& savestate);

template <typename T>
void pvIcLoad(struct PvIc* ic, T& loader);

#endif  // _PV_IC_
