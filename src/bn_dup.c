/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

BigNum*
bn_dup(const BigNum *src, Arena *arena)
{
    BigNum *num = NULL;
    MUST(src != NULL, "src pointer is NULL in bn_dup");

    /* create new BigNum num */
    num = bn_new(arena);

    MUST(num != NULL, "Allocating memory in bn_dup");

    bn_copy(num, src, arena);

    return num;
}
