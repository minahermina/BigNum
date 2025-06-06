/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

BigNum*
bn_zero(Arena *arena)
{
    BigNum *num;

    num = bn_new(arena);

    MUST(num != NULL, "num pointer is NULL in bn_zero");

    num->size = 1;
    num->words[0] = 0;
    return num;
}
