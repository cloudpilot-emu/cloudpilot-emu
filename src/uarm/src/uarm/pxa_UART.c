//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_UART.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "util.h"

#define UART_FIFO_DEPTH 64

#define PXA_UART_SIZE 0x00010000UL

#define UART_FIFO_EMPTY 0xFF

struct UartFifo {
    uint8_t read;
    uint8_t write;
    uint16_t buf[UART_FIFO_DEPTH];
};

struct SocUart {
    struct SocIc *ic;
    uint32_t baseAddr;

    struct Reschedule reschedule;

    SocUartReadF readF;
    SocUartWriteF writeF;
    void *accessFuncsData;

    struct UartFifo TX, RX;

    uint16_t transmitShift;    // char currently "sending"
    uint16_t transmitHolding;  // holding register for no-fifo mode

    uint16_t receiveHolding;  // char just received

    uint8_t irq : 5;
    uint8_t cyclesSinceRecv : 3;

    uint8_t IER;  // interrupt enable register
    uint8_t IIR;  // interrupt information register
    uint8_t FCR;  // fifo control register
    uint8_t LCR;  // line control register
    uint8_t LSR;  // line status register
    uint8_t MCR;  // modem control register
    uint8_t MSR;  // modem status register
    uint8_t SPR;  // scratchpad register
    uint8_t DLL;  // divisor latch low
    uint8_t DLH;  // divior latch high;
    uint8_t ISR;  // infrared selection register
};

#define UART_IER_DMAE 0x80   // DMA enable
#define UART_IER_UUE 0x40    // Uart unit enable
#define UART_IER_NRZE 0x20   // NRZI enable
#define UART_IER_RTOIE 0x10  // transmit timeout interrupt enable
#define UART_IER_MIE 0x08    // modem interrupt enable
#define UART_IER_RLSE 0x04   // receiver line status interrupt enable
#define UART_IER_TIE 0x02    // transmit data request interrupt enable
#define UART_IER_RAVIE 0x01  // receiver data available interrupt enable

#define UART_IIR_FIFOES 0xC0       // fifo enable status
#define UART_IIR_TOD 0x08          // character timout interrupt pending
#define UART_IIR_RECV_ERR 0x06     // receive error(overrun, parity, framing, break)
#define UART_IIR_RECV_DATA 0x04    // receive data available
#define UART_IIR_RCV_TIMEOUT 0x0C  // receive data in buffer and been a while since we've seen more
#define UART_IIR_SEND_DATA 0x02    // transmit fifo requests data
#define UART_IIR_MODEM_STAT 0x00   // modem lines changed state(CTS, DSR, DI, DCD)
#define UART_IIR_NOINT 0x01        // no interrupt pending

#define UART_FCR_ITL_MASK 0xC0  // mask for ITL part of FCR
#define UART_FCR_ITL_1 0x00     // interrupt when >=1 byte in recv fifo
#define UART_FCR_ITL_8 0x40     // interrupt when >=8 byte in recv fifo
#define UART_FCR_ITL_16 0x80    // interrupt when >=16 byte in recv fifo
#define UART_FCR_ITL_32 0xC0    // interrupt when >=32 byte in recv fifo
#define UART_FCR_RESETTF 0x04   // reset tranmitter fifo
#define UART_FCR_RESETRF 0x02   // reset receiver fifo
#define UART_FCR_TRFIFOE 0x01   // transmit and receive fifo enable

#define UART_LCR_DLAB 0x80      // divisor latch access bit
#define UART_LCR_SB 0x40        // send break
#define UART_LCR_STKYP 0x20     // sticky parity (send parity bit but dont care what value)
#define UART_LCR_EPS 0x10       // even parity select
#define UART_LCR_PEN 0x08       // parity enable
#define UART_LCR_STB 0x04       // stop bits (1 = 2, 0 = 1)
#define UART_LCR_WLS_MASK 0x03  // mask for WLS values
#define UART_LCR_WLS_8 0x03     // 8 bit words
#define UART_LCR_WLS_7 0x02     // 7 bit words
#define UART_LCR_WLS_6 0x01     // 6 bit words
#define UART_LCR_WLS_5 0x00     // 5 bit words

#define UART_LSR_FIFOE 0x80  // fifo contails an error (framing, parity, or break)
#define UART_LSR_TEMT \
    0x40  // tranmitter empty (shift reg is empty and no more byte sin fifo/no byte in holding reg)
