#ifndef _RESCHEDULE_H_
#define _RESCHEDULE_H_

#include <stdint.h>

#define RESCHEDULE_TASK_DEVICE_TIER0 0
#define RESCHEDULE_TASK_NAND 1
#define RESCHEDULE_TASK_SSP 2
#define RESCHEDULE_TASK_UART 3
#define RESCHEDULE_TASK_DMA 4

typedef void (*RescheduleCallbackT)(void* ctx, uint32_t type);

struct Reschedule {
    RescheduleCallbackT rescheduleCb;
    void* ctx;
};

#endif