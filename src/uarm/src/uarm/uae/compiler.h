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

extern unsigned long op_unimplemented(uint32_t opcode);

#define CHECK_STACK_POINTER_ASSIGNMENT() {}

#define CHECK_STACK_POINTER_DECREMENT()	{}

#define CHECK_STACK_POINTER_INCREMENT()	{}

// Called in RTD handler
#define compiler_flush_jsr_stack() do { ; } while (0)

STATIC_INLINE void m68k_do_rts(void)
{
	uaecptr	returnAddr = get_long (m68k_areg (regs, 7));

    m68k_setpc(returnAddr);
    m68k_areg(regs, 7) += 4;
}

STATIC_INLINE void m68k_do_rte(uaecptr newpc)
{
	m68k_setpc_rte(newpc);
}

STATIC_INLINE void m68k_do_bsr(uaecptr oldpc, uae_s32 offset)
{
    m68k_areg(regs, 7) -= 4;
    put_long(m68k_areg(regs, 7), oldpc);
    m68k_incpc(offset);
}

STATIC_INLINE void m68k_do_jsr(uaecptr oldpc, uaecptr dest)
{
    m68k_areg(regs, 7) -= 4;
    put_long(m68k_areg(regs, 7), oldpc);
    m68k_setpc(dest);
}

#ifdef __cplusplus
}
#endif

