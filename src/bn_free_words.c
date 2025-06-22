/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

void
bn_free_words(BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bn_free_words");
    MUST(num->words != NULL, "num->words pointer is NULL in bn_free_words");

    free(num->words);
}
