/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include<bignum.h>
#include<utils.h>

int
bn_uadd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    BigNumWord carry, sum, a;
    const BigNum *temp;
    size_t max_size, min_size, i;
    MUST(res  != NULL, "res pointer is NULL in bn_uadd");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_uadd");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_uadd");

    /* swap num1 with num2 */
    if(num1->size < num2->size){
        temp = num1;
        num1 = num2;
        num2 = temp;
    }

    max_size = num1->size;
    min_size = num2->size;

    bn_resize(res, max_size + 1, arena);

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


    /* loop for remaining words in the larger number (num1) */
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
