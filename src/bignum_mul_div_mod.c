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
 * this uses the double-and-add algorithm
 * which is not efficient but i needed this function just to work :)
*/
int bignum_mul(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int i, num1_bit_len;
    BigNum *temp;
    MUST(res != NULL, "res pointer is NULL in bignum_mul");
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_mul");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_mul");

    if (bignum_is_zero(num1) || bignum_is_zero(num2)) {
        return bignum_set_zero(res);
    }

    num1_bit_len = bignum_num_bits(num1);

    /* if num1_bit_len is valid */
    if (num1_bit_len <= 0) {
        return bignum_set_zero(res);
    }

    /* temp for double operation*/
    temp = bignum_new(arena);
    MUST(temp != NULL, "Failed to allocate temp in bignum_mul");

    bignum_set_zero(res);

    bignum_copy(res, num2, arena);

    for (i = num1_bit_len - 2; i >= 0; i--) {
        bignum_copy(temp, res, arena);

        /* Double the res (shift left by 1) */
        bignum_lshift(res, temp, 1, arena);

        /* if bit is set, add num2 to res */
        if (bignum_is_bit_set(num1, i)) {
            /* Copy res to temp */
            bignum_copy(temp, res, arena);

            /* Add num2 to res */
            bignum_uadd(res, temp, num2, arena);
        }
    }

    /* Set the sign of the result */
    res->negative = (num1->negative != num2->negative) && !bignum_is_zero(res);
    if(arena == NULL){
        bignum_free(temp);
    }
    return 0;
}


/*
 * (num1 = q * num2 + remain)
 *
 * TODO: Optimize this slow udiv function
 */
int
bignum_udiv(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    BigNum temp1, sub_res, *temp_q;
    size_t cnt;
    int ucompare_result;

    /* TODO: implement a util to init a pre-existed BigNum */
    bignum_init(&temp1, arena);
    bignum_init(&sub_res, arena);

    MUST(q      != NULL, "q pointer is NULL in bignum_udiv");
    MUST(num1   != NULL, "num1 pointer is NULL in bignum_udiv");
    MUST(num2   != NULL, "num2 pointer is NULL in bignum_udiv");
    MUST(remain != NULL, "remain pointer is NULL in bignum_udiv");
    MUST(bignum_is_zero(num2) == 0, "num2 is zero, division by zero is undefined in bignum_udiv");

    /* num1 == num2, q = 0, remain = num1 */
    if(bignum_ucompare(num1, num2) == 1){
        bignum_copy(remain, num1, arena);

        if(bignum_set_zero(q) < 0){
            return -1;
        }
    }

    bignum_copy(&sub_res, num1, arena);
    bignum_set_zero(q);

    cnt = 0;
    ucompare_result = bignum_ucompare(&sub_res, num2);
    while(ucompare_result == 0 || ucompare_result == 2){
        bignum_copy(&temp1, &sub_res, arena);
        if(bignum_usubtract(&sub_res, &temp1, num2, arena) < 0){
            return -1;
        }
        ucompare_result = bignum_ucompare(&sub_res, num2);
        cnt++;
    }

    /* now remain = sub_res, remain = cnt */
    bignum_copy(remain, &sub_res, arena);

    temp_q = bignum_from_int(cnt, arena);
    bignum_copy(q, temp_q, arena);
    return 0;
}

/*
 * TODO: Fix memory leaks caused by q_u & remain_u & temp & one variables
 */
int
bignum_div(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{

    BigNum q_u, remain_u, *one, temp;
    MUST(q      != NULL, "q pointer is NULL in bignum_div");
    MUST(num1   != NULL, "num1 pointer is NULL in bignum_div");
    MUST(num2   != NULL, "num2 pointer is NULL in bignum_div");
    MUST(remain != NULL, "remain pointer is NULL in bignum_div");

    /*init our variables*/
    bignum_init(&q_u, arena); 
    bignum_init(&remain_u, arena); 
    bignum_init(&temp, arena); 
    one = bignum_from_int(1, arena); 

    if(bignum_udiv(&q_u, &remain_u, num1, num2, arena) < 0){
        return -1;
    }

    /*num1 > 0 && num2 > 0*/
    if(!bignum_is_negative(num1) && !bignum_is_negative(num2)){
        bignum_copy(q, &q_u, arena);
        bignum_copy(remain, &remain_u, arena);
    } 
    /*num1 < 0 && num2 > 0*/
    else if(bignum_is_negative(num1) && !bignum_is_negative(num2)){
        if(bignum_is_zero(&remain_u)){
            bignum_copy(q, &q_u, arena); 
            q->negative = q->negative ? 0: 1; /* q = q_unsigned*/
            bignum_set_zero(remain);/* remain = 0*/
        } 
        else{
            bignum_add(&temp, &q_u, one, arena); /*temp = q_unsigned + 1*/
            bignum_copy(q, &temp, arena); /*q = temp*/
            q->negative = q->negative ? 0: 1; /* q = -temp = -(q_unsigned + 1) = -q_unsigned - 1*/
            bignum_subtract(remain, num2, &remain_u, arena); /*remain = num2 - remain_u*/
        }

    }
    /*num1 > 0 && num2 < 0*/
    else if(!bignum_is_negative(num1) && bignum_is_negative(num2)){
        if(bignum_is_zero(&remain_u)){
            bignum_copy(q, &q_u, arena); 
            q->negative = q->negative ? 0: 1; /* q = q_unsigned*/
            bignum_set_zero(remain);/* remain = 0*/
        } 
        else{
            // bignum_uadd(&temp, &q_u, one, arena); /*temp = |q_unsigned| + 1*/
            bignum_copy(q, &q_u, arena); /*q = temp*/
            q->negative = q->negative ? 0: 1; /* q = -temp = -(|q_unsigned| + 1) = -|q_unsigned| - 1*/
            bignum_copy(remain, &remain_u, arena); /*q = temp*/
        }
    }
    /*num1 < 0 && num2 < 0*/
    else{
        if(bignum_is_zero(&remain_u)){
            bignum_copy(q, &q_u, arena); 
            bignum_set_zero(remain);/* remain = 0*/
        } 
        else{
            bignum_uadd(&temp, &q_u, one, arena); /*temp = |q_unsigned| + 1*/
            bignum_copy(q, &temp, arena); /*q = temp*/
            bignum_usubtract(remain, num2, &remain_u, arena); /*remain = num2 - remain_u*/
        }
    }
    return 0;
}

/*
int
bignum_mod(BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{
}
*/
