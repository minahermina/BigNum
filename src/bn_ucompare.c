/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

/*
 *  NOTE: this function assume that either num1 or num2 has no leading zero words
 * -1 --> indicates error
 *  0 --> num1 > num2
 *  1 --> num1 < num2
 *  2 --> num1 = num2
*/

int
bn_ucompare(const BigNum *num1, const BigNum *num2)
{
    size_t i;
    MUST(num1 != NULL, "num1 pointer is NULL in bn_ucompare");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_ucompare");

    if(num1->size > num2->size){
        return 0;
    }
    else if(num1->size < num2->size){
        return  1;
    }

    /* num1->size == num2->size */
    for(i = 0; i < num1->size; ++i){
        if(num1->words[i] > num2->words[i]){
            return  0;
        }

        if(num1->words[i] < num2->words[i]){
            return  1;
        }
    }

    return 2;
}
