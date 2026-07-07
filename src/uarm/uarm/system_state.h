#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_

#include <cstdint>

#define SYSTEM_STATE_OS_VERSION_UNDEFINED 0xffffffff

struct SystemState;

struct SystemState* createSystemState();

bool systemStateIsUiInitialized(struct SystemState* state);
void systemStateSetUiInitialized(struct SystemState* state, bool uiInitialized);

uint32_t systemStateGetOsVersion(struct SystemState* state);
void systemStateSetOsVersion(struct SystemState* state, uint32_t osVersion);

template <typename T>
void systemStateSave(struct SystemState* state, T& savestate);

template <typename T>
void systemStateLoad(struct SystemState* state, T& loader);

#endif  // _SYSTEM_STATE_H_
