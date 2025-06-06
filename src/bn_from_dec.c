/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

BigNum*
bn_from_dec(const char *str, size_t len, Arena* arena)
{
    BigNum *num;
    const char *hex_str;

    MUST(str != NULL, "str pointer is NULL in bn_from_dec");
    MUST(len != 0, "len is 0 in bn_from_dec");

    hex_str = dec_to_hex(str, len);
    assert(hex_str != NULL);

    printf("hex string is: %s\n", hex_str);
    num = bn_from_hex(hex_str, strlen(hex_str), arena);


    return num;
}
