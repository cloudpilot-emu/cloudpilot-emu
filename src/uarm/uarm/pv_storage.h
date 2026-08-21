#ifndef _PF_STORAGE_
#define _PF_STORAGE_

struct PvStorage;
struct ArmMem;
struct PvIc;

PvStorage* pvStorageInit(ArmMem* mem, PvIc* ic);

#endif  // _PF_STORAGE_
