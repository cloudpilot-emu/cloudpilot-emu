#ifndef _PXA_I2C_H_
#define _PXA_I2C_H_

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"

struct PxaI2c;

enum ActionI2C {  // designed so returns can be ORRed together with good results
    i2cStart,     // no params, no returns
    i2cRestart,   // no params, no returns
    i2cStop,      // no params, no returns
    i2cTx,        // param is byte master sent, return is bool "Ack"
    i2cRx,        // param is "bool willBeAcked", return is byte slave sent
};
typedef uint_fast8_t (*I2cDeviceActionF)(void *userData, enum ActionI2C stimulus,
                                         uint_fast8_t value);

struct PxaI2c *pxaI2cInit(struct ArmMem *physMem, struct PxaIc *ic, struct PxaDma *dma,
                          uint32_t base, uint32_t irqNo);
bool pxaI2cDeviceAdd(struct PxaI2c *i2c, I2cDeviceActionF actF, void *userData);

template <typename T>
void pxaI2cSave(struct PxaI2c *i2c, T &savestate, uint32_t index = 0);

template <typename T>
void pxaI2cLoad(struct PxaI2c *i2c, T &loader, uint32_t index = 0);

#endif  // _PXA_I2C_H_
