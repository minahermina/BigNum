/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

/* The arena is unused here but retained for interface consistency. */
int
bn_usubtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    (void)arena;
    size_t i;
    BigNumWord a, b, diff, borrow, cmp;
    MUST(res  != NULL, "res pointer is NULL in bn_usubtract");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_usubtract");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_usubtract");

    /* check if num1 < num2 */
    cmp = bn_ucompare(num1, num2);
    if (cmp == 1){
        return -1;
    }

    borrow = 0;
    for(i = 0; i < num1->size; ++i){
        a = num1->words[i];
        b = (i < num2->size ? num2->words[i] : 0); /* as num1 is longer than num2 */

        if(a < b + borrow){ /* borrow needed */
            diff = (a + BN_MASK1 + 1) - a - borrow;
            res->words[i] = diff;
            borrow = 1;
        } else {/* no borrow ! */
            diff = a - b - borrow;
            borrow = 0;
        }
        res->words[i] = diff;
    }

    res->size = num1->size;
    res->negative = 0;

    return 0;
}
