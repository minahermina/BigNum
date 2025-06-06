/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include <bignum.h>
#include <utils.h>
#include <inttypes.h>
#include <assert.h>

int
bn_pow(BigNum *res, const BigNum *num, const BigNum *pow, Arena *arena)
{

    BigNum temp;
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
    }

    /* temp for square operation*/
    bn_init(&temp, arena);

    bn_copy(res, num, arena);

    for (i = pow_bit_len - 2; i >= 0; i--) {
        bn_copy(&temp, res, arena);

        /* square the res */
        bn_mul(res, &temp, &temp, arena);

        /* if bit is set, multiply num to res */
        if (bn_is_bit_set(pow, i)) {
            /* Copy res to temp */
            bn_copy(&temp, res, arena);

            /* multiply res by num*/
            bn_mul(res, &temp, num, arena);
        }
    }

    if(arena == NULL){
        bn_free(&temp);
    }
    return 0;

}