#define UART_LSR_TDRQ 0x20  // transmitter data request (see docs)
#define UART_LSR_BI \
    0x10  // send when char at front of fifo (or in holding reg) was a break char (chr reads as zero
          // by itself)
#define UART_LSR_FE 0x08  // same as above, but for framing errors
#define UART_LSR_PE 0x04  // dame as above, but for parity errors
#define UART_LSR_OE 0x02  // recv fifo overran
#define UART_LSR_DR 0x01  // byte received

#define UART_MCR_LOOP 0x10  // loop modem control lines (not full loopback)
#define UART_MCR_OUT2 0x08  // when loop is 0 enables or disables UART interrupts
#define UART_MCR_OUT1 0x04  // force RI to 1
#define UART_MCR_RTS 0x02   // 1 -> nRTS is 0
#define UART_MCR_DTR 0x01   // 0 -> nDTR is 0

#define UART_MSR_DCD 0x80
#define UART_MSR_RI 0x40
#define UART_MSR_DSR 0x20
#define UART_MSR_CTS 0x10
#define UART_MSR_DDCD 0x08  // dcd changed since last read
#define UART_MSR_TERI 0x04  // ri has changed from 0 to 1 since last read
#define UART_MSR_DDSR 0x02  // dsr changed since last read
#define UART_MSR_DCTS 0x01  // cts changed since last read

static void socUartPrvRecalc(struct SocUart *uart);

static void socUartPrvIrq(struct SocUart *uart, bool raise) {
    socIcInt(uart->ic, uart->irq,
             !(uart->MCR & UART_MCR_LOOP) && (uart->MCR & UART_MCR_OUT2) &&
                 raise /* only raise if ints are enabled */);
}

static uint_fast16_t socUartPrvDefaultRead(
    void *userData)  // these are special funcs since they always get their own userData - the uart
                     // pointer :)
{
    (void)userData;

    return UART_CHAR_NONE;  // we read nothing..as always
}

static void socUartPrvDefaultWrite(uint_fast16_t chr,
                                   void *userData)  // these are special funcs since they always get
                                                    // their own userData - the uart pointer :)
{
    (void)chr;
    (void)userData;

    // nothing to do here
}

static uint16_t socUartPrvGetchar(struct SocUart *uart) {
    SocUartReadF func = uart->readF;

    void *data = (func == socUartPrvDefaultRead) ? uart : uart->accessFuncsData;

    return func(data);
}

static void socUartPrvPutchar(struct SocUart *uart, uint_fast16_t chr) {
    SocUartWriteF func = uart->writeF;
    void *data = (func == socUartPrvDefaultWrite) ? uart : uart->accessFuncsData;

    func(chr, data);
}

static uint_fast8_t socUartPrvFifoUsed(struct UartFifo *fifo)  // return num spots used
{
    uint_fast8_t v;

    if (fifo->read == UART_FIFO_EMPTY) return 0;

    v = fifo->write + UART_FIFO_DEPTH - fifo->read;

    if (v > UART_FIFO_DEPTH) v -= UART_FIFO_DEPTH;

    return v;
}

static void socUartPrvFifoFlush(struct UartFifo *fifo) {
    fifo->read = UART_FIFO_EMPTY;
    fifo->write = UART_FIFO_EMPTY;
}

static bool socUartPrvFifoPut(struct UartFifo *fifo, uint_fast16_t val)  // return success
{
    if (fifo->read == UART_FIFO_EMPTY) {
        fifo->read = 0;
        fifo->write = 1;
        fifo->buf[0] = val;
    } else if (fifo->read != fifo->write) {  // only if not full

        fifo->buf[fifo->write++] = val;
        if (fifo->write == UART_FIFO_DEPTH) fifo->write = 0;
    } else
        return false;

    return true;
}

static uint_fast16_t socUartPrvFifoGet(struct UartFifo *fifo) {
    uint_fast16_t ret;

    if (fifo->read == UART_FIFO_EMPTY) {
        ret = 0xFFFF;  // why not?
    } else {
        ret = fifo->buf[fifo->read++];
        if (fifo->read == UART_FIFO_DEPTH) fifo->read = 0;

        if (fifo->read == fifo->write) {  // it is now empty

            fifo->read = UART_FIFO_EMPTY;
            fifo->write = UART_FIFO_EMPTY;
        }
    }
    return ret;
}

