/*
    Copyright (C) 2025  Mina Albert Saeed mina.albert.saeed@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <bignum.h>
#include <inttypes.h>
#include <assert.h>



/*
set prev_msb = all zeros
FOR from 0 to num's size
    1- set word = num.words[0]
    2- num.words[i] = nums.words[i] | with prev_msb
    3- set prev_msb with nums.words[i]'s  for next loop
*/

BigNumWord
bignum_lshift_word(BigNumWord word, size_t nbits)
{
    if(nbits == 0)
        return word;

    if(nbits > BIGNUM_WORD_SIZE - 1)
        return 0;

    return (word << nbits);
}
int
bignum_lshift(BigNum *res, BigNum *num, size_t nbits, Arena *arena)
{
    BigNumWord word, prev_msb;
    size_t i, shift_bits, word_prepend_count;

    assert(res != NULL && num != NULL );
    /*no shifting needed*/
    if(nbits == 0)
        return 0;

    /* calc how many words may be needed to prepend if nbits >= BIGNUM_WORD_SIZE
        and shift_bits to shift each word
    */
    word_prepend_count = nbits / BIGNUM_WORD_SIZE;
    shift_bits         = nbits % BIGNUM_WORD_SIZE;

    /*may be optimized !*/
    bignum_copy(res, num, arena);

    prev_msb = 0; /*a mask with all zeros*/
    /*nbits < 64 or < 32*/
    if(shift_bits != 0){
        for(i = 0; i < res->size; ++i){ // O(N)
            word = num->words[i];
            res->words[i] = bignum_lshift_word(word, shift_bits);
            res->words[i] |=  prev_msb;
            prev_msb = ((word >> (BIGNUM_WORD_SIZE - shift_bits)));
        }

        if(prev_msb != 0) {
            bignum_append_word(res, prev_msb, arena);
        }
    }

    bignum_prepend_zero_words(res, word_prepend_count, arena);
    return 0;
}



BigNumWord
bignum_rshift_word(BigNumWord word, size_t nbits)
{
    if(nbits == 0)
        return word;

    if(nbits > BIGNUM_WORD_SIZE - 1)
        return 0;

    return (word >> nbits);
}

/* int
bignum_rshift(BigNum *res, const BigNum *a, int nbits)
{
} */

