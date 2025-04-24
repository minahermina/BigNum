#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>
#include <arena_allocator.h>

#if defined(__x86_64__)
    typedef uint64_t BIGNUM_WORD;
    #define BIGNUM_WORD_SIZE 64
#endif
#if defined(__x86_32__)
    typedef uint32_t BIGNUM_WORD;
    #define BIGNUM_WORD_SIZE 32
#endif

#define DEBUG_BIGNUM(num) \
do {\
    printf("BigNum: %p\n", &(num));       \
    printf("data: %p\n", (num).digits);       \
    printf("size: %d\n", (num).size);         \
    printf("capacity: %d\n", (num).capacity); \
    printf("negative: %d\n", (num).negative); \
} while(0)\

typedef struct {
    BIGNUM_WORD *digits;
    uint16_t size;
    uint16_t capacity;
    int negative;
} BigNum;

BigNum* bignum_new(Arena *arena);
uint8_t bignum_copy(BigNum *dest, BigNum *src, Arena *arena);
BigNum* bignum_dup(BigNum *src, Arena *arena);
char *bignum_2dec(const BigNum *num);
char *bignum_2hex(const BigNum *num);

/* Basic Arithmetic */
int bignum_add(BigNum *r, const BigNum *a, const BigNum *b);
int bignum_sub(BigNum *r, const BigNum *a, const BigNum *b);
int bignum_mul(BigNum *r, const BigNum *a, const BigNum *b, Arena *arena);
int bignum_div(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);
int bignum_mod(BigNum *rem, const BigNum *a, const BigNum *m, Arena *arena);

/* Modular Arithmetic */
int bignum_mod_exp(BigNum *r, const BigNum *a, const BigNum *p, const BigNum *m, Arena *arena);
int bignum_mod_inverse(BigNum *r, const BigNum *a, const BigNum *n, Arena *arena);

int bignum_cmp(const BigNum *a, const BigNum *b);
int bignum_is_zero(const BigNum *a);
int bignum_is_one(const BigNum *a);
int bignum_is_negative(const BigNum *a);

#endif /*BigNum*/
