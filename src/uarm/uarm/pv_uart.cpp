#include "pv_uart.h"

#include <cstdio>

#include "mem.h"

#define UART_SIZE 4

struct PvUart {
    uint32_t base{0};

    PvUartWriteF writeF{nullptr};
    void* writeFCtx{nullptr};
};

static bool pvUartPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                void* buf) {
    if (size != 4 && size != 1) {
        fprintf(stderr, "invalid UART access\n");
        return false;
    }

    auto uart = reinterpret_cast<PvUart*>(userData);

    if (write) {
        if (!uart->writeF) return true;

        uint8_t val =
            size == 4 ? *reinterpret_cast<uint32_t*>(buf) : *reinterpret_cast<uint8_t*>(buf);

        uart->writeF(val, uart->writeFCtx);
    } else {
        if (size == 4) {
            *reinterpret_cast<uint32_t*>(buf) = 0;
        } else {
            *reinterpret_cast<uint8_t*>(buf) = 0;
        }
    }

    return true;
}

PvUart* pvUartInit(ArmMem* mem, uint32_t base) {
    auto uart = new PvUart();

    memRegionAdd(mem, base, UART_SIZE, pvUartPrvMemAccessF, uart);

    return uart;
}

void pvUartSetWriteF(PvUart* uart, PvUartWriteF writeF, void* ctx) {
    uart->writeF = writeF;
    uart->writeFCtx = ctx;
}
