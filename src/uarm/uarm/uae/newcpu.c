#include "UAE.h"

regstruct regs;
struct flag_struct regflags;

int areg_byteinc[] = {1, 1, 1, 1, 1, 1, 1, 2};
int imm8_table[] = {8, 1, 2, 3, 4, 5, 6, 7};

int movem_index1[256];
int movem_index2[256];
int movem_next[256];

uae_u32 get_disp_ea_000(uae_u32 base, uae_u32 dp) {
  int reg = (dp >> 12) & 15;
  uae_s32 regd = regs.regs[reg];

  if ((dp & 0x800) == 0)
    regd = (uae_s32)(uae_s16)regd;

  return base + (uae_s8)dp + regd;
}

void MakeSR() {
  regs.sr = (regs.sr & 0xff00) | (GET_XFLG << 4) | (GET_NFLG << 3) |
            (GET_ZFLG << 2) | (GET_VFLG << 1) | GET_CFLG;
}

void MakeFromSR() {
  SET_XFLG((regs.sr >> 4) & 1);
  SET_NFLG((regs.sr >> 3) & 1);
  SET_ZFLG((regs.sr >> 2) & 1);
  SET_VFLG((regs.sr >> 1) & 1);
  SET_CFLG(regs.sr & 1);
}
