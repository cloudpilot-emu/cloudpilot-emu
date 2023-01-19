/* io.c - Virtual disk input/output

   Copyright (C) 1993 Werner Almesberger <werner.almesberger@lrc.di.epfl.ch>
   Copyright (C) 1998 Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
   Copyright (C) 2008-2014 Daniel Baumann <mail@daniel-baumann.ch>
   Copyright (C) 2015 Andreas Bombe <aeb@debian.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.

   The complete text of the GNU General Public License
   can be found in /usr/share/common-licenses/GPL-3 file.
*/

/*
 * Thu Feb 26 01:15:36 CET 1998: Martin Schulze <joey@infodrom.north.de>
 *	Fixed nasty bug that caused every file with a name like
 *	xxxxxxxx.xxx to be treated as bad name that needed to be fixed.
 */

/* FAT32, VFAT, Atari format support, and various fixes additions May 1998
 * by Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de> */

#include "io.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
    #include <sys/types.h>
#endif

#include "card_io.h"
#include "common.h"

typedef struct _change {
    void *data;
    off_t pos;
    int size;
    struct _change *next;
} CHANGE;

static CHANGE *changes, *last;
static int did_change = 0;

void fs_open() {
    if (!card_open()) die("failed to open card");

    changes = last = NULL;
    did_change = 0;
}

/**
 * Read data from the partition, accounting for any pending updates that are
 * queued for writing.
 *
 * @param[in]   pos     Byte offset, relative to the beginning of the partition,
 *                      at which to read
 * @param[in]   size    Number of bytes to read
 * @param[out]  data    Where to put the data read
 */
void fs_read(int pos, int size, void *data) {
    CHANGE *walk;

    if (!card_read(pos, size, data)) die("failed to read from card");

    for (walk = changes; walk; walk = walk->next) {
        if (walk->pos < pos + size && walk->pos + walk->size > pos) {
            if (walk->pos < pos)
                memcpy(data, (char *)walk->data + pos - walk->pos,
                       min(size, walk->size - pos + walk->pos));
            else
                memcpy((char *)data + walk->pos - pos, walk->data,
                       min(walk->size, size + pos - walk->pos));
        }
    }
}

int fs_test(int pos, int size) { return card_is_valid_range(pos, size); }

void fs_write(int pos, int size, void *data) {
    CHANGE *new;

    if (write_immed) {
        did_change = 1;
        if (!card_write(pos, size, data)) die("failed to write to card");
    }
    new = alloc(sizeof(CHANGE));
    new->pos = pos;
    memcpy(new->data = alloc(new->size = size), data, size);
    new->next = NULL;
    if (last)
        last->next = new;
    else
        changes = new;
    last = new;
}

static void fs_flush(void) {
    CHANGE *this;

    while (changes) {
        this = changes;
        changes = changes->next;

        if (!card_write(this->pos, this->size, this->data)) die("failed to write chunk to card");

        free(this->data);
        free(this);
    }
}

int fs_close(int write) {
    CHANGE *next;
    int changed;

    changed = !!changes;
    if (write)
        fs_flush();
    else
        while (changes) {
            next = changes->next;
            free(changes->data);
            free(changes);
            changes = next;
        }
    if (!card_close()) die("failed to close card");
    return changed || did_change;
}

int fs_changed(void) { return !!changes || did_change; }
