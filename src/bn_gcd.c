/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

 /*
 * Greatest Common Divisor using Euclidean algorithm
 * gcd(a, b) = gcd(b, a mod b) until b = 0
 * gcd is always +ve
 */
int
bn_gcd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{

    BigNum *a, *b, *temp;
    MUST(res  != NULL, "res pointer is NULL in bn_gcd");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_gcd");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_gcd");

    /* zero cases */
    if (bn_is_zero(num1)) {
        bn_copy(res, num2, arena);
        res->negative = 0;
        return 0;
    }

    if (bn_is_zero(num2)) {
        bn_copy(res, num1, arena);
        res->negative = 0;
        return 0;
    }

    a = bn_dup(num1, arena);
    b = bn_dup(num2, arena);
    a->negative = 0;
    b->negative = 0;

    temp = bn_new(arena);

    /* gcd(a, b) = gcd(b, a mod b) */
    while (!bn_is_zero(b)) {
        if (bn_mod(temp, a, b, arena) < 0) {
            return -1;
        }

        // a = b, b = temp (a mod b)
        bn_copy(a, b, arena);
        bn_copy(b, temp, arena);
    }

    bn_copy(res, a, arena);

    if(arena == NULL){
        bn_free(a);
        bn_free(b);
        bn_free(temp);
    }

    return 0;
}
