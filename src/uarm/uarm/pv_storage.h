#ifndef _PF_STORAGE_
#define _PF_STORAGE_

#include "ROM.h"
struct PvStorage;
struct ArmMem;
struct ArmRam;
struct ArmRom;
struct PvIc;

PvStorage* pvStorageInit(ArmMem* mem, PvIc* ic, ArmRam* ram, ArmRom* rom);

void pvStorageInsert(PvStorage* storage);
void pvStorageEject(PvStorage* storage);

template <typename T>
void pvStorageSave(struct PvStorage* storage, T& savestate);

template <typename T>
void pvStorageLoad(struct PvStorage* storage, T& loader);

#endif  // _PF_STORAGE_
