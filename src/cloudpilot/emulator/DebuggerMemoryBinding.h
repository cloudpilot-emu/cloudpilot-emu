#ifndef _DEBUGGER_MEMORY_BINDING_
#define _DEBUGGER_MEMORY_BINDING_

#include "EmTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void* gDebuggerPtr;

void DbgNotifyRead8(emuptr address);
void DbgNotifyRead16(emuptr address);
void DbgNotifyRead32(emuptr address);

void DbgNotifyWrite8(emuptr address);
void DbgNotifyWrite16(emuptr address);
void DbgNotifyWrite32(emuptr address);

#ifdef __cplusplus
}
#endif

#endif  // _DEBUGGER_MEMORY_BINDING_
