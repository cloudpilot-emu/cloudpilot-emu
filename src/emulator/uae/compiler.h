 /*
  * UAE - The Un*x Amiga Emulator
  *
  * m68k -> i386 compiler
  *
  * (c) 1995 Bernd Schmidt
  */

#ifdef __cplusplus
extern "C" {
#endif

extern void Software_ProcessLINK (uae_s32 linkSize);
extern int Software_ProcessRTS (uaecptr dest);
extern int Software_ProcessRTE (uaecptr newpc);
extern int Software_ProcessJSR (uaecptr oldpc, uaecptr dest);
extern int Software_ProcessJSR_Ind (uaecptr oldpc, uaecptr dest);
extern void Software_CheckNewPC (uaecptr newPC);

extern void Software_CheckStackPointerAssignment (void);
extern void Software_CheckStackPointerDecrement (void);
extern void Software_CheckStackPointerIncrement (void);
extern void Software_CheckKernelStack (void);

extern uae_u32	gStackHigh;
extern uae_u32	gStackLowWarn;
extern uae_u32	gStackLow;
extern uae_u32	gKernelStackOverflowed;

#define CHECK_STACK_POINTER_ASSIGNMENT()		\
do {											\
	Software_CheckStackPointerAssignment ();	\
	if (gKernelStackOverflowed)					\
		Software_CheckKernelStack ();			\
} while (0)

#define CHECK_STACK_POINTER_DECREMENT()			\
do {											\
	if (m68k_areg (regs, 7) < gStackLowWarn)	\
		Software_CheckStackPointerDecrement ();	\
	if (gKernelStackOverflowed)					\
		Software_CheckKernelStack ();			\
} while (0)

#define CHECK_STACK_POINTER_INCREMENT()			\
do {											\
	if (m68k_areg (regs, 7) > gStackHigh)		\
		Software_CheckStackPointerIncrement ();	\
	if (gKernelStackOverflowed)					\
		Software_CheckKernelStack ();			\
} while (0)

// Called in RTD handler
#define compiler_flush_jsr_stack() do { ; } while (0)

STATIC_INLINE void m68k_do_rts(void)
{
	uaecptr	returnAddr = get_long (m68k_areg (regs, 7));
	if (Software_ProcessRTS (returnAddr))
		return;

    m68k_setpc(returnAddr);
    m68k_areg(regs, 7) += 4;
}

STATIC_INLINE void m68k_do_rte(uaecptr newpc)
{
	if (Software_ProcessRTE(newpc))
		return;

	m68k_setpc_rte(newpc);
}

STATIC_INLINE void m68k_do_bsr(uaecptr oldpc, uae_s32 offset)
{
	/* CHANGED BY B. CAMERON LESIUK
	 * 19 Jan 2001
	 * Made this modification so that BSRs cause the profiler to
	 * track function calls properly.
	 */
//	Software_CheckNewPC (m68k_getpc () + offset);

	if (Software_ProcessJSR (oldpc, m68k_getpc () + offset))
		return;

    m68k_areg(regs, 7) -= 4;
	Software_CheckStackPointerDecrement ();
    put_long(m68k_areg(regs, 7), oldpc);
    m68k_incpc(offset);
}

STATIC_INLINE void m68k_do_jsr(uaecptr oldpc, uaecptr dest)
{
	if (Software_ProcessJSR (oldpc, dest))
		return;

    m68k_areg(regs, 7) -= 4;
	Software_CheckStackPointerDecrement ();
    put_long(m68k_areg(regs, 7), oldpc);
    m68k_setpc(dest);
}

#ifdef __cplusplus
}
#endif

