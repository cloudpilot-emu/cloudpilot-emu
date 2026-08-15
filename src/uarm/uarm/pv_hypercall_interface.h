#ifndef _PV_HYPERCALL_INTERFACE_
#define _PV_HYPERCALL_INTERFACE_

#include <cstdint>

struct PvHypercallInterface;
struct ArmCpu;

PvHypercallInterface* pvHypercallInterfaceInit(ArmCpu* cpu, uint32_t ramSize);

#endif  //  _PV_HYPERCALL_INTERFACE_
