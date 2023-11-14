/*
 * UAE - The Un*x Amiga Emulator
 *
 * MC68000 emulation
 *
 * Copyright 1995 Bernd Schmidt
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SET_CFLG

#define SET_CFLG(x) (CFLG = (x))
#define SET_NFLG(x) (NFLG = (x))
#define SET_VFLG(x) (VFLG = (x))
#define SET_ZFLG(x) (ZFLG = (x))
#define SET_XFLG(x) (XFLG = (x))

#define GET_CFLG CFLG
#define GET_NFLG NFLG
#define GET_VFLG VFLG
#define GET_ZFLG ZFLG
#define GET_XFLG XFLG

#define CLEAR_CZNV                                                             \
  do {                                                                         \
    SET_CFLG(0);                                                               \
    SET_ZFLG(0);                                                               \
    SET_NFLG(0);                                                               \
    SET_VFLG(0);                                                               \
  } while (0)

#define COPY_CARRY (SET_XFLG(GET_CFLG))
#endif

extern int areg_byteinc[];
extern int imm8_table[];

extern int movem_index1[256];
extern int movem_index2[256];
extern int movem_next[256];

typedef unsigned long cpuop_func(uae_u32) REGPARAM;

struct cputbl {
  cpuop_func *handler;
  int specific;
  uae_u16 opcode;
  struct perfRec perf;
};

extern unsigned long op_illg(uae_u32) REGPARAM;

typedef uae_u8 flagtype;

typedef struct regstruct {
  uae_u32 regs[16];
  uae_u16 sr;

  uae_u32 pc;
} regstruct;

extern regstruct regs;

#define m68k_dreg(r, num) ((r).regs[(num)])
#define m68k_areg(r, num) (((r).regs + 8)[(num)])

#define get_ibyte(o) get_byte(regs.pc + (o) + 1)
#define get_iword(o) get_word(regs.pc + (o))
#define get_ilong(o) get_long(regs.pc + (o))

#define m68k_incpc(o) (regs.pc += (o))

STATIC_INLINE void m68k_setpc(uaecptr newpc) { regs.pc = newpc; }

STATIC_INLINE uaecptr m68k_getpc(void) { return regs.pc; }

#define m68k_setpc_fast m68k_setpc
#define m68k_setpc_bcc m68k_setpc
#define m68k_setpc_rte m68k_setpc

uae_u32 get_disp_ea_000(uae_u32 base, uae_u32 dp);

extern void MakeSR(void);
extern void MakeFromSR(void);
extern void Exception(int, uaecptr);

#define CPU_OP_NAME(a) op##a

/* 68000 */
extern struct cputbl op_smalltbl_3[];

#ifdef __cplusplus
}
#endif
