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

/*
Function return conventions:

==> Arithmetic operations: 0 = success, -1 = error
    (Note: Functions like bn_uadd always return 0 since memory errors 
    are handled via assertions - the return value is for consistency)

==> Comparison functions: return comparison result (-1, 0, 1, 2)

==> Memory errors handled via assertions

==> Most of the utils that stores the result in the passed `res` parameter assumes 
    that `res` is a valid pointer to a valid BigNum 
    and res is resized if required

==> TODOs for BigNum Operations:
    [] Optimize division using for example (optimized school division, Newton-Raphson, Burnikel-Ziegler(very large numbers)).
    [] Optimize Multiplication using Karatsuba algorithm.
    [] Optimize exponentiation using CRT and Reduction algorithms.
    [] Implement robust error handling system using error codes.
    [] Implement cryptographic utilities: PRNG, and Fermat primality test
    [] Implement utils to convert the BigNum to different radixes(decimal, hex and binary)
    [] Implement utils to convert the BigNum to different radixes(decimal, hex and binary)

*/

#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>
#include <arena_allocator.h>

/* Previously had custom handling for architectures, but this (from libtommath) is a better solution */
#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || \
    defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || \
    defined(__s390x__) || defined(__arch64__) || defined(__aarch64__) || \
    defined(__sparcv9) || defined(__sparc_v9__) || defined(__sparc64__) || \
    defined(__ia64) || defined(__ia64__) || defined(__itanium__) || defined(_M_IA64) || \
    defined(__LP64__) || defined(_LP64) || defined(__64BIT__)
    typedef uint64_t BigNumWord;
    #define BN_WORD_SIZE 64
    #define BN_MASK1 ((BigNumWord)-1)
#elif defined(__i386__) || defined(__i686__)
    typedef uint32_t BigNumWord;
    #define BN_WORD_SIZE 32
    #define BN_MASK1 ((BigNumWord)-1)
#endif

#define BN_DEFAULT_WORDS_SIZE 24

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


BigNumWord* bn_alloc_words(size_t size, Arena *arena);                                              /*done*/
BigNum* bn_new(Arena *arena);                                                                       /*done*/
void bn_init(BigNum *num, Arena *arena);                                                            /*done*/
BigNum* bn_zero(Arena *arena);                                                                      /*done*/
int bn_set_zero(BigNum *num);                                                                       /*done*/
void bn_resize(BigNum* num, size_t new_size, Arena *arena);                                         /*done*/

void bn_copy(BigNum *dest, const BigNum *src, Arena *arena);                                        /*done*/
BigNum* bn_dup(const BigNum *src, Arena *arena);                                                    /*done*/
void bn_append_word(BigNum* num, const BigNumWord word, Arena *arena);                              /*done*/
void bn_prepend_zero_words(BigNum* num, size_t cnt , Arena *arena);                                 /*done*/
void bn_print_word(const BigNumWord word, char format);
void bn_print_words(const BigNum* num, char format);                                                /*done*/
void bn_print(const BigNum* num, char format);                                                      /*done*/
void bn_free_words(BigNum *num);                                                                    /*done*/
void bn_free(BigNum *num);                                                                          /*done*/

/* Conversion */
char *bn_bn2dec(const BigNum *num, Arena *arena);
char *bn_bn2hex(const BigNum *num, Arena *arena);
char *bn_bn2bin(const BigNum *num, Arena *arena);
BigNum *bn_from_int(int n, Arena* arena);                                                           /*done*/
BigNum *bn_from_bin(const char *str, size_t len, Arena *arena);                                     /*done*/
BigNum *bn_from_dec(const char *str, size_t len, Arena *arena);                                     /*done*/
BigNum *bn_from_hex(const char *str, size_t len, Arena *arena);                                     /*done*/

/* Arithmetic */

/* 
 * The int return type is used for consistency with other arithmetic utility functions.
 * This function always returns 0, as any memory issues are caught by assertions. 
 */
int bn_uadd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                     /*done*/

int bn_add(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/
int bn_usubtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                /*done*/
int bn_subtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                 /*done*/
int bn_mul(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/

/* res = num ^ pow */
int bn_pow(BigNum *res, const BigNum *num, const BigNum *pow, Arena *arena);                        /*done*/

/*
 * Euclidean Division (https://en.wikipedia.org/wiki/Euclidean_division)
 * following this equation: (num1 = q * num2 + remain) as 0 <= remain < |num2| .
 */
int bn_div(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena);        /*done*/

/*
 * divide num1 by num2 discarding their signs and compute q and remain
 */
int bn_udiv(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena);       /*done*/

int bn_mod(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/
int bn_gcd(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/

/* Bitwise Operation */
BigNumWord bn_lshift_word(const BigNumWord word, size_t nbits);                                     /*done*/
int bn_lshift(BigNum *res, const BigNum *num, size_t nbits, Arena *arena);                          /*done*/
BigNumWord bn_rshift_word(const BigNumWord word, size_t nbits);                                     /*done*/
int bn_rshift(BigNum *res, const BigNum *num1, int nbits);
int bn_and(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/
int bn_or(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                       /*done*/
int bn_xor(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);                      /*done*/

/* Modular Arithmetic */
int bn_mod_pow(BigNum *res, const BigNum *num1, const BigNum *pow, const BigNum *mod, Arena *arena);/*done*/
int bn_mod_inverse(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena);              /*done*/

/*
 * return values:
 * -1 --> indicates error
 *  0 --> num1 > num2
 *  1 --> num1 < num2
 *  2 --> num1 = num2
*/
int bn_compare(const BigNum *num1, const BigNum *num2);                                             /*done*/
int bn_ucompare(const BigNum *num1, const BigNum *num2);                                            /*done*/
int bn_is_zero(const BigNum *num);                                                                  /*done*/
int bn_is_one(const BigNum *num);                                                                   /*done*/

int bn_num_bits_word(const BigNumWord word);                                                        /*done*/
int bn_num_bits(const BigNum *num);                                                                 /*done*/
int bn_num_bytes(const BigNum *num);                                                                /*done*/
int bn_is_negative(const BigNum *num);                                                              /*done*/
int bn_set_bit(BigNum *num, int n);                                                                 /*done*/
int bn_unset_bit(BigNum *num, int n);                                                               /*done*/
int bn_is_bit_set(const BigNum *num, int n);                                                        /*done*/

#endif /*BigNum*/
