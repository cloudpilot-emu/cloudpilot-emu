#ifndef _CARD_IO_H_
#define _CARD_IO_H_

#ifdef __cplusplus

    #include <cstddef>

extern "C" {
#else
    #include <stddef.h>
#endif

void card_initialize(void* card_volume);

int card_get_size();

int card_open();

int card_read(int offset, int size, void* buffer);

int card_is_valid_range(int offset, int size);

int card_write(int offset, int size, const void* buffer);

int card_close();

int card_geometry_sectors();

int card_geometry_heads();

int card_geometry_start();

#ifdef __cplusplus
}
#endif

#endif  // _CARD_IO_H_
