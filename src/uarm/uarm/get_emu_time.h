#ifndef _GET_EMU_TIME_H_
#define _GET_EMU_TIME_H_

#include <stdint.h>

typedef uint64_t (*GetTimeF)(void* userdata);

struct GetEmuTime {
    GetTimeF getTimeF;
    void* userdata;
};

#endif  // _GET_EMU_TIME_H_