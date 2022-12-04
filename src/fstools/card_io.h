#ifndef _CARD_IO_H_
#define _CARD_IO_H_

#ifdef __cplusplus

    #include <cstddef>

extern "C" {
#else
    #include <stddef.h>
#endif

int card_get_size();

int card_open();

int card_read(int offset, int size, void* buffer);

int card_is_valid_range(int offset, int size);

int card_write(int offset, int size, const void* buffer);

int card_close();

#ifdef __cplusplus
}
#endif

#endif  // _CARD_IO_H_
