#ifndef _MISCELLANEOUS_H_
#define _MISCELLANEOUS_H_

#include "EmCommon.h"
#include "EmStructs.h"

uint32 NextPowerOf2(uint32 x);

Bool GetSystemCallContext(emuptr, SystemCallContext&);

#endif
