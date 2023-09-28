//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _VIRTUAL_SD_H_
#define _VIRTUAL_SD_H_

#include "SoC.h"

struct VSD;
typedef struct VSD VSD;

enum SdReplyType {
    SdReplyNone,
    SdReply48bits,         // R1,R6,R7 (have checksum), R3 (no checksum)
    SdReply48bitsAndBusy,  // R1b
    SdReply136bits,        // R2
};

enum SdDataReplyType {
    SdDataOk,
    SdDataErrWrongBlockSize,
    SdDataErrWrongCurrentState,
    SdDataErrBackingStore,
};

typedef bool (*SdSectorR)(uint32_t secNum, void *buf);
typedef bool (*SdSectorW)(uint32_t secNum, const void *buf);

struct VSD *vsdInit(SdSectorR, SdSectorW, uint32_t nSec);

enum SdReplyType vsdCommand(struct VSD *vsd, uint8_t command, uint32_t param,
                            void *replyOut /* should be big enough for any reply */);
bool vsdIsCardBusy(struct VSD *vsd);
enum SdDataReplyType vsdDataXferBlockToCard(struct VSD *vsd, const void *data, uint32_t blockSz);
enum SdDataReplyType vsdDataXferBlockFromCard(struct VSD *vsd, void *data, uint32_t blockSz);

// util
uint8_t vsdCRC7(uint8_t *data, uint32_t sz);

#endif
