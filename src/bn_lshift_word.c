/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

BigNumWord
bn_lshift_word(const BigNumWord word, size_t nbits)
{
    if(nbits == 0)
        return word;

    if(nbits > BN_WORD_SIZE - 1)
        return 0;

    return (word << nbits);
}
