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
#include <utils.h>
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
bignum_lshift_word(const BigNumWord word, size_t nbits)
{
    if(nbits == 0)
        return word;

    if(nbits > BIGNUM_WORD_SIZE - 1)
        return 0;

    return (word << nbits);
}
int
bignum_lshift(BigNum *res, const BigNum *num, size_t nbits, Arena *arena)
{
    BigNumWord word, prev_msb;
    size_t i, shift_bits, word_prepend_count;

    MUST(res != NULL, "res pointer is NULL in bignum_lshift");
    MUST(num != NULL, "num pointer is NULL in bignum_lshift");

    /*no shifting needed*/
    if(nbits == 0)
        return 0;

    /* 
     * calc how many words may be needed to prepend if nbits >= BIGNUM_WORD_SIZE
     * and shift_bits to shift each word
    */
    word_prepend_count = nbits / BIGNUM_WORD_SIZE;
    shift_bits         = nbits % BIGNUM_WORD_SIZE;

    /*may be optimized !*/
    bignum_copy(res, num, arena);

    prev_msb = 0; /* a mask with all zeros */
    /* nbits < 64 or < 32 */
    if(shift_bits != 0){
        for(i = 0; i < res->size; ++i){ /* O(N) */
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
bignum_rshift_word(const BigNumWord word, size_t nbits)
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

/*
    * 0 -> and
    * 1 -> or
    * 2 -> xor
*/
BigNumWord
bignum_and_or_xor_word(const BigNumWord word1, BigNumWord word2, int op)
{
    assert(op == 0 || op == 1 || op == 2);
    BigNumWord res = 0;
    switch(op){
        case 0:
            res = word1 & word2;
            break;
        case 1:
            res = word1 | word2;
            break;
        case 2:
            res = word1 ^ word2;
            break;
        default:
            break;
    }
    return res;
}

int
bignum_and_or_xor(BigNum *res, const BigNum *num1, const BigNum *num2, int op, Arena *arena)
{
    BigNumWord zero;
    size_t i, max_size, min_size;
    const BigNum *temp;

    /* swap num1 with num2 */
    if(num1->size < num2->size){
        temp = num1;
        num1 = num2;
        num2 = temp;
    }

    max_size = num1->size;
    min_size = num2->size;
    bignum_resize(res, max_size, arena);

    for(i = 0; i < min_size; i++){
        res->words[i] = bignum_and_or_xor_word(num1->words[i], num2->words[i], op);
    }

    zero = 0;
    for(; i < max_size; i++){
        res->words[i] = bignum_and_or_xor_word(num1->words[i], zero, op);
    }

    return 1;
}

int
bignum_and(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res != NULL, "res pointer is NULL in bignum_and");
    MUST(num1 != NULL, "num pointer is NULL in bignum_and");
    MUST(num2 != NULL, "num pointer is NULL in bignum_and");

    bignum_and_or_xor(res, num1, num2, 0, arena);
    return 1;
}

int
bignum_or(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res != NULL, "res pointer is NULL in bignum_or");
    MUST(num1 != NULL, "num pointer is NULL in bignum_or");
    MUST(num2 != NULL, "num pointer is NULL in bignum_or");

    bignum_and_or_xor(res, num1, num2, 1, arena);
    return 1;
}

int
bignum_xor(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res != NULL, "res pointer is NULL in bignum_xor");
    MUST(num1 != NULL, "num pointer is NULL in bignum_xor");
    MUST(num2 != NULL, "num pointer is NULL in bignum_xor");

    bignum_and_or_xor(res, num1, num2, 2, arena);
    return 1;
}
