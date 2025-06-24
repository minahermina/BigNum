/*
    Copyright (C) 2025  Mina Albert Saeed mina.albert.saeed@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef BIGUM_UTILS
#define BIGUM_UTILS
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <bignum.h>

#define ABS(x) (x < 0 ? -x : x)
#define MUST(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Error: %s\n", (message)); \
            assert(condition); \
        } \
    } while (0)

char* dec_to_hex(const char* str, size_t len);
char* bin_to_hex(const char* str, size_t len);

/*
 * Helper function to calc word and bit indices.
 */
int bn_get_indices(const BigNum *num, int n, size_t *word_idx, size_t *bit_idx);
#endif /*BIGUM_UTILS*/
