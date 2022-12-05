/* common.c - Common functions

   Copyright (C) 1993 Werner Almesberger <werner.almesberger@lrc.di.epfl.ch>
   Copyright (C) 1998 Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
   Copyright (C) 2008-2014 Daniel Baumann <mail@daniel-baumann.ch>
   Copyright (C) 2018 Pali Rohár <pali.rohar@gmail.com>

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

/* FAT32, VFAT, Atari format support, and various fixes additions May 1998
 * by Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de> */

#include "common.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <wctype.h>

#include "charconv.h"
#include "terminate.h"

int interactive;
int write_immed;
int atari_format;
const char *program_name;

typedef struct _link {
    void *data;
    struct _link *next;
} LINK;

void die(const char *msg, ...) {
    va_list args;

    if (program_name) fprintf(stderr, "%s: ", program_name);

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");

    terminate();
}

void pdie(const char *msg, ...) {
    va_list args;

    if (program_name) fprintf(stderr, "%s: ", program_name);

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, ":%s\n", strerror(errno));

    terminate();
}

void *alloc(int size) {
    void *this;

    if ((this = malloc(size))) return this;
    pdie("malloc");
    return NULL; /* for GCC */
}

void *qalloc(void **root, int size) {
    LINK *link;

    link = alloc(sizeof(LINK));
    link->next = *root;
    *root = link;
    return link->data = alloc(size);
}

void qfree(void **root) {
    LINK *this;

    while (*root) {
        this = (LINK *)*root;
        *root = this->next;
        free(this->data);
        free(this);
    }
}

int min(int a, int b) { return a < b ? a : b; }

#ifdef EMSCRIPTEN
static int vasprintf(char **strp, const char *fmt, va_list va) {
    int length;
    va_list vacopy;

    va_copy(vacopy, va);

    length = vsnprintf(NULL, 0, fmt, vacopy);
    if (length < 0) return length;

    *strp = malloc(length + 1);
    if (!*strp) {
        errno = ENOMEM;
        return -1;
    }

    return vsnprintf(*strp, length + 1, fmt, va);
}
#endif

int xasprintf(char **strp, const char *fmt, ...) {
    va_list va;
    int retval;

    va_start(va, fmt);
    retval = vasprintf(strp, fmt, va);
    va_end(va);

    if (retval < 0) pdie("asprintf");

    return retval;
}

int get_choice(int noninteractive_result, const char *noninteractive_msg, int choices, ...) {
    if (!interactive) {
        printf("%s\n", noninteractive_msg);
        return noninteractive_result;
    }

    terminate();
    return 0;
}

char *get_line(const char *prompt, char *dest, size_t length) {
    terminate();
    return NULL;
}

/*
 * ++roman: On m68k, check if this is an Atari; if yes, turn on Atari variant
 * of MS-DOS filesystem by default.
 */
void check_atari(void) {
#if defined(__mc68000__) && defined(__linux__) && defined(CONF_CHECK_ATARI)
    FILE *f;
    char line[128], *p;

    if (!(f = fopen("/proc/hardware", "r"))) {
        perror("/proc/hardware");
        return;
    }

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Model:", 6) == 0) {
            p = line + 6;
            p += strspn(p, " \t");
            if (strncmp(p, "Atari ", 6) == 0) atari_format = 1;
            break;
        }
    }
    fclose(f);
#endif
}

uint32_t generate_volume_id(void) {
    struct timeval now;
    char *source_date_epoch = NULL;

    source_date_epoch = getenv("SOURCE_DATE_EPOCH");
    if (source_date_epoch) {
        char *tmp = NULL;
        long long conversion = 0;
        errno = 0;
        conversion = strtoll(source_date_epoch, &tmp, 10);
        if (!isdigit((unsigned char)*source_date_epoch) || *tmp != '\0' || errno != 0) {
            die("SOURCE_DATE_EPOCH is too big or contains non-digits: \"%s\"", source_date_epoch);
        }
        return (uint32_t)conversion;
    } else if (gettimeofday(&now, NULL) != 0 || now.tv_sec == (time_t)-1 || now.tv_sec < 0) {
        srand(getpid());
        /* rand() returns int from [0,RAND_MAX], therefore only 31 bits */
        return (((uint32_t)(rand() & 0xFFFF)) << 16) | ((uint32_t)(rand() & 0xFFFF));
    }

    /* volume ID = current time, fudged for more uniqueness */
    return ((uint32_t)now.tv_sec << 20) | (uint32_t)now.tv_usec;
}

/*
 * Validate volume label
 *
 * @param[in]   doslabel   Label stored according to current DOS codepage
 *
 * @return   bitmask of errors
 *           0x01 - lowercase character
 *           0x02 - character below 0x20
 *           0x04 - character in disallowed set
 *           0x08 - empty or space-only label
 *           0x10 - space at beginning
 */
int validate_volume_label(char *doslabel) {
    int i;
    int ret = 0;
    wchar_t wlabel[12];

    if (dos_string_to_wchar_string(wlabel, doslabel, sizeof(wlabel))) {
        for (i = 0; wlabel[i]; i++) {
            /* FAT specification: Lower case characters are not allowed in DIR_Name
                                  (what these characters are is country specific)
               Original label is stored in DOS OEM code page, so islower() function
               cannot be used. Therefore convert original label to locale independent
               wchar_t* and then use iswlower() function for it.
            */
            if (iswlower(wlabel[i])) {
                ret |= 0x01;
                break;
            }
        }
    }

    /* According to FAT specification those bytes (after conversion to DOS OEM
       code page) are not allowed.
     */
    for (i = 0; i < 11; i++) {
        if ((unsigned)doslabel[i] < 0x20) ret |= 0x02;
        if (doslabel[i] == 0x22 || (doslabel[i] >= 0x2A && doslabel[i] <= 0x2C) ||
            doslabel[i] == 0x2E || doslabel[i] == 0x2F ||
            (doslabel[i] >= 0x3A && doslabel[i] <= 0x3F) ||
            (doslabel[i] >= 0x5B && doslabel[i] <= 0x5D) || doslabel[i] == 0x7C)
            ret |= 0x04;
    }

    if (memcmp(doslabel, "           ", 11) == 0) ret |= 0x08;

    if (doslabel[0] == ' ') ret |= 0x10;

    return ret;
}