static uint_fast16_t socUartPrvFifoPeekNth(struct UartFifo *fifo, uint_fast8_t n) {
    uint_fast16_t ret;

    if (fifo->read == UART_FIFO_EMPTY) {
        ret = 0xFFFF;  // why not?
    } else {
        n += fifo->read;
        if (n >= UART_FIFO_DEPTH) n -= UART_FIFO_DEPTH;
        ret = fifo->buf[n];
    }
    return ret;
}

static uint_fast16_t socUartPrvFifoPeek(struct UartFifo *fifo) {
    return socUartPrvFifoPeekNth(fifo, 0);
}

static void socUartPrvSendChar(struct SocUart *uart, uint_fast16_t v) {
    if (uart->LSR & UART_LSR_TEMT) {  // if transmit, put in shift register immediately if it's idle

        uart->transmitShift = v;
        uart->LSR &= ~UART_LSR_TEMT;
        uart->reschedule.rescheduleCb(uart->reschedule.ctx, RESCHEDULE_TASK_UART);
    } else if (uart->FCR & UART_FCR_TRFIFOE) {  // put in tx fifo if in fifo mode

        socUartPrvFifoPut(&uart->TX, v);
        if (socUartPrvFifoUsed(&uart->TX) >
            UART_FIFO_DEPTH / 2)  // we go went below half-full buffer - set appropriate bit...
            uart->LSR &= ~UART_LSR_TDRQ;
    } else if (uart->LSR & UART_LSR_TDRQ) {  // send without fifo if in polled mode

        uart->transmitHolding = v;
        uart->LSR &= ~UART_LSR_TDRQ;
    } else {
        // nothing to do - buffer is full so we ignore this request
    }
}

static bool socUartPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void *buf) {
    struct SocUart *uart = (struct SocUart *)userData;
    bool DLAB = (uart->LCR & UART_LCR_DLAB) != 0;
    bool recalcValues = false;
    uint_fast8_t t, val = 0;

    if (size != 4 && size != 1) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return true;
    }

    pa = (pa - uart->baseAddr) >> 2;

    if (write) {
        recalcValues = true;
        val = (size == 1) ? *(uint8_t *)buf : *(uint32_t *)buf;

        switch (pa) {
            case 0:
                if (DLAB) {  // if DLAB - set "baudrate"...
                    uart->DLL = val;
                    recalcValues = false;
                } else {
                    socUartPrvSendChar(uart, val);
                }
                break;

            case 1:
                if (DLAB) {
                    uart->DLH = val;
                    recalcValues = false;
                } else {
                    t = uart->IER ^ val;

                    if (t & UART_IER_DMAE) {
                        // we could support this...later
                        ERR("pxaUART: DMA mode cannot be enabled");
                        t &= ~UART_IER_DMAE;  // undo the change
                    }

                    if (t & UART_IER_UUE) {
                        if (val & UART_IER_UUE) {
                            uart->LSR = UART_LSR_TEMT | UART_LSR_TDRQ;
                            uart->MSR = UART_MSR_CTS;
                        }
                    }
                    uart->IER ^= t;
                }
                break;

            case 2:
                t = uart->FCR ^ val;
                if (t & UART_FCR_TRFIFOE) {
                    if (val & UART_FCR_TRFIFOE) {  // fifos are now on - perform other actions as
                                                   // requested

                        if (val & UART_FCR_RESETRF) {
                            socUartPrvFifoFlush(&uart->RX);  // clear the RX fifo now
                        }
                        if (val & UART_FCR_RESETTF) {
                            socUartPrvFifoFlush(&uart->TX);  // clear the TX fifo now
                            uart->LSR = UART_LSR_TEMT | UART_LSR_TDRQ;
                        }
                        uart->IIR = UART_IIR_FIFOES | UART_IIR_NOINT;
                    } else {
                        socUartPrvFifoFlush(&uart->TX);
                        socUartPrvFifoFlush(&uart->RX);
                        uart->LSR = UART_LSR_TEMT | UART_LSR_TDRQ;
                        uart->IIR = UART_IIR_NOINT;
                    }
                }
                uart->FCR = val;
                break;

            case 3:
                t = uart->LCR ^ val;
                if (t & UART_LCR_SB) {
                    if (val & UART_LCR_SB) {  // break set (tx line pulled low)

                        // nothing
                    } else {  // break cleared (tx line released)

                        socUartPrvSendChar(uart, UART_CHAR_BREAK);
                    }
                }
                uart->LCR = val;
                break;

            case 4:
                uart->MCR = val;
                break;

            case 7:
                uart->SPR = val;
                break;

            case 8:
                uart->ISR = val;
                if (val & 3) {
                    fprintf(stderr, "UART: IrDA mode set on UART\n");
                }
                break;
        }
    } else {
        switch (pa) {
            case 0:
                if (DLAB)
                    val = uart->DLL;
                else if (!(uart->LSR & UART_LSR_DR)) {  // no data-> too bad

                    val = 0;
                } else if (uart->FCR & UART_FCR_TRFIFOE) {  // fifo mode -> read fifo

                    val = socUartPrvFifoGet(&uart->RX);
                    if (!socUartPrvFifoUsed(&uart->RX)) uart->LSR &= ~UART_LSR_DR;
                    recalcValues = true;  // error bits might have changed
                } else {                  // polled mode -> read rx polled reg

                    val = uart->receiveHolding;
                    uart->LSR &= ~UART_LSR_DR;
                }
                break;

            case 1:
                if (DLAB)
                    val = uart->DLH;
                else
                    val = uart->IER;
                break;

            case 2:
                val = uart->IIR;
                break;

            case 3:
                val = uart->LCR;
                break;

            case 4:
                val = uart->MCR;
                break;

            case 5:
                val = uart->LSR;
                break;

            case 6:
                val = uart->MSR;
                break;

            case 7:
                val = uart->SPR;
                break;

            case 8:
                val = uart->ISR;
                break;
        }
        if (size == 1)
            *(uint8_t *)buf = val;
        else
            *(uint32_t *)buf = val;
    }

    if (recalcValues) socUartPrvRecalc(uart);

    return true;
}

