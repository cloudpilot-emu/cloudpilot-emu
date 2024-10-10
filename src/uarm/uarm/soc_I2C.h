//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_I2C_H_
#define _SOC_I2C_H_

#include "CPU.h"
#include "mem.h"
#include "soc_DMA.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocI2c;

enum ActionI2C {  // designed so returns can be ORRed together with good results
    i2cStart,     // no params, no returns
    i2cRestart,   // no params, no returns
    i2cStop,      // no params, no returns
    i2cTx,        // param is byte master sent, return is bool "Ack"
    i2cRx,        // param is "bool willBeAcked", return is byte slave sent
};
typedef uint_fast8_t (*I2cDeviceActionF)(void *userData, enum ActionI2C stimulus,
                                         uint_fast8_t value);

struct SocI2c *socI2cInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma,
                          uint32_t base, uint32_t irqNo);
bool socI2cDeviceAdd(struct SocI2c *i2c, I2cDeviceActionF actF, void *userData);

#ifdef __cplusplus
}
#endif

#endif
