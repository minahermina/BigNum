/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

int
bn_num_bits_word(BigNumWord word)
{
    int bits;
    if(word == 0){
        return 0;
    }

    bits = 0;
    while(word > 0){
        bits++;
        word >>= 1;
    }
    return bits;
}