void socUartSetFuncs(struct SocUart *uart, SocUartReadF readF, SocUartWriteF writeF,
                     void *userData) {
    if (!readF)
        readF = socUartPrvDefaultRead;  // these are special funcs since they get their own private
                                        // data - the uart :)
    if (!writeF) writeF = socUartPrvDefaultWrite;

    uart->readF = readF;
    uart->writeF = writeF;
    uart->accessFuncsData = userData;
}

struct SocUart *socUartInit(struct ArmMem *physMem, struct Reschedule reschedule, struct SocIc *ic,
                            uint32_t baseAddr, uint8_t irq) {
    struct SocUart *uart = (struct SocUart *)malloc(sizeof(*uart));

    if (!uart) ERR("cannot alloc UART at 0x%08x", baseAddr);

    memset(uart, 0, sizeof(*uart));
    uart->ic = ic;
    uart->reschedule = reschedule;
    uart->irq = irq;
    uart->baseAddr = baseAddr;
    uart->IIR = UART_IIR_NOINT;
    uart->IER = UART_IER_UUE | UART_IER_NRZE;  // uart on
    uart->LSR = UART_LSR_TEMT | UART_LSR_TDRQ;
    uart->MSR = UART_MSR_CTS;
    socUartPrvFifoFlush(&uart->TX);
    socUartPrvFifoFlush(&uart->RX);

    socUartSetFuncs(uart, NULL, NULL, NULL);

    if (!memRegionAdd(physMem, baseAddr, PXA_UART_SIZE, socUartPrvMemAccessF, uart))
        ERR("cannot add UART at 0x%08x to MEM\n", baseAddr);

    return uart;
}

