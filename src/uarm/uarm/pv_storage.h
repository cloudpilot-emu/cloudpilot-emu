#ifndef _PF_STORAGE_
#define _PF_STORAGE_

struct PvStorage;
struct ArmMem;
struct ArmRam;
struct PvIc;

PvStorage* pvStorageInit(ArmMem* mem, PvIc* ic, ArmRam* ram);

void pvStorageInsert(PvStorage* storage);
void pvStorageEject(PvStorage* storage);

template <typename T>
void pvStorageSave(struct PvStorage* storage, T& savestate);

template <typename T>
void pvStorageLoad(struct PvStorage* storage, T& loader);

#endif  // _PF_STORAGE_
