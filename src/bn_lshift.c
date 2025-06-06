/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

/*
set prev_msb = all zeros
FOR from 0 to num's size
    1- set word = num.words[0]
    2- num.words[i] = nums.words[i] | with prev_msb
    3- set prev_msb with nums.words[i]'s  for next loop
*/

int
bn_lshift(BigNum *res, const BigNum *num, size_t nbits, Arena *arena)
{
    BigNumWord word, prev_msb;
    size_t i, shift_bits, word_prepend_count;

    MUST(res != NULL, "res pointer is NULL in bn_lshift");
    MUST(num != NULL, "num pointer is NULL in bn_lshift");

    /*no shifting needed*/
    if(nbits == 0)
        return 0;

    /* 
     * calc how many words may be needed to prepend if nbits >= BN_WORD_SIZE
     * and shift_bits to shift each word
    */
    word_prepend_count = nbits / BN_WORD_SIZE;
    shift_bits         = nbits % BN_WORD_SIZE;

    /*may be optimized !*/
    bn_copy(res, num, arena);

    prev_msb = 0; /* a mask with all zeros */
    /* nbits < 64 or < 32 */
    if(shift_bits != 0){
        for(i = 0; i < res->size; ++i){ /* O(N) */
            word = num->words[i];
            res->words[i] = bn_lshift_word(word, shift_bits);
            res->words[i] |=  prev_msb;
            prev_msb = ((word >> (BN_WORD_SIZE - shift_bits)));
        }

        if(prev_msb != 0) {
            bn_append_word(res, prev_msb, arena);
        }
    }

    bn_prepend_zero_words(res, word_prepend_count, arena);
    return 0;
}