void socUartProcess(struct SocUart *uart)  // send and rceive up to one character
{
    uint_fast16_t v;
    uint_fast8_t t;

    // first process sending (if any)
    if (!(uart->LSR & UART_LSR_TEMT)) {
        socUartPrvPutchar(uart, uart->transmitShift);

        if (uart->FCR & UART_FCR_TRFIFOE) {  // fifo mode

            t = socUartPrvFifoUsed(&uart->TX);

            if (t--) {
                uart->transmitShift = socUartPrvFifoGet(&uart->TX);
                if (t <= UART_FIFO_DEPTH / 2)
                    uart->LSR |= UART_LSR_TDRQ;  // above half full - clear TDRQ bit
            } else {
                uart->LSR |= UART_LSR_TEMT;
            }
        } else if (uart->LSR & UART_LSR_TDRQ) {
            uart->LSR |= UART_LSR_TEMT;
        } else {
            uart->transmitShift = uart->transmitHolding;
            uart->LSR |= UART_LSR_TDRQ;
        }
    }

    // now process receiving
    v = socUartPrvGetchar(uart);
    if (v != UART_CHAR_NONE) {
        uart->cyclesSinceRecv = 0;
        uart->LSR |= UART_LSR_DR;

        if (uart->FCR & UART_FCR_TRFIFOE) {  // fifo mode

            if (!socUartPrvFifoPut(&uart->RX, v)) {
                uart->LSR |= UART_LSR_OE;
            }
        } else {
            if (uart->LSR & UART_LSR_DR)
                uart->LSR |= UART_LSR_OE;
            else
                uart->receiveHolding = v;
        }
    } else if (uart->cyclesSinceRecv <= 4) {
        uart->cyclesSinceRecv++;
    }

    socUartPrvRecalc(uart);
}

static void socUartPrvRecalcCharBits(struct SocUart *uart, uint_fast16_t c) {
    if (c & UART_CHAR_BREAK) uart->LSR |= UART_LSR_BI;

    if (c & UART_CHAR_FRAME_ERR) uart->LSR |= UART_LSR_FE;

    if (c & UART_CHAR_PAR_ERR) uart->LSR |= UART_LSR_PE;
}

static void socUartPrvRecalc(struct SocUart *uart) {
    bool errorSet = false;
    uint_fast8_t v;

    uart->LSR &= ~UART_LSR_FIFOE;
    uart->IIR &= UART_IIR_FIFOES;  // clear all other bits...
    uart->LSR &= ~(UART_LSR_BI | UART_LSR_FE | UART_LSR_PE);

    if (uart->FCR & UART_FCR_TRFIFOE) {  // fifo mode

        // check rx fifo for errors
        for (v = 0; v < socUartPrvFifoUsed(&uart->RX); v++) {
            if ((socUartPrvFifoPeekNth(&uart->RX, v) >> 8) && (uart->IER & UART_IER_RLSE)) {
                uart->LSR |= UART_LSR_FIFOE;
                uart->IIR |= UART_IIR_RECV_ERR;
                errorSet = true;
                break;
            }
        }

        v = socUartPrvFifoUsed(&uart->RX);
        if (v) socUartPrvRecalcCharBits(uart, socUartPrvFifoPeek(&uart->RX));

        switch (uart->FCR & UART_FCR_ITL_MASK) {
            case UART_FCR_ITL_1:
                v = v >= 1;
                break;

            case UART_FCR_ITL_8:
                v = v >= 8;
                break;

            case UART_FCR_ITL_16:
                v = v >= 16;
                break;

            case UART_FCR_ITL_32:
                v = v >= 32;
                break;
        }
        if (v && (uart->IER & UART_IER_RAVIE) && !errorSet) {
            errorSet = true;
            uart->IIR |= UART_IIR_RECV_DATA;
        }
        if (socUartPrvFifoUsed(&uart->RX) && uart->cyclesSinceRecv >= 4 &&
            (uart->IER & UART_IER_RAVIE) && !errorSet) {
            errorSet = true;
            uart->IIR |= UART_IIR_RCV_TIMEOUT;
        }
    } else {  // polling mode

        uint_fast16_t c = uart->receiveHolding;

        if (uart->LSR & UART_LSR_DR) {
            socUartPrvRecalcCharBits(uart, c);

            if ((c >> 8) && !errorSet && (uart->IER & UART_IER_RLSE)) {
                uart->IIR |= UART_IIR_RECV_ERR;
                errorSet = true;
            } else if (!errorSet && (uart->IER & UART_IER_RAVIE)) {
                uart->IIR |= UART_IIR_RECV_DATA;
                errorSet = true;
            }
        }
    }

    if (uart->LSR & UART_LSR_TDRQ && !errorSet && (uart->IER & UART_IER_TIE)) {
        errorSet = true;
        uart->IIR |= UART_IIR_SEND_DATA;
    }

    if (!errorSet) uart->IIR |= UART_IIR_NOINT;

    socUartPrvIrq(uart, errorSet);
}

bool socUartTaskRequired(struct SocUart *uart) { return (uart->LSR & UART_LSR_TEMT) == 0; }
