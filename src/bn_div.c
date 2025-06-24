/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>
/*
 * TODO: Fix memory leaks caused by q_u & remain_u & temp & one variables
 */
int
bn_div(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{

    BigNum *q_u, *remain_u, *one, *temp;
    MUST(q      != NULL, "q pointer is NULL in bn_div");
    MUST(num1   != NULL, "num1 pointer is NULL in bn_div");
    MUST(num2   != NULL, "num2 pointer is NULL in bn_div");
    MUST(remain != NULL, "remain pointer is NULL in bn_div");

    /*init our variables*/
    q_u = bn_new(arena); 
    remain_u = bn_new(arena); 
    temp = bn_new(arena); 
    one = bn_from_int(1, arena); 

    if(bn_udiv(q_u, remain_u, num1, num2, arena) < 0){
        return -1;
    }

    /*num1 > 0 && num2 > 0*/
    if(!bn_is_negative(num1) && !bn_is_negative(num2)){
        bn_copy(q, q_u, arena);
        bn_copy(remain, remain_u, arena);
    } 
    /*num1 < 0 && num2 > 0*/
    else if(bn_is_negative(num1) && !bn_is_negative(num2)){
        if(bn_is_zero(remain_u)){
            bn_copy(q, q_u, arena); 
            q->negative = q->negative ? 0: 1; /* q = -q_unsigned*/
            bn_set_zero(remain);/* remain = 0*/
        } 
        else{
            bn_add(temp, q_u, one, arena); /*temp = q_unsigned + 1*/
            bn_copy(q, temp, arena); /*q = temp*/
            q->negative = q->negative ? 0: 1; /* q = -temp = -(q_unsigned + 1) = -q_unsigned - 1*/
            bn_subtract(remain, num2, remain_u, arena); /*remain = num2 - remain_u*/
        }

    }
    /*num1 > 0 && num2 < 0*/
    else if(!bn_is_negative(num1) && bn_is_negative(num2)){
        if(bn_is_zero(remain_u)){
            bn_copy(q, q_u, arena); 
            q->negative = q->negative ? 0: 1; /* q = q_unsigned*/
            bn_set_zero(remain);/* remain = 0*/
        } 
        else{
            // bn_uadd(temp, q_u, one, arena); /*temp = |q_unsigned| + 1*/
            bn_copy(q, q_u, arena); /*q = temp*/
            q->negative = q->negative ? 0: 1; /* q = -temp = -(|q_unsigned| + 1) = -|q_unsigned| - 1*/
            bn_copy(remain, remain_u, arena); /*q = temp*/
        }
    }
    /*num1 < 0 && num2 < 0*/
    else{
        if(bn_is_zero(remain_u)){
            bn_copy(q, q_u, arena); 
            bn_set_zero(remain);/* remain = 0*/
        } 
        else{
            bn_uadd(temp, q_u, one, arena); /*temp = |q_unsigned| + 1*/
            bn_copy(q, temp, arena); /*q = temp*/
            bn_usubtract(remain, num2, remain_u, arena); /*remain = num2 - remain_u*/
        }
    }

    if(arena == NULL){
        bn_free(q);
        bn_free(num1);
        bn_free(num2);
        bn_free(remain);
    }

    return 0;
}
