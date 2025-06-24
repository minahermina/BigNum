/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

void
bn_prepend_zero_words(BigNum *num, size_t cnt, Arena *arena)
{
    size_t oldsize, i;
    assert(num != NULL);

    if (cnt == 0)
        return ;

    oldsize = num->size;

    bn_resize(num, oldsize + cnt, arena);

    /* shift from back to front to avoid overwriting */
    for (i = oldsize; i > 0; i--) {
        num->words[i + cnt - 1] = num->words[i - 1];
    }

    /* Zero out the prepended cnt words */
    for (i = 0; i < cnt; i++) {
        num->words[i] = 0;
    }
    return;
}
