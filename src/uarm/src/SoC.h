//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_H_
#define _SOC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CHAR_CTL_C -1L
#define CHAR_NONE -2L

typedef bool (*SdSectorR)(uint32_t secNum, void *buf);
typedef bool (*SdSectorW)(uint32_t secNum, const void *buf);

struct SoC *socInit(void **romPieces, const uint32_t *romPieceSizes, uint32_t romNumPieces,
                    uint32_t sdNumSectors, SdSectorR sdR, SdSectorW sdW, FILE *nandFile,
                    int gdbPort, uint_fast8_t socRev);
void socRun(struct SoC *soc);

void socBootload(struct SoC *soc, uint32_t method, void *param);  // soc-specific

// externally needed
void socExtSerialWriteChar(int ch);
int socExtSerialReadChar(void);

#endif
