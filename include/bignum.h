#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>
#include <arena_allocator.h>

#if defined(__x86_64__)
    typedef uint64_t BIGNUM_WORD;
    #define BIGNUM_WORD_SIZE 64
#elif defined(__i386__) || defined(__i686__)
    typedef uint32_t BIGNUM_WORD;
    #define BIGNUM_WORD_SIZE 32
#endif


#define BIGNUM_DEFAULT_WORDS_SIZE 16
#define TOHEX(number) bignum_int2hex((BIGNUM_WORD)(number))
#define DEBUG_BIGNUM(num) \
do {\
    printf("--------------------------------\n");  \
    printf("BigNum: %p\n", &(num));                \
    printf("data: %p\n", (num).words);             \
    printf("size: %zu\n", (num).size);             \
    printf("capacity: %zu\n", (num).capacity);     \
    printf("negative: %d\n", (num).negative);      \
} while(0)\

typedef struct {
    BIGNUM_WORD *words;
    size_t size;
    size_t capacity;
    int negative;
} BigNum;


BigNum* bignum_new(Arena *arena);
uint8_t bignum_copy(BigNum *dest, BigNum *src, Arena *arena);
BigNum* bignum_dup(BigNum *src, Arena *arena);
char *bignum_2dec(const BigNum *num);
char *bignum_2hex(const BigNum *num);

/* Basic Arithmetic */
int bignum_add(BigNum *res, const BigNum *a, const BigNum *b);
int bignum_sub(BigNum *res, const BigNum *a, const BigNum *b);
int bignum_mul(BigNum *res, const BigNum *a, const BigNum *b, Arena *arena);
int bignum_div(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);
int bignum_mod(BigNum *rem, const BigNum *a, const BigNum *m, Arena *arena);

/* Modular Arithmetic */
int bignum_mod_exp(BigNum *res, const BigNum *a, const BigNum *p, const BigNum *m, Arena *arena);
int bignum_mod_inverse(BigNum *res, const BigNum *a, const BigNum *n, Arena *arena);

int bignum_cmp(const BigNum *a, const BigNum *b);
int bignum_is_zero(const BigNum *a);
int bignum_is_one(const BigNum *a);
int bignum_is_negative(const BigNum *a);

#endif /*BigNum*/
