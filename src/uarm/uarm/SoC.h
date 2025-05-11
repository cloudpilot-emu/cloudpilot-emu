//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_H_
#define _SOC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "SoC_type.h"
#include "buffer.h"
#include "deviceType.h"
#include "keys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHAR_CTL_C -1L
#define CHAR_NONE -2L

struct SoC;
struct AudioQueue;
struct ArmCpu;
struct SyscallDispatch;
struct NAND;

typedef bool (*SdSectorR)(uint32_t secNum, void *buf);
typedef bool (*SdSectorW)(uint32_t secNum, const void *buf);

struct SoC *socInit(enum DeviceType deviceType, uint32_t ramSize, void *romData,
                    const uint32_t romSize, uint8_t *nandContent, size_t nandSize, int gdbPort,
                    uint_fast8_t socRev);

void socReset(struct SoC *soc);

uint64_t socRun(struct SoC *soc, uint64_t maxCycles, uint64_t cyclesPerSecond);
bool socRunToPaceSyscall(struct SoC *soc, uint16_t syscall, uint64_t maxCycles,
                         uint64_t cyclesPerSecond);
bool socExecuteInjected(struct SoC *soc, struct ArmCpu *cpu, uint64_t maxCycles,
                        uint64_t cyclesPerSecond);
uint64_t socGetInjectedTimeNsec(struct SoC *soc);
void socResetInjectedTimeNsec(struct SoC *soc);

uint32_t *socGetPendingFrame(struct SoC *soc);
void socResetPendingFrame(struct SoC *soc);

void socKeyDown(struct SoC *soc, enum KeyId key);
void socKeyUp(struct SoC *soc, enum KeyId key);
void socPenDown(struct SoC *soc, int x, int y);
void socPenUp(struct SoC *soc);

void socSleep(struct SoC *soc);
void socWakeup(struct SoC *soc, uint8_t wakeupSource);

// externally needed
void socExtSerialWriteChar(int ch);
int socExtSerialReadChar(void);

void socSetFramebufferDirty(struct SoC *soc);
bool socSetFramebuffer(struct SoC *soc, uint32_t start, uint32_t size);

void socSetAudioQueue(struct SoC *soc, struct AudioQueue *audioQueue);
void socSetPcmSuspended(struct SoC *soc, bool pcmSuspended);

void socSetPcmOutputEnabled(struct SoC *soc, bool pcmOutputEnabled);

struct Buffer socGetRomData(struct SoC *soc);

struct Buffer socGetNandData(struct SoC *soc);
struct Buffer socGetNandDirtyPages(struct SoC *soc);
bool socIsNandDirty(struct SoC *soc);
void socSetNandDirty(struct SoC *soc, bool isDirty);

struct Buffer socGetMemoryData(struct SoC *soc);
struct Buffer socGetMemoryDirtyPages(struct SoC *soc);

bool socSave(struct SoC *soc);
bool socLoad(struct SoC *soc, size_t savestateSize, void *savestateData);
struct Buffer socGetSavestate(struct SoC *soc);

void socSdInsert(struct SoC *soc);
bool socSdRemount(struct SoC *soc);
void socSdEject(struct SoC *soc);
bool socSdInserted(struct SoC *soc);

void socDumpMMU(struct SoC *soc);

enum DeviceType socGetDeviceType(struct SoC *soc);

struct ArmCpu *socGetCpu(struct SoC *soc);
struct SyscallDispatch *socGetSyscallDispatch(struct SoC *soc);
bool socIsPacePatched(struct SoC *soc);

uint32_t socGetRamSize(struct SoC *soc);

struct NAND *socGetNand(struct SoC *soc);

#ifdef __cplusplus
}
#endif

#endif
