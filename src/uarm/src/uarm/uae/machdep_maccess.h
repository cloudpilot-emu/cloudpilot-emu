 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory access functions
  *
  * Copyright 1996 Bernd Schmidt
  */

#define get_long			uae_get32
#define get_word			uae_get16
#define get_byte			uae_get8

#define put_long			uae_put32
#define put_word			uae_put16
#define put_byte			uae_put8

extern uint8_t uae_get8(uint32_t addr);
extern uint16_t uae_get16(uint32_t addr);
extern uint32_t uae_get32(uint32_t addr);

extern void uae_put8(uint8_t value, uint32_t addr);
extern void uae_put16(uint16_t value, uint32_t addr);
extern void uae_put32(uint32_t value, uint32_t addr);
