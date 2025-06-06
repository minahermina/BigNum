/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

int
bn_num_bits(const BigNum *num)
{
    size_t top_index;
    BigNumWord top_word;
    int bits;

    MUST(num != NULL, "num pointer is NULL in bn_num_bits");

    if (bn_is_zero(num)) {
        return 0;
    }

    top_index = num->size - 1;
    top_word = num->words[top_index];

    bits = bn_num_bits_word(top_word);

    return bits + (top_index * BN_WORD_SIZE);
}
