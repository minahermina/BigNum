/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>
/*
 * TODO: CRT and other exponentiation reduction may be used but for now this works
*/
int
bn_mod_pow(BigNum *res, const BigNum *num, const BigNum *pow, const BigNum *mod, Arena *arena)
{
    BigNum *temp;
    int i, pow_bit_len;
    MUST(res != NULL, "res pointer is NULL in bn_pow");
    MUST(num != NULL, "num1 pointer is NULL in bn_pow");
    MUST(pow != NULL, "num2 pointer is NULL in bn_pow");

    pow_bit_len = bn_num_bits(pow);

    /* 0 ^ pow = 0  || pow = 0 */
    if (bn_is_zero(num) || pow_bit_len <= 0) {
        return bn_set_zero(res);
    }

    /* num ^ 0 = 1 */
    if (bn_is_zero(pow)) {
        bn_resize(res, 1, arena);
        res->words[0] = 1;
        return 0;
    }

    /* temp for square operation*/
    temp = bn_new(arena);

    bn_copy(res, num, arena);
    bn_copy(temp, res, arena);
    for (i = pow_bit_len - 2; i >= 0; i--) {
        /* square the res */
        bn_mul(res, temp, temp, arena);

        /* if bit is set, multiply num to res */
        if (bn_is_bit_set(pow, i)) {
            /* Copy res to temp */
            bn_copy(temp, res, arena);

            /* multiply res by num*/
            bn_mul(res, temp, num, arena);
        }
        bn_mod(temp, res, mod, arena);
        bn_copy(res, temp, arena);
    }

    if(arena == NULL){
        bn_free(temp);
    }
    return 0;
}
