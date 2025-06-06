/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

int
bn_unset_bit(BigNum *num, int n)
{
    size_t word_idx, bit_idx;
    MUST(num != NULL, "num pointer is NULL in bn_unset_bit");

    if (bn_get_indices(num, n, &word_idx, &bit_idx) != 0) {
        return -1;
    }
    num->words[word_idx] &= ~((BigNumWord)1 << bit_idx);
    return 0;
}
