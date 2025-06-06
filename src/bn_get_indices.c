/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

/*
 * Helper function to validate inputs and calc word and bit indices.
 */
int
bn_get_indices(const BigNum *num, int n, size_t *word_idx, size_t *bit_idx)
{
    int bits;

    bits = bn_num_bits(num);
    if (n < 0 || n > bits) {
        return -1;
    }

    *word_idx = n / BN_WORD_SIZE;
    *bit_idx = n % BN_WORD_SIZE;
    return 0;
}
