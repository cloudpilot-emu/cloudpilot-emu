#include "UAE.h"

regstruct regs;

int areg_byteinc[] = {1, 1, 1, 1, 1, 1, 1, 2};
int imm8_table[] = {8, 1, 2, 3, 4, 5, 6, 7};

int movem_index1[256];
int movem_index2[256];
int movem_next[256];
