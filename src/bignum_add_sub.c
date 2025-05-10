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

#include <inttypes.h>
#include <assert.h>
#include <bignum.h>
#include <utils.h>



int
bignum_uadd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    BigNumWord carry, sum, a;
    const BigNum *temp;
    size_t max_size, min_size, i;
    MUST(res  != NULL, "res pointer is NULL in bignum_uadd");
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_uadd");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_uadd");

    /*swap num1 with num2*/
    if(num1->size < num2->size){
        temp = num1;
        num1 = num2;
        num2 = temp;
    }

    max_size = num1->size;
    min_size = num2->size;

    bignum_resize(res, max_size + 1, arena);

    carry = 0;
    for(i = 0; i < min_size; ++i){
        sum = num1->words[i] + num2->words[i];
        /* overflow happens */
        res->words[i] = sum + carry;
        if(sum < num1->words[i]){
            carry = 1 + (res->words[i] < sum);
        } else{
            carry = (res->words[i] < sum);
        }
    }


    /*loop for remaining words in the larger number (num1)*/
    for (; i < max_size; i++) {
        a = (i < num1->size) ? num1->words[i] : 0;
        sum = a + carry;
        res->words[i] = sum;
        carry = (sum < a);
    }

    /* Handle final carry */
    if (carry) {
        res->words[i] = 1;
        res->size = i + 1;
    } else {
        res->size = i;
    }

    res->negative = 0;
    return 0;
}

int
bignum_add(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int compare_result, ret;
    MUST(res  != NULL, "res pointer is NULL in bignum_add");
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_add");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_add");


    if(bignum_is_negative(num1) == bignum_is_negative(num2)){
        ret = bignum_uadd(res, num1, num2, arena);
        res->negative = num1->negative;
    } else{
        compare_result = bignum_ucompare(num1, num2);
        /*
         * num1 > num2
         * + > -
         */
        if(compare_result == 0) {
            res->negative = 1;
            ret = bignum_usubtract(res, num1, num2, arena);
        } 
        /*
         * num1 < num2
         * - > +
         */
        else if(compare_result == 1) {
            ret = bignum_usubtract(res, num2, num1, arena);
            res->negative = 1;
        } 
        /*
         * num1 = num2
         * - = +
         * each one is zero
         */
        else{
            ret = bignum_set_zero(res);
            res->negative = 0;
        }
    }

    return ret;
}

/* The arena is unused here but retained for interface consistency. */
int
bignum_usubtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    (void)arena;
    size_t i;
    BigNumWord a, b, diff, borrow, cmp;
    MUST(res  != NULL, "res pointer is NULL in bignum_usubtract");
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_usubtract");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_usubtract");

    /*check for num1 > num2*/
    cmp = bignum_ucompare(num1, num2);
    if (cmp == 1){
        return -1;
    }

    borrow = 0;
    for(i = 0; i < num1->size; ++i){
        a = num1->words[i];
        b = (i < num2->size ? num2->words[i] : 0); /*as num1 is longer than num2*/

        if(a < b + borrow){ /* borrow needed */
            diff = (a + BIGNUM_MASK1 + 1) - a - borrow;
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

int
bignum_subtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int compare_result, ret;
    MUST(res  != NULL, "res pointer is NULL in bignum_subtract");
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_subtract");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_subtract");

    /*
     * +num1 - -num2 =  num1 + num2;
     * -num1 - +num2 =  -(num1 + num2);
     */
    if(bignum_is_negative(num1) != bignum_is_negative(num2)){
        ret = bignum_uadd(res, num1, num2, arena);
        res->negative = num1->negative;
    } else{
        compare_result = bignum_ucompare(num1, num2);
        /*
         * -5 - -3 = (5-3) * -1 = -2
         */
        if(compare_result == 0) {
            ret = bignum_usubtract(res, num1, num2, arena);
            res->negative = num1->negative;
        }
        /*
         * -5 - -7 = (7-5) * opposite sign of (-5) = -2
         */
        else if(compare_result == 1){
            ret = bignum_usubtract(res, num2, num1, arena);
            res->negative = !(num1->negative);
        }
        /*
         * num1 == num2
         */
        else {
            ret = bignum_set_zero(res);
            res->negative = 0;
        }
    }

    return ret;
}
