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

#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>
#include <arena_allocator.h>

#if defined(__x86_64__)
    typedef uint64_t BigNumWord;
    #define BIGNUM_WORD_SIZE 64
    #define BIGNUM_MASK1 ((BigNumWord)-1)
#elif defined(__i386__) || defined(__i686__)
    typedef uint32_t BigNumWord;
    #define BIGNUM_WORD_SIZE 32
    #define BIGNUM_MASK1 ((BigNumWord)-1)
#endif

#define BIGNUM_DEFAULT_WORDS_SIZE 24

#define DEBUG_BIGNUM(num) \
do {\
    printf("--------------------------------\n");  \
    printf("BigNum: %p\n", &(num));                \
    printf("data: %p\n", (num).words);             \
    printf("size: %zu\n", (num).size);             \
    printf("capacity: %zu\n", (num).capacity);     \
    printf("negative: %d\n", (num).negative);      \
    printf("--------------------------------\n");  \
} while(0)\

typedef struct {
    BigNumWord *words;
    size_t size;
    size_t capacity;
    int negative;
} BigNum;


int dec_to_hex_string(const char *dec_str, char *hex_str, size_t hex_size);
BigNum* bignum_new(Arena *arena);                                                                       /*done*/
BigNum* bignum_zero(Arena *arena);                                                                      /*done*/
int bignum_set_zero(BigNum *num);                                                                       /*done*/
void bignum_resize(BigNum* num, size_t new_size, Arena *arena);                                         /*done*/
int bignum_copy(BigNum *dest, BigNum *src, Arena *arena);                                               /*done*/
BigNum* bignum_dup(BigNum *src, Arena *arena);                                                          /*done*/
int bignum_append_word(BigNum* num, const BigNumWord word, Arena *arena);                               /*done*/
int bignum_prepend_zero_words(BigNum* num, size_t cnt , Arena *arena);                                  /*done*/
void bignum_print_words(const BigNum* num, char format);                                                /*done*/
void bignum_print(BigNum* num, char format);                                                            /*done*/
void bignum_free(BigNum *num);                                                                          /*done*/

/*Conversion*/
int *bignum_bn2dec(const BigNum *num, char *dest, size_t len);
int *bignum_bn2hex(const BigNum *num, char *dest, size_t len);
int *bignum_bn2bin(const BigNum *num, char *dest, size_t len);
BigNum *bignum_from_int(int n, Arena* arena);                                                           /*done*/
BigNum *bignum_from_bin(const char *str, size_t len, Arena *arena);                                     /*done*/
BigNum *bignum_from_dec(const char *str, size_t len, Arena *arena);                                     /*done*/
BigNum *bignum_from_hex(const char *str, size_t len, Arena *arena);                                     /*done*/

/* Arithmetic */
int bignum_uadd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                     /*done*/
int bignum_add(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/
int bignum_usubtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                /*done*/
int bignum_subtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                 /*done*/
int bignum_mul(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);
int bignum_div(BigNum *div, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena);
int bignum_mod(BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena);
int bignum_gcd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);

/*Bitwise Operation*/
BigNumWord bignum_lshift_word(BigNumWord word, size_t nbits);                                           /*done*/
int bignum_lshift(BigNum *res, BigNum *num, size_t nbits, Arena *arena);                                /*done*/
BigNumWord bignum_rshift_word(BigNumWord word, size_t nbits);                                           /*done*/
int bignum_rshift(BigNum *res, BigNum *num1, int nbits);
int bignum_and(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/
int bignum_or(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                       /*done*/
int bignum_xor(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/


/* Modular Arithmetic */
int bignum_mod_exp(BigNum *res, const BigNum *num1, const BigNum *exp, const BigNum *m, Arena *arena);
int bignum_mod_inverse(BigNum *res, const BigNum *num1, const BigNum *n, Arena *arena);

int bignum_compare(const BigNum *num1, const BigNum *num2);                                             /*done*/
int bignum_ucompare(const BigNum *num1, const BigNum *num2);                                            /*done*/
int bignum_is_zero(const BigNum *num);                                                                  /*done*/
int bignum_is_one(const BigNum *num);                                                                   /*done*/

int bignum_num_bits_word(BigNumWord word);                                                              /*done*/
int bignum_num_bits(const BigNum *num);                                                                 /*done*/
int bignum_num_bytes(const BigNum *num);                                                                /*done*/
int bignum_is_negative(const BigNum *num);                                                              /*done*/
int bignum_set_bit(BigNum *num, int n);                                                                 /*done*/
int bignum_unset_bit(BigNum *num, int n);                                                               /*done*/
int bignum_is_bit_set(const BigNum *num, int n);                                                        /*done*/

#endif /*BigNum*/
