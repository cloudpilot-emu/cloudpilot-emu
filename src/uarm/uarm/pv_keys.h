#ifndef _PV_KEYS_H_
#define _PV_KEYS_H_

#include "keys.h"

struct PvKeys;
struct PvIc;
struct ArmMem;

PvKeys* pvKeysInit(ArmMem* mem, PvIc* ic);

void pvKeysEngage(PvKeys* keys, KeyId key, bool down);

template <typename T>
void pvKeysSave(PvKeys* keys, T& savestate);

template <typename T>
void pvKeysLoad(PvKeys* keys, T& loader);

#endif  // _PV_KEYS_H_
