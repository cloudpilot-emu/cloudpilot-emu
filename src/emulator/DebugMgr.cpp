#include "DebugMgr.h"

emuptr gExceptionAddress;
int gExceptionSize;
Bool gExceptionForRead;

void Debug::CheckStepSpy(emuptr writeAddress, int writeBytes) {}
