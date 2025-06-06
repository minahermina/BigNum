/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
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

#include <utils.h>

/*
 * TODO: improve memory allocation and use arena maybe !
*/
char*
bin_to_hex(const char* str, size_t len)
{
    size_t i, j, hex_len, val;
    char *result;

    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return NULL;
        }
    }

    hex_len = (len + 3) / 4;

    /* result (+1 for '\0') */
    result = (char*) malloc(hex_len + 1);
    if (result == NULL) {
        return NULL;
    }

    /* Process in groups of 4 */
    for (i = 0; i < hex_len; i++) {
        val = 0;

        /* each group  */
        for (j = 0; j < 4; j++) {
            /* position in the input string */
            int pos = len - 1 - (i * 4 + j);

            if (pos >= 0) {
                if (str[pos] == '1') {
                    val |= (1 << j);
                }
            }
        }

        /* Convert to hex char */
        result[hex_len - 1 - i] = val < 10 ? '0' + val : 'a' + (val - 10);
    }

    result[hex_len] = '\0';
    return result;
}

char*
dec_to_hex(const char* str, size_t len)
{
    size_t hex_len, i, j, start_idx = 0;
    char *result, digit;
    int is_negative = 0;

    // Check for negative sign
    if(str[0] == '-'){
        start_idx = 1;
        is_negative = 1;
    }

    // Check if the input contains only decimal digits
    for (size_t i = start_idx; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return NULL; // Invalid input
        }
    }

    // Handle special case: "-0" or "0"
    if ((len == 1 && str[0] == '0') || (len == 2 && str[0] == '-' && str[1] == '0')) {
        char* result = (char*)malloc(2);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '0';
        result[1] = '\0';
        return result;
    }

    // Calculate buffer size based on actual digits (excluding the sign)
    size_t digit_len = len - start_idx;
    size_t max_hex_len = (digit_len * 10 + 3) / 4;
    char* hex_buf = (char*) calloc(max_hex_len + 1, 1);
    if (hex_buf == NULL) {
        return NULL;
    }

    // Process decimal digits
    for (i = start_idx; i < len; i++) {
        int carry = str[i] - '0';
        for (j = 0; j < max_hex_len; j++) {
            int val = hex_buf[j] * 10 + carry;
            hex_buf[j] = val & 0xF;
            carry = val >> 4;
        }
    }

    // Find the end of the significant digits
    hex_len = max_hex_len;
    while (hex_len > 0 && hex_buf[hex_len - 1] == 0) {
        hex_len--;
    }

    size_t result_size = hex_len + (is_negative ? 1 : 0) + 1;
    result = (char*)malloc(result_size);
    if (result == NULL) {
        free(hex_buf);
        return NULL;
    }

    // Convert to hex char
    size_t result_idx = 0;

    if (is_negative) {
        result[result_idx++] = '-';
    }

    // Convert to hex characters
    for (i = 0; i < hex_len; i++) {
        digit = hex_buf[hex_len - i - 1];
        result[result_idx + i] = digit < 10 ? '0' + digit : 'a' + (digit - 10);
    }

    result[result_idx + hex_len] = '\0';
    free(hex_buf);
    return result;
}
