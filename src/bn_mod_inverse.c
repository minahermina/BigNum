/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

/*
 * uses Extended Euclidean Algorithm to find the modular inverse based on the description 
 * in `Understanding Cryptography` Book by Prof. Christof Paar in Chapter 6
 * 
 * Can be optimized by looking for special cases like if num2 is odd, then use Binary EEA,
 * but for now this works just fine 
 */
int
bn_mod_inverse(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    BigNum *s0, *s1, *t0, *t1, *q, *r, *s, *t, *r0, *r1, *zero, *temp;
    MUST(res  != NULL, "res pointer is NULL in bn_add");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_add");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_add");

    /* num1 !< num2 */
    if(bn_compare(num1, num2) != 1){
        return -1;
    }

    s0 = bn_from_int(1, arena);
    s1 = bn_from_int(0, arena);
    t0 = bn_from_int(0, arena);
    t1 = bn_from_int(1, arena);
    zero = bn_from_int(0, arena);

    q = bn_new(arena);
    r = bn_new(arena);
    s = bn_new(arena);
    t = bn_new(arena);
    temp = bn_new(arena);

    r0 = bn_dup(num1, arena);
    r1 = bn_dup(num2, arena);

    while(bn_compare(r1, zero) != 2){
        // q_{i-1} = (r_{i-2})/(r_{i-1})
        bn_div(q, temp, r0, r1, arena);

        // r_i = r_{i-2} mod r_{i-1}
        bn_mod(r, r0, r1, arena);

        // s_i = s_{i-2} - q_{i-1}·s_{i-1}
        bn_mul(temp, q, s1, arena);
        bn_subtract(s, s0, temp, arena);

        // t_i = t_{i-2} - q_{i-1}·t_{i-1}
        bn_mul(temp, q, t1, arena);
        bn_subtract(t, t0, temp, arena);

        // r0 = r1;
        bn_copy(r0, r1, arena);
        // r1 = r;
        bn_copy(r1, r, arena);
        // s0 = s1;
        bn_copy(s0, s1, arena);
        // s1 = s;
        bn_copy(s1, s, arena);
        // t0 = t1;
        bn_copy(t0, t1, arena);
        // t1 = t;
        bn_copy(t1, t, arena);
    }

    // Return s_{i-1} which is s0
    // Make sure s0 is positive
    while (bn_compare(s0, zero) == 1) {
        bn_copy(temp, s0, arena);
        bn_add(s0, temp, num2, arena);
    }
    bn_copy(res, s0, arena);

    if(arena == NULL){
        bn_free(r0);
        bn_free(r1);
        bn_free(t0);
        bn_free(t1);
        bn_free(s0);
        bn_free(s1);
        bn_free(q);
        bn_free(zero);
        bn_free(r);
        bn_free(t);
        bn_free(temp);
    }
}
