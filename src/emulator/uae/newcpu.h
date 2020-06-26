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

#define CLEAR_CZNV do { \
 SET_CFLG (0); \
 SET_ZFLG (0); \
 SET_NFLG (0); \
 SET_VFLG (0); \
} while (0)

#define COPY_CARRY (SET_XFLG (GET_CFLG))
#endif

extern int areg_byteinc[];
extern int imm8_table[];

extern int movem_index1[256];
extern int movem_index2[256];
extern int movem_next[256];

extern int fpp_movem_index1[256];
extern int fpp_movem_index2[256];
extern int fpp_movem_next[256];

extern int broken_in;

typedef unsigned long cpuop_func (uae_u32) REGPARAM;

struct cputbl {
    cpuop_func *handler;
    int specific;
    uae_u16 opcode;
	struct perfRec perf;
};

extern unsigned long op_illg (uae_u32) REGPARAM;

typedef char flagtype;

typedef struct regstruct
{
    uae_u32 regs[16];
    uaecptr  usp,isp,msp;
    uae_u16 sr;
    flagtype t1;
    flagtype t0;
    flagtype s;
    flagtype m;
    flagtype x;
    flagtype stopped;
    int intmask;

    uae_u32 pc;
    uae_u8 *pc_p;
    uae_u8 *pc_oldp;

    uae_u8 *pc_meta_oldp;

    uae_u32 vbr,sfc,dfc;

    double fp[8];
    uae_u32 fpcr,fpsr,fpiar;

    uae_u32 spcflags;
    uae_u32 kick_mask;

    /* Fellow sources say this is 4 longwords. That's impossible. It needs
     * to be at least a longword. The HRM has some cryptic comment about two
     * instructions being on the same longword boundary.
     * The way this is implemented now seems like a good compromise.
     */
    uae_u32 prefetch;
} regstruct;

#ifndef __ECM_DYNAMIC_PATCH

extern regstruct regs;
extern regstruct lastint_regs;

#define gRegs regs
#define gLastint_regs lastint_regs

#else //__ECM_DYNAMIC_PATCH

extern regstruct *gDynRegsP;

#define gRegs (*gDynRegsP)

#endif //__ECM_DYNAMIC_PATCH



#define m68k_dreg(r,num) ((r).regs[(num)])
#define m68k_areg(r,num) (((r).regs + 8)[(num)])

	// If we're profiling, go through the real work so we can count
	// read cycles.  Note that we don't want to actually return the
	// value returned by get_foo.  That function doesn't always word-
	// swap on little-endian machines (e.g., the DummyBank function).
	// However, the rest of the emulator is positioned to always
	// expect that opcode should be word-swapped on little-endian
	// machines (e.g., ATrap::DoCall).  Therefore, always fetch opcodes
	// with do_get_mem_foo, which will do that swapping.

#if HAS_PROFILING

	#define M68K_GETPC() (regs.pc+((char*)regs.pc_p-(char*)regs.pc_oldp))

	STATIC_INLINE uae_u8 get_ibyte (uae_s32 o) {
		if (gProfilingEnabled) get_byte(M68K_GETPC());
		return do_get_mem_byte((uae_u8 *)(regs.pc_p + (o) + 1)); }

	STATIC_INLINE uae_u16 get_iword (uae_s32 o) {
		if (gProfilingEnabled) get_word(M68K_GETPC());
		return do_get_mem_word((uae_u16 *)(regs.pc_p + (o))); }

	STATIC_INLINE uae_u32 get_ilong (uae_s32 o) {
		if (gProfilingEnabled) get_long(M68K_GETPC());
		return do_get_mem_long((uae_u32 *)(regs.pc_p + (o))); }

#else

	#define get_ibyte(o) do_get_mem_byte((uae_u8 *)(regs.pc_p + (o) + 1))
	#define get_iword(o) do_get_mem_word((uae_u16 *)(regs.pc_p + (o)))
	#define get_ilong(o) do_get_mem_long((uae_u32 *)(regs.pc_p + (o)))

#endif


#define m68k_incpc(o) (regs.pc_p += (o))

extern void Software_CheckNewPC (uaecptr newPC);

STATIC_INLINE void m68k_setpc (uaecptr newpc)
{
	Software_CheckNewPC (newpc);

	{
	addrbank*	bank = &(get_mem_bank(newpc));
    regs.pc_p = regs.pc_oldp = (bank->xlateaddr)(newpc);
    regs.pc = newpc;
    regs.pc_meta_oldp = (bank->xlatemetaaddr)(newpc);
	}
}

STATIC_INLINE uaecptr m68k_getpc (void)
{
    return regs.pc + ((char *)regs.pc_p - (char *)regs.pc_oldp);
}

STATIC_INLINE uaecptr m68k_getpc_p (uae_u8 *p)
{
    return regs.pc + ((char *)p - (char *)regs.pc_oldp);
}

#define m68k_setpc_fast m68k_setpc
#define m68k_setpc_bcc  m68k_setpc
#define m68k_setpc_rte  m68k_setpc

STATIC_INLINE void m68k_setstopped (int stop)
{
    regs.stopped = stop;
}

extern uae_u32 get_disp_ea_020 (uae_u32 base, uae_u32 dp);
extern uae_u32 get_disp_ea_000 (uae_u32 base, uae_u32 dp);

extern uae_s32 ShowEA (int reg, amodes mode, wordsizes size, char *buf);

extern void MakeSR (void);
extern void MakeFromSR (void);
extern void Exception (int, uaecptr);

/* Opcode of faulting instruction */
extern uae_u16 last_op_for_exception_3;
/* PC at fault time */
extern uaecptr last_addr_for_exception_3;
/* Address that generated the exception */
extern uaecptr last_fault_for_exception_3;

#define CPU_OP_NAME(a) op ## a

/* 68020 + 68881 */
extern struct cputbl op_smalltbl_0[];
/* 68020 */
extern struct cputbl op_smalltbl_1[];
/* 68010 */
extern struct cputbl op_smalltbl_2[];
/* 68000 */
extern struct cputbl op_smalltbl_3[];
/* 68000 slow but compatible.  */
extern struct cputbl op_smalltbl_4[];

extern cpuop_func *cpufunctbl[65536] ASM_SYM_FOR_FUNC ("cpufunctbl");

#ifdef __cplusplus
}
#endif

