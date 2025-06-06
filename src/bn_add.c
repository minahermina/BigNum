/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

int
bn_add(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int compare_result, ret;
    MUST(res  != NULL, "res pointer is NULL in bn_add");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_add");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_add");


    if(bn_is_negative(num1) == bn_is_negative(num2)){
        ret = bn_uadd(res, num1, num2, arena);
        res->negative = num1->negative;
    } else{
        compare_result = bn_ucompare(num1, num2);
        /*
         * num1 > num2
         * + > -
         */
        if(compare_result == 0) {
            res->negative = 1;
            ret = bn_usubtract(res, num1, num2, arena);
        } 
        /*
         * num1 < num2
         * - > +
         */
        else if(compare_result == 1) {
            ret = bn_usubtract(res, num2, num1, arena);
            res->negative = 1;
        } 
        /*
         * num1 = num2
         * - = +
         * each one is zero
         */
        else{
            ret = bn_set_zero(res);
            res->negative = 0;
        }
    }

    return ret;
}
