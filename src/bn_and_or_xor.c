/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

/*
    * 0 -> and
    * 1 -> or
    * 2 -> xor
*/
BigNumWord
bn_and_or_xor_word(const BigNumWord word1, BigNumWord word2, int op)
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
bn_and_or_xor(BigNum *res, const BigNum *num1, const BigNum *num2, int op, Arena *arena)
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
    bn_resize(res, max_size, arena);

    for(i = 0; i < min_size; i++){
        res->words[i] = bn_and_or_xor_word(num1->words[i], num2->words[i], op);
    }

    zero = 0;
    for(; i < max_size; i++){
        res->words[i] = bn_and_or_xor_word(num1->words[i], zero, op);
    }

    return 1;
}

int
bn_and(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res  != NULL, "res pointer is NULL in bn_and");
    MUST(num1 != NULL, "num pointer is NULL in bn_and");
    MUST(num2 != NULL, "num pointer is NULL in bn_and");

    bn_and_or_xor(res, num1, num2, 0, arena);
    return 1;
}

int
bn_or(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res  != NULL, "res pointer is NULL in bn_or");
    MUST(num1 != NULL, "num pointer is NULL in bn_or");
    MUST(num2 != NULL, "num pointer is NULL in bn_or");

    bn_and_or_xor(res, num1, num2, 1, arena);
    return 1;
}

int
bn_xor(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res  != NULL, "res pointer is NULL in bn_xor");
    MUST(num1 != NULL, "num pointer is NULL in bn_xor");
    MUST(num2 != NULL, "num pointer is NULL in bn_xor");

    bn_and_or_xor(res, num1, num2, 2, arena);
    return 1;
}
