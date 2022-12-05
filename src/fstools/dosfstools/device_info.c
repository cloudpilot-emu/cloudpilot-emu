/* device_info.c - Collect device information for mkfs.fat

   Copyright (C) 2015 Andreas Bombe <aeb@debian.org>
   Copyright (C) 2018 Pali Rohár <pali.rohar@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "device_info.h"

#include "card_io.h"

static const struct device_info device_info_clueless = {
    .type = TYPE_UNKNOWN,
    .partition = -1,
    .has_children = -1,
    .geom_heads = -1,
    .geom_sectors = -1,
    .geom_start = -1,
    .geom_size = -1,
    .sector_size = -1,
    .size = -1,
};

int device_info_verbose;

int get_device_info(int fd, struct device_info *info) {
    *info = device_info_clueless;

    info->type = TYPE_FILE;
    info->partition = 0;
    info->size = card_get_size();
    info->sector_size = 512;
    info->geom_start = 0;
    info->geom_heads = card_geometry_heads();
    info->geom_sectors = card_geometry_sectors();

    return 0;
}

int is_device_mounted(const char *path) {
    (void)path; /* prevent unused parameter warning */
    return 0;
}
