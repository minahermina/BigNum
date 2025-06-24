/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>
/*
 * this uses the double-and-add algorithm
 * which is not efficient but i needed this function just to work :)
*/
int bn_mul(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int i, num1_bit_len;
    BigNum *temp;
    MUST(res != NULL, "res pointer is NULL in bn_mul");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_mul");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_mul");

    if (bn_is_zero(num1) || bn_is_zero(num2)) {
        return bn_set_zero(res);
    }

    num1_bit_len = bn_num_bits(num1);

    /* if num1_bit_len is valid */
    if (num1_bit_len <= 0) {
        return bn_set_zero(res);
    }

    /* temp for double operation*/
    temp = bn_new(arena);
    MUST(temp != NULL, "Failed to allocate temp in bn_mul");

    if(bn_set_zero(res) < 0){
        return -1;
    }

    bn_copy(res, num2, arena);

    for (i = num1_bit_len - 2; i >= 0; i--) {
        bn_copy(temp, res, arena);

        /* Double the res (shift left by 1) */
        bn_lshift(res, temp, 1, arena);

        /* if bit is set, add num2 to res */
        if (bn_is_bit_set(num1, i)) {
            /* Copy res to temp */
            bn_copy(temp, res, arena);

            /* Add num2 to res */
            bn_uadd(res, temp, num2, arena);
        }
    }

    /* Set the sign of the result */
    res->negative = (num1->negative != num2->negative) && !bn_is_zero(res);
    if(arena == NULL){
        bn_free(temp);
    }
    return 0;
}
