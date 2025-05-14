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

    return 0;
}


/*
int
bignum_div(BigNum *div, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{
}

int
bignum_mod(BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{
}
*/
